.globl uvisor_init
.type uvisor_init, %function
.section .text.uvisor, "x"
 .align 5
 .thumb
 .thumb_func
uvisor_init:
 .incbin "uvisor-MK64FN1M0XXX12.box"
 .long 0x2FE539A6
 .long 0
 .long 0
 .long __uvisor_cfgtbl_start
 .long __uvisor_cfgtbl_end
 .align 5
.section .bss.uvisor,"awM",@nobits
 .align 5
 .space 0x1000
 .align 5
