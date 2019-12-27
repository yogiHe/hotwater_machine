#ifndef __WATERCONTROL_THREAD_H
#define __WATERCONTROL_THREAD_H

#include <rtdevice.h>
#include <drv_gpio.h>

#define HEATING_GPIO_GROUP	A
#define HEATING_GPIO_PIN	6
#define HEATING_PIN			22



static void hot_water_control(int timeout);




#endif