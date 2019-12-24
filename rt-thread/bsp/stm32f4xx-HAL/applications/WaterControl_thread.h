#ifndef __WATERCONTROL_THREAD_H
#define __WATERCONTROL_THREAD_H

typedef enum status_control
{
	stop=0,
	run,
}status_enum_tydef;
extern struct rt_ringbuffer* ringbuffer_watercontrol;
static void hot_water_control(int timeout);

#endif