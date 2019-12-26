#ifndef __WATERCONTROL_THREAD_H
#define __WATERCONTROL_THREAD_H

#include <rtdevice.h>
#include <drv_gpio.h>

#define HEATING_GPIO_GROUP	C
#define HEATING_GPIO_PIN	1
#define HEATING_PIN			GET_PIN(HEATING_GPIO_GROUP, HEATING_GPIO_PIN)



static void hot_water_control(int timeout);




#endif