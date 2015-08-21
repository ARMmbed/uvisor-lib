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
#include "mbed/mbed.h"
#include "uvisor-lib/uvisor-lib.h"
#include "nesting_patterns.h"

/* number of nested calls for each pattern provided in g_box_data */
const int g_nest_data[] = {
    3,
    4,
    1,
    3,
};

/* nesting call patterns: each number identifies a box id; if a line reads
 *     1 3 2 4 2 3
 * it means there are 5 levels of nesting (hence the corresponding g_nest_data
 * element will be 5) and the topmost function called is in box 1, then 3, 2,
 * etc.; each line is a different pattern */
const int g_box_data[][NESTING_MAX_DEPTH] = {
    {1, 3, 4, 2},
    {1, 3, 4, 3, 2},
    {1, 2},
    {4, 3, 4, 3},
};

/* number of patterns to be tested */
const int g_nest_data_num = (int) (sizeof(g_nest_data) / sizeof(g_nest_data[0]));
