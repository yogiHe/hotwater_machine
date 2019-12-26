#ifndef __WATEROUT_CONTROL_H
#define __WATEROUT_CONTROL_H

#include <rtdevice.h>
#include "user_def.h"
#include <drv_gpio.h>

#define VALVA_GPIO_GROUP 		C
#define VALVA_GPIO_PIN 			1
#define VALVA_PIN  				GET_PIN(VALVA_GPIO_GROUP, VALVA_GPIO_PIN)

#define MOTOR_GPIO_GROUP	C
#define MOTOR_GPIO_PIN		1
#define MOTOR_PIN			GET_PIN(MOTOR_GPIO_GROUP, MOTOR_GPIO_PIN)


extern struct rt_ringbuffer* ringbuffer_watercontrol;
extern ThreadStruct_Tydef WaterOut_class;
#endif