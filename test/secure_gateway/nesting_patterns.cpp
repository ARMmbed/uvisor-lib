/***************************************************************
 * This confidential and  proprietary  software may be used only
 * as authorised  by  a licensing  agreement  from  ARM  Limited
 *
 *             (C) COPYRIGHT 2013-2015 ARM Limited
 *                      ALL RIGHTS RESERVED
 *
 *  The entire notice above must be reproduced on all authorised
 *  copies and copies  may only be made to the  extent permitted
 *  by a licensing agreement from ARM Limited.
 *
 ***************************************************************/
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
const int g_box_data[][UVISOR_SVC_CONTEXT_MAX_DEPTH] = {
    {1, 3, 4, 2},
    {1, 3, 4, 3, 2},
    {1, 2},
    {4, 3, 4, 3},
};

/* number of patterns to be tested */
const int g_nest_data_num = (int) (sizeof(g_nest_data) / sizeof(g_nest_data[0]));
