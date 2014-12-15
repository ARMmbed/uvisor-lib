################################################################
#
#  This confidential and  proprietary  software may be used only
#  as authorised  by  a licensing  agreement  from  ARM  Limited
#
#             (C) COPYRIGHT 2013-2014 ARM Limited
#                    ALL RIGHTS RESERVED
#
#   The entire notice above must be reproduced on all authorised
#   copies and copies  may only be made to the  extent permitted
#   by a licensing agreement from ARM Limited.
#
################################################################

# select correct source for target/toolchain
set(UVISOR_DIR "MK64FN1M0XXX12")		# FRDM-K64F, GCC_ARM

# instruct cmake to compile the .s file
set(UVISOR_ASM "${UVISOR_DIR}/uvisor-GCC_ARM.s")
enable_language(ASM)
set_property(SOURCE ${UVISOR_ASM} PROPERTY LANGUAGE ASM)
add_library(uvisor-lib
    ${UVISOR_ASM}
)

# the INCBIN directive in the .s file looks 
# for the target bin in its own folder
include_directories(${UVISOR_DIR}/)
