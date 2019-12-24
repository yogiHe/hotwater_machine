#include "WaterControl_thread.h"
#include "stdbool.h" 
#include "user_def.h"
#include "pthread.h"
#include "user_def.h"
#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <pthread.h>

#define RINGBUFFER_SIZE 4


ThreadDef_Init(WaterControl_class);
rt_adc_device_t adc_dev;
static const int adc_channle = 8;
static pthread_t tid;
static int init(void);
static float get_water_temp(void);
static status_enum_tydef status=stop;
struct rt_ringbuffer* ringbuffer_watercontrol;

extern ADC_HandleTypeDef hadc1;
static void *run(void *arg)
{
	unsigned char data;
	unsigned int size;
//	config();
	for(;;){
		msleep(5000);
		size = rt_ringbuffer_get(ringbuffer_watercontrol, &data, 1);
		if(size == 1){
			switch(data){
				case 0x03:

				break;
				case 0x01:break;
			}
		}
//		HAL_ADC_Start(&hadc1);
		get_water_temp();
	}
}

static void start(void *arg)
{
	init();
	pthread_create(&tid, NULL, run, arg);
}
/*
*@brief This function control water pump for draw out
 the hot water
* @param  timeout:time out
* @retval None

*/
static void hot_water_control(int timeout)
{
	

}
/*
*@brief This function control electromagnetic valve for
 cold water can comes out automatically
* @param  timeout:time out
* @retval None
*/
static void cold_water_control(int timeout)
{

}

/*
*@brief This function control the 
*/
static void water_heating(bool flag)
{

}
/*
*@brief This function is thread handle
* @param  parameter:not use
* @retval None:never return
*/
void waterControl_thread_handle(void *parameter)
{


}
static gpio_init(void)
{
	rt_pin_mode(GET_PIN(), PIN_MODE_OUTPUT);
	rt_pin_write(GET_PIN,PIN_HIGH);
}
static int init(void)
{
	status=stop;
	WaterControl_class.pParameter = &status;
	ringbuffer_watercontrol = rt_ringbuffer_create();
	gpio_init();
//	unsigned int ADC_DEV_CHANNEL=4;
//	int value;
	adc_dev = (rt_adc_device_t)rt_device_find("adc1");
	if(adc_dev == NULL){
		rt_kprintf("adc sample run failed! can't find %s device!\n", "adc1");
		return RT_ERROR;
	}
//	rt_device_open(adc_dev, RT_DEVICE_FLAG_RDWR);
//	rt_device_control(adc_dev, RT_ADC_CMD_ENABLE, (void*)ADC_DEV_CHANNEL);
//	rt_device_read(adc_dev, ADC_DEV_CHANNEL, &value, 4);
	rt_adc_enable(adc_dev, adc_channle);
	int value = rt_adc_read(adc_dev, adc_channle);
	rt_kprintf("the value is :%d \n", value);
	int vol = value * 330 / 4096;
	rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);
}

static float get_water_temp(void)
{
//	int value;unsigned int ADC_DEV_CHANNEL=4;
//	int ret=rt_device_read(adc_dev, ADC_DEV_CHANNEL, &value, 4);
	int value = rt_adc_read(adc_dev, adc_channle);
	rt_kprintf("the value is :%d  \n", value);
	int vol = value * 330 / 4096;
	rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);
}