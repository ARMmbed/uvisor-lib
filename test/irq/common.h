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
#ifndef __COMMON_H__
#define __COMMON_H__

#define TEST1_IRQn 42
#define TEST1_PRIO 2
#define TEST1_VAL  1

#define TEST2_IRQn 43
#define TEST2_PRIO 1
#define TEST2_VAL  2

#define DFLT_PRIO 1

extern volatile int g_flag;

#endif/*__COMMON_H__*/
