SECTIONS
{
   . = 0x500;
   .text : { *(.text) }
   .data : { *(.data) *(.rodata)}
   .bss : { *(.bss) }
}