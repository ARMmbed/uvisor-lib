###########################################################################
#
#  Copyright (c) 2016-2017, Nuvoton, All Rights Reserved
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

if(TARGET_LIKE_NUC472 OR TARGET_LIKE_NUC442)
    # Nuvoton NUC472/442 series
    set(UVISOR_FAMILY "nuc472")
    set(UVISOR_CONFIGURATION "configuration_nuc472_m4_0x20000000_0x0")
elseif(TARGET_LIKE_M451 OR TARGET_LIKE_M452 OR TARGET_LIKE_M453)
    # Nuvoton M451 series
    set(UVISOR_FAMILY "m451")
    set(UVISOR_CONFIGURATION "configuration_m451_m4_0x20000000_0x0")
endif()
