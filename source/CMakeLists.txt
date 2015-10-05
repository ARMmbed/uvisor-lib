###########################################################################
#
#  Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
#  SPDX-License-Identifier: Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the "License"); you may
#  not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
###########################################################################

# select correct source for target/toolchain
set(UVISOR_PRESENT false)
if     (TARGET_LIKE_FRDM_K64F)
    if(TARGET_LIKE_GCC)
        set(UVISOR_PRESENT true)
        set(UVISOR_DIR "MK64FN1M0XXX12")        # FRDM-K64F
    endif()
elseif(TARGET_LIKE_STM32F429I_DISCO)
    if(TARGET_LIKE_GCC)
        set(UVISOR_PRESENT true)
        set(UVISOR_DIR "STM32F429xx")           # STM32F429I
    endif()
endif()

# select source files depending on platform (supported/unsupported)
if(UVISOR_PRESENT)
    add_library(uvisor-lib
        "${UVISOR_DIR}/uvisor.o"
        "error.cpp"
        "interrupts.cpp"
        "benchmark.cpp"
    )
else()
    message(WARNING
"\n         *******************************************************************
         * WARNING (uvisor-lib): unsupported platform; your code will still
         *                       work but no security feature is provided;
         *                       UVISOR_DISABLED is set by default
         *******************************************************************")
    add_library(uvisor-lib
        "unsupported.cpp"
    )
endif()
