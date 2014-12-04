.globl uvisor_init
.globl uvisor_config
.type uvisor_init, %function
.weak __uvisor_mode
.section .text.uvisor, "x"
 .align 5
 .thumb
 .thumb_func
uvisor_init:
 .incbin "uvisor-MK64FN1M0XXX12.box"
uvisor_config:
 .long 0x2FE539A6
 .long 0
 .long __uvisor_mode
 .long __uvisor_cfgtbl_start
 .long __uvisor_cfgtbl_end
__uvisor_mode:
 .long 0
 .align 5
.section .bss.uvisor,"awM",@nobits
 .align 5
 .space 0x1000
 .align 5
