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

# Detect yotta build mode.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(UVISOR_MODE "debug")
else()
    set(UVISOR_MODE "release")
endif()

# Determine the uVisor binary based on the target.
# Note: We currently only support the GCC toolchain.
set(UVISOR_SUPPORTED false)
if(TARGET_LIKE_GCC)
    # Pick the correct folder based on the target family.
    if(TARGET_LIKE_KINETIS)
        set(UVISOR_FAMILY "kinetis")
    elseif(TARGET_LIKE_STM32)
        set(UVISOR_FAMILY "stm32")
    elseif(TARGET_LIKE_EFM32)
        set(UVISOR_FAMILY "efm32")
    endif()

    # Infer path to binary.
    # The included file defines UVISOR_CONFIGURATION.
    include(${UVISOR_FAMILY}/get_configuration.cmake)
    if(NOT ("${UVISOR_CONFIGURATION}" STREQUAL ""))
        set(UVISOR_SUPPORTED true)
        set(UVISOR_BINARY "${UVISOR_FAMILY}/${UVISOR_MODE}/${UVISOR_CONFIGURATION}.o")
    endif()
endif()

# Build all files if uVisor is supported, otherwise throw a warning and only
# build unsupported.cpp.
if(UVISOR_SUPPORTED)
    # uvisor-lib source files
    file(GLOB UVISOR_SRC "*.cpp")

    add_library(uvisor-lib
        ${UVISOR_BINARY}
        ${UVISOR_SRC}
    )
else()
    message(WARNING
"\n         *********************************************************************
         * WARNING (uvisor-lib): unsupported platform; your code will still  *
         *                       work but no security feature is provided;   *
         *                       UVISOR_DISABLED is set by default           *
         *********************************************************************")
    add_library(uvisor-lib
        "unsupported.cpp"
    )

    # check that the yotta config option has not been mistakenly changed
    if((DEFINED YOTTA_CFG_UVISOR_PRESENT) AND ("${YOTTA_CFG_UVISOR_PRESENT}" STREQUAL "1"))
        message(FATAL_ERROR
"\n         *********************************************************************
         * ERROR   (uvisor-lib): this is an unsupported platform, but        *
         *                             yotta config \"uvisor.present\"         *
         *                       is set to 1; please contact the target      *
         *                       owner to set it to 0 while uVisor is not    *
         *                       officially supported.                       *
         *                       You can apply the config option as a        *
         *                       temporary fix in your executable:           *
         *                                                                   *
         *                yotta --config='{\"uvisor\":{\"present\": 0}}' build   *
         *                                                                   *
         *********************************************************************")
    endif()
endif()
