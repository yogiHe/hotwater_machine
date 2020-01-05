#ifndef __WATEROUT_CONTROL_H
#define __WATEROUT_CONTROL_H

#include <rtdevice.h>
#include "user_def.h"
#include <drv_gpio.h>

#define VALVA_GPIO_GROUP 		A
#define VALVA_GPIO_PIN 			4
#define VALVA_PIN  				20

#define MOTOR_GPIO_GROUP	C
#define MOTOR_GPIO_PIN		2
#define MOTOR_PIN			10


extern struct rt_ringbuffer* ringbuffer_watercontrol;
extern ThreadStruct_Tydef WaterOut_class;
#endif