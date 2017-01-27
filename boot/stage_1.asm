;BIOS will load bootsector at this base address
;N.B. the bootsector will be limited to 512 bytes, 
;which is the size of a single disk sector.
;
;All of memory will be addressable, but only the first 
;512 bytes will actually be loaded into the system by
;the BIOS. This portion is also known as the "Stage 1"
;bootloader
[org 0x7C00]
[bits 16]			;x86 boots in 16-bit Real Mode

;Execution starts here
bootloader_entry:
	jmp bootloader_stage_1
;OEM Parameter block MUST go next, and begin at EXACTLY the
;fourth byte (i.e. 0x7C02); the first 3 bytes are JMP addr16
bpbOEM					db "MATTSOS", 0 ;Must be EXACTLY 8 bytes
bpbBytesPerSector:		dw 512
bpbSectorsPerCluster:	db 1
bpbReservedSectors:		dw 1
bpbNumberOfFATs:		db 2
pbRootEntries: 	    	dw 224
bpbTotalSectors: 	    dw 2880
bpbMedia: 	            db 0xF0
bpbSectorsPerFAT: 	    dw 9
bpbSectorsPerTrack: 	dw 18
bpbHeadsPerCylinder: 	dw 2
bpbHiddenSectors: 	    dd 0
bpbTotalSectorsBig:     dd 0
bsDriveNumber: 	        db 0
bsUnused: 	            db 0
bsExtBootSignature: 	db 0x29
bsSerialNumber:	        dd 0xA0A1A2A3
bsVolumeLabel: 	        db "MOS FLOPPY "
bsFileSystem: 	        db "FAT12   "

;INPUT: DS:SI must point to a null-terminated string
print_string:
	xor 	ax, ax
	xor		bx, bx		;faster than mov bx, 0
	
	print_string_loop:
	lodsb
	or 		al, al	;faster than cmp al, 0
	jz 		print_string_done
	mov 	ah, 0x0E
	int 	0x10	;BIOS teletype with ah=0x0E
	jmp 	print_string_loop

	print_string_done:
	ret

;INPUT: a 16-bit number in AX
print_hex_16:
	push 	ax
	mov 	si, HEX_PREFIX
	call 	print_string

	pop 	ax			;The actual number
	mov     cl, 0x0C	;Counter, decrements by 4

	print_hex_16_loop:
	mov 	bx, ax		;Use this register as a workspace
	shr 	bx, cl
	print_hex_16_evaluate_digit:
	and 	bl, 0x0F
	cmp 	bl, 0x0A
	jge 	print_hex_16_adjust_alpha
	add 	bl, 0x30
	jmp 	print_hex_16_print_char
	print_hex_16_adjust_alpha:
	add  	bl, 0x37
	print_hex_16_print_char:
	push 	ax
	push 	bx
	mov 	al, bl
	mov 	ah, 0x0E
	xor 	bx, bx
	int  	0x10
	pop 	bx 
	pop 	ax
	or 		dx, dx
	jnz 	print_hex_16_done
	sub  	cl, 0x04
	or 		cl, cl
	jnz 	print_hex_16_loop
	;Don't forget to print the final digit!
	mov 	dx, 1
	mov 	bx, ax
	jmp 	print_hex_16_evaluate_digit
	
	print_hex_16_done:
	ret

load_stage_2_from_disk:
	;First, we reset the floppy drive to read from sector 0,
	;which ensures that we are not reading from a random location
	load_stage_2_from_disk_reset:
	xor  	ax, ax
	mov 	dx, ax ;Reset the disk in drive 0
	int 	0x13 ;Reset disk drive with ah=0x00
	jc  	load_stage_2_from_disk_failure ;Stop if BIOS threw an error

	load_stage_2_from_disk_read:
	;BIOS will write the disk contents to ES:BX
	mov 	ax, 0x50
	mov 	es, ax
	mov 	bx, 0

	mov  	ah, 0x02
	mov 	al, 43 		;number of sectors to read
	mov 	ch, 0 		;cylinder number
	;Use the 2nd sector (cl=2), because the 
	;first sector contains the stage 1 bootloader
	mov 	cl, 2		;sector number
	mov 	dl, 0 	;dh=head number; dl=drive number
	mov 	dh, 0

	;Read & load a disk sector with ah=0x02
	int 	0x13
	jc  	load_stage_2_from_disk_failure ;Stop if BIOS threw an error
	xor ah, ah
	push ax
	mov si, SECTOR_STR
	call print_string
	pop ax
	call print_hex_16
	mov si, CRLF
	call print_string

	;Segment-crossing jumps implicitly change cs
	jmp 	0x50:0 	;Jump to the code we loaded

	load_stage_2_from_disk_failure:
	ret

bootloader_stage_1:
	xor 	ax, ax

	;zero-out the segments
	mov 	ds, ax
	mov 	es, ax

	mov 	si, BOOT_MSG
	call	print_string
	mov 	si, CRLF
	call 	print_string

	;Print the amount of RAM available
	mov 	si, AMOUNT_OF_RAM_STRING
	call	print_string
	;Places the amount of RAM available into AX (that is addressable in 16
	;bit mode)
	xor 	ax, ax
	int 	0x12
	call 	print_hex_16
	mov 	si, CRLF
	call 	print_string

	mov 	si, LOAD_STAGE_2_FROM_FLOPPY_STRING
	call 	print_string
	mov 	si, CRLF
	call 	print_string
	call 	load_stage_2_from_disk

	;Should never get here
	bootloader_stage_1_fatal_error:
	xor 	ax, ax
	mov 	ds, ax
	mov 	si, CRLF
	call 	print_string
	mov 	si, STAGE_1_ERR_STR
	call 	print_string
	cli
	hlt

BOOT_MSG:	db	"Booted into 16-bit Real Mode!", 0
CRLF: 		db 	0x0D, 0x0A, 0x00
HEX_PREFIX:	db	"0x", 0
AMOUNT_OF_RAM_STRING: 	db "Number of Kilobytes of RAM available: ", 0
LOAD_STAGE_2_FROM_FLOPPY_STRING:	db "Loading boot program...", 0
STAGE_1_ERR_STR: 	db "BOOTLOADER ERROR!", 0
SECTOR_STR: 	db "Sectors read: ", 0

times 510 - ($-$$) db 0

dw 0xAA55		;Bootsector signature