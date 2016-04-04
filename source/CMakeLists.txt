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

# uVisor paths
set(UVISOR_ROOT "${CMAKE_CURRENT_LIST_DIR}/../uvisor")
set(UVISOR_API_LIB "${UVISOR_ROOT}/api/lib")
set(UVISOR_API_SRC "${UVISOR_ROOT}/api/src")

# Detect the yotta build mode.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(UVISOR_MODE "debug")
else()
    set(UVISOR_MODE "release")
endif()

# Determine the uVisor configuration based on the target.
# The included files define UVISOR_CONFIGURATION, if any.
file(GLOB UVISOR_LIB_SUPPORT_FILES "${CMAKE_CURRENT_LIST_DIR}/support_*.cmake")
foreach(SUPPORT_FILE ${UVISOR_LIB_SUPPORT_FILES})
    include("${SUPPORT_FILE}")
endforeach(SUPPORT_FILE)

# Determine UVISOR_SUPPORTED.
# Even if a target claims that uVisor is supported (YOTTA_CFG_UVISOR_PRESENT),
# we still need to check that the uVisor code-base actually supports the target
# family.
# Note: Currently we only support targets built with GCC.
if(YOTTA_CFG_UVISOR_PRESENT AND UVISOR_CONFIGURATION AND TARGET_LIKE_GCC)
    set(UVISOR_SUPPORTED true)
else()
    set(UVISOR_SUPPORTED false)
endif()

if(UVISOR_SUPPORTED)
    add_library(uvisor-lib)
    target_link_libraries(uvisor-lib ${UVISOR_API_LIB}/${UVISOR_FAMILY}/${UVISOR_MODE}/${UVISOR_CONFIGURATION}.a)
else()
    # Build unsupported.c
    add_library(uvisor-lib ${UVISOR_API_SRC}/unsupported.c)

    message(WARNING
"\n         *********************************************************************
         * WARNING (uvisor-lib): unsupported platform; your code will still  *
         *                       work but no security feature is provided;   *
         *                       UVISOR_DISABLED is set by default           *
         *********************************************************************")

    # Check that the yotta config option has not been mistakenly changed.
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

# This is needed as this library does not specify any source file when the
# target supports uVisor.
set_target_properties(uvisor-lib PROPERTIES LINKER_LANGUAGE C)
