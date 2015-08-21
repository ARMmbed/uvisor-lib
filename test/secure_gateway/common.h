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
#ifndef __COMMON_H__
#define __COMMON_H__

#define TEST_ARG1 (uint32_t) 0x10
#define TEST_ARG2 (uint32_t) 0x20
#define TEST_ARG3 (uint32_t) 0x30
#define TEST_ARG4 (uint32_t) 0x40

#define TEST_RES0 (uint32_t) 42
#define TEST_RES1 (uint32_t) (TEST_ARG1)
#define TEST_RES2 (uint32_t) (TEST_ARG1 + TEST_ARG2)
#define TEST_RES3 (uint32_t) (TEST_ARG1 + TEST_ARG2 + TEST_ARG3)
#define TEST_RES4 (uint32_t) (TEST_ARG1 + TEST_ARG2 + TEST_ARG3 + TEST_ARG4)

#define TEST_ERR  (uint32_t) 0xFFFFFFFF

UVISOR_EXTERN uint32_t __test_function0(void);
UVISOR_EXTERN uint32_t __test_function1(uint32_t a1);
UVISOR_EXTERN uint32_t __test_function2(uint32_t a1, uint32_t a2);
UVISOR_EXTERN uint32_t __test_function3(uint32_t a1, uint32_t a2, uint32_t a3);
UVISOR_EXTERN uint32_t __test_function4(uint32_t a1, uint32_t a2, uint32_t a3,
                                        uint32_t a4);

#endif/*__COMMON_H__*/
