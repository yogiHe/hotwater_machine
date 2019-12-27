#include "WaterControl_thread.h"
#include "stdbool.h" 
#include "user_def.h"
#include "pthread.h"
#include "user_def.h"
#include <board.h>
#include <rtthread.h>
#include <pthread.h>

#define RINGBUFFER_SIZE 4


ThreadDef_Init(WaterControl_class);
rt_adc_device_t adc_dev;
static const int adc_channle = 8;
static pthread_t tid;
static int init(void);
static double get_water_temp(void);


static void water_heating(bool flag);
extern ADC_HandleTypeDef hadc1;

static void *run(void *arg)
{
	unsigned char data;
	unsigned int size;
	float Voltage;
	for(;;){
		msleep(1000);
		Voltage = get_water_temp();
		if(Voltage < 0.1){
			rt_kprintf("water temp is lower than 90\n");
			water_heating(true);
		}
		else{
			water_heating(false);
		}
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
	if(flag == true)
		rt_pin_write(HEATING_PIN, PIN_HIGH);
	else
		rt_pin_write(HEATING_PIN, PIN_LOW);
}

/*
*@brief This function is thread handle
* @param  parameter:not use
* @retval None:never return
*/
void waterControl_thread_handle(void *parameter)
{


}

static void gpio_init(void)
{
	rt_pin_mode(HEATING_PIN, PIN_MODE_OUTPUT);
	rt_pin_write(HEATING_PIN, PIN_LOW);
}
static int init(void)
{

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

static double get_water_temp(void)
{
//	int value;unsigned int ADC_DEV_CHANNEL=4;
//	int ret=rt_device_read(adc_dev, ADC_DEV_CHANNEL, &value, 4);
	int value = rt_adc_read(adc_dev, adc_channle);
	rt_kprintf("the value is :%d  \n", value);
	double vol = value * 3.3 / 4096;
	rt_kprintf("the voltage is :%d.%d \n", (int)vol%100, ((int)(vol *1000))%1000);
	return vol;
}