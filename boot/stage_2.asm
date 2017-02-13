[bits 16]

;The stage 1 bootloader will jump here as soon as it completes
jmp stage_2_entry

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

install_gdt:
	cli
	pusha
	lgdt 	[GDT_POINTER]
	sti 
	popa 
	ret

stage_2_entry:
	mov 	si, STAGE_2_MSG
	call 	print_string
	mov 	si, CRLF
	call 	print_string
	;Set all segments to cs, which will be 0x50
	push 	cs
	pop 	ax
	mov 	ds, ax
	mov 	es, ax

	;Get ready to enter Protected Mode
	cli
	xor 	ax, ax
	mov 	ds, ax
	mov 	es, ax
	mov 	ax, 0x9000
	mov 	ss, ax
	mov 	sp, 0xFFFF
	sti

	call 	install_gdt

	cli
	mov 	eax, cr0
	or 		eax, 1
	mov 	cr0, eax
	;We are now in 32 bit mode!

	;Set all segments except cs to be data descriptors
	mov 	ax, 0x10
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax
	mov 	ss, ax
	mov 	esp, 0x90000
	mov 	ebp, esp

	;We are now in 32 bit mode, so we must user descriptor:offset addressing.
	;In order to clear out the old value in CS, we use an addressing mode
	;which will implicitly set CS to 0x08, which is the offset of 
	;the code descriptor in our GDT! Also, note that we do not use a call
	;to get to the kernel, as that would screw with our newly created stack.
	jmp 	0x08:_kernel_entry

	;The kernel never returns, so we will never hit this
	hlt

STAGE_2_MSG:	db	"Arrived in Stage 2!", 0
CRLF: 			db 	0x0D, 0x0A, 0x00

;GDT data
;Each GDT entry is 8 bytes
GDT_START:
GDT_NULL_DESCRIPTOR:
	dq 	0
GDT_CODE_DESCRIPTOR:
	dw 	0xFFFF  ;low bytes of the segment limit address
	dw  0		;low bytes of the segment base address
	db 	0		;middle byte of the segment base address
	db  10011010b	;access flags
	db  11001111b	;granularity flags
	db 	0			;high byte of the base address
GDT_DATA_DESCRIPTOR:
	dw 	0xFFFF
	dw 	0
	db 	0
	db 	10010010b	;bit 3 removes execute permissions when 0
	db	11001111b
	db 	0
GDT_END:
GDT_POINTER:
	dw GDT_END - GDT_START - 1  ;Size of the GDT MINUS 1!
	dd GDT_START 				;GDT base address

;32-bit code starts here
[bits 32]

[extern _kernel_entry]