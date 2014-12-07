.globl uvisor_init
.globl uvisor_config
.type uvisor_init, %function
.weak __uvisor_mode
.section .uvisor.main, "x"
 .thumb
 .thumb_func
uvisor_init:
 .align 5
 .incbin "uvisor-MK64FN1M0XXX12.box"
uvisor_config:
 .long 0x2FE539A6
 .long 0
 .long __uvisor_mode
 .long __uvisor_cfgtbl_start
 .long __uvisor_cfgtbl_end
 .long __uvisor_data_src
 .long __uvisor_data_start
 .long __uvisor_data_end
 .long __uvisor_secure_start
 .long __uvisor_secure_end
__uvisor_mode:
 .long 0
 .align 5
.section .uvisor.bss.main,"awM",@nobits
 .align 5
__uvisor_stack_start:
 .space 0x1000
 .align 5
__uvisor_stack_end:
