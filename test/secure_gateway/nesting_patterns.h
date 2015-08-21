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
#ifndef __NESTING_PATTERNS_H__
#define __NESTING_PATTERNS_H__

/* the nesting depth is limited by uvisor if uvisor is supported for a given
 * platform, otherwise it is arbitrary (secure gateways map to simple function
 * calls) */
#ifdef UVISOR_SVC_CONTEXT_MAX_DEPTH
#define NESTING_MAX_DEPTH UVISOR_SVC_CONTEXT_MAX_DEPTH
#else
#define NESTING_MAX_DEPTH 0x10
#endif

extern const int g_nest_data[];
extern const int g_box_data[][NESTING_MAX_DEPTH];
extern const int g_nest_data_num;

#endif/*__NESTING_PATTERNS_H__*/
