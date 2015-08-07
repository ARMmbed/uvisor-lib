/***************************************************************
 * This confidential and  proprietary  software may be used only
 * as authorised  by  a licensing  agreement  from  ARM  Limited
 *
 *             (C) COPYRIGHT 2013-2015 ARM Limited
 *                      ALL RIGHTS RESERVED
 *
 *  The entire notice above must be reproduced on all authorised
 *  copies and copies  may only be made to the  extent permitted
 *  by a licensing agreement from ARM Limited.
 *
 ***************************************************************
 *
 *  DO NOT CHANGE - this file has been autogenerated from:
 *
 *  https://github.com/ARMmbed/uvisor-private
 *    - k64f/uvisor/lib/uvisor-gcc-header.S
 *    - k64f/uvisor/lib/uvisor-gcc-input.S
 *
 ***************************************************************/
.globl uvisor_init
.globl uvisor_config
.globl uvisor_ctx
.type uvisor_init, %function
.weak __uvisor_mode
.section .uvisor.main, "x"
    .thumb
    .thumb_func
uvisor_init:
    .align 5
    .incbin "uvisor-STM32F429xx.box"
uvisor_config:
    .long 0x2FE539A6
    .long 0
    .long __uvisor_mode
    .long __uvisor_cfgtbl_start
    .long __uvisor_cfgtbl_end
    .long __uvisor_data_src
    .long __uvisor_data_start
    .long __uvisor_data_end
    .long __uvisor_bss_start
    .long __uvisor_bss_end
    .long __uvisor_secure_start
    .long __uvisor_secure_end
    .long __uvisor_reserved_start
    .long __uvisor_reserved_end
    .long __uvisor_box_context
__uvisor_mode:
    .long 0
    .align 5
.section .bss
__uvisor_box_context:
uvisor_ctx:
    .long 0
.section .uvisor.bss.main,"awM",@nobits
__uvisor_reserved_start:
    .space 0x2000
__uvisor_reserved_end:
