/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>
#include <user_def.h>
#include <watercontrol_thread.h>
#include "user_def.h"

extern ThreadStruct_Tydef WaterControl_class;
extern ThreadStruct_Tydef AndroidCmdHandlel_class;
extern ThreadStruct_Tydef WaterLevel_class;
extern ThreadStruct_Tydef WaterOut_class;
extern ThreadStruct_Tydef gprs_class;
int main(void)
{
    /* user app entry */
	WaterControl_class.start(NULL);
	AndroidCmdHandlel_class.start(NULL);
	WaterLevel_class.start(NULL);
	WaterOut_class.start(NULL);
//	gprs_class.start(NULL);
    return 0;
}
