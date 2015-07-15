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
