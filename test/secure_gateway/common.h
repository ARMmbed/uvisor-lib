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

#define TEST_ARG1 (uint32_t) 0x10
#define TEST_ARG2 (uint32_t) 0x20
#define TEST_ARG3 (uint32_t) 0x30
#define TEST_ARG4 (uint32_t) 0x40

#define TEST_RES0 (uint32_t) 42
#define TEST_RES1 (uint32_t) (TEST_ARG1)
#define TEST_RES2 (uint32_t) (TEST_ARG1 + TEST_ARG2)
#define TEST_RES3 (uint32_t) (TEST_ARG1 + TEST_ARG2 + TEST_ARG3)
#define TEST_RES4 (uint32_t) (TEST_ARG1 + TEST_ARG2 + TEST_ARG3 + TEST_ARG4)

UVISOR_EXTERN uint32_t __test_function(uint32_t a1, uint32_t a2,
                                       uint32_t a3, uint32_t a4);

#endif/*__COMMON_H__*/
