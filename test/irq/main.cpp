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
#include "common.h"

/* main box ACLs */
static const UvisorBoxAclItem g_main_acl[] = {
    {MCG,                  sizeof(*MCG),       UVISOR_TACLDEF_PERIPH},
    {UART0,                sizeof(*UART0),     UVISOR_TACLDEF_PERIPH},
    {PIT,                  sizeof(*PIT),       UVISOR_TACLDEF_PERIPH},
    {SIM,                  sizeof(*SIM),       UVISOR_TACLDEF_PERIPH},
    {PORTB,                sizeof(*PORTB),     UVISOR_TACLDEF_PERIPH},
};

/* enable uvisor */
UVISOR_SET_MODE_ACL(2, g_main_acl);

volatile int g_flag;
