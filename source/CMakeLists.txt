################################################################
#
#  This confidential and  proprietary  software may be used only
#  as authorised  by  a licensing  agreement  from  ARM  Limited
#
#             (C) COPYRIGHT 2013-2015 ARM Limited
#                    ALL RIGHTS RESERVED
#
#   The entire notice above must be reproduced on all authorised
#   copies and copies  may only be made to the  extent permitted
#   by a licensing agreement from ARM Limited.
#
################################################################

# select correct source for target/toolchain
if    (TARGET_LIKE_STM32F429I_DISCO_GCC)
    set(UVISOR_DIR "STM32F429xx")           # STM32,     GCC_ARM
elseif(TARGET_LIKE_FRDM_K64F_GCC)
    set(UVISOR_DIR "MK64FN1M0XXX12")        # FRDM-K64F, GCC_ARM
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# instruct cmake to compile the .s file
set(UVISOR_ASM "${UVISOR_DIR}/uvisor-GCC_ARM.s")
enable_language(ASM)
set_property(SOURCE ${UVISOR_ASM} PROPERTY LANGUAGE ASM)

# library
add_library(uvisor-lib
    ${UVISOR_ASM}
    "bitband.cpp"
    "interrupts.cpp"
    "benchmark.cpp"
)

# the INCBIN directive in the .s file looks for the target bin in its own folder
include_directories(${UVISOR_DIR}/)
