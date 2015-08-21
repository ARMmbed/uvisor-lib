/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __MAIN_ACL_H__
#define __MAIN_ACL_H__

#if   defined(TARGET_LIKE_FRDM_K64F_GCC)

#define MAIN_ACL(acl_list_name) \
    static const UvisorBoxAclItem acl_list_name[] = {      \
        {MCG,   sizeof(*MCG),   UVISOR_TACLDEF_PERIPH}, \
        {UART0, sizeof(*UART0), UVISOR_TACLDEF_PERIPH}, \
        {PIT,   sizeof(*PIT),   UVISOR_TACLDEF_PERIPH}, \
        {SIM,   sizeof(*SIM),   UVISOR_TACLDEF_PERIPH}, \
        {PORTB, sizeof(*PORTB), UVISOR_TACLDEF_PERIPH}, \
    }

#elif defined(TARGET_LIKE_STM32F429I_DISCO_GCC)

#define MAIN_ACL(acl_list_name) \
    static const UvisorBoxAclItem acl_list_name[] = {      \
    }

#else

#define MAIN_ACL(acl_list_name) \
    static const UvisorBoxAclItem acl_list_name[] = {}

#endif

#endif/*__MAIN_ACL_H__*/
