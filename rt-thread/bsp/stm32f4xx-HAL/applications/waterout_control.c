#include "waterout_control.h"
#include "stdbool.h"
#include "pthread.h"
#include <board.h>
#include <rtthread.h>
#include <pthread.h>


ThreadDef_Init(WaterOut_class);
static void init(void);
static void stop_out_hotwater(void);
static unsigned char data[10];
static unsigned int out_water_cnt;
static unsigned char water_flag;
static unsigned char stop_flag = 0;
static pthread_t tid;

extern struct rt_ringbuffer* ringbuffer_androidTx;
/*冷热比*/
static const unsigned int temp_tab[101] = {
	0xFFFFFFFF, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90,
				89, 88, 87, 86, 85, 84, 83, 82, 81, 80,
				79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
				69, 68, 67, 66, 65, 64, 63, 62, 61, 60,
				59, 58, 57, 56, 55, 54, 53, 52, 51, 50,
				49, 48, 47, 46, 45, 44, 43, 42, 41, 40,
				39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
				29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
				19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
				01, 02, 03, 04, 05, 04, 03, 02, 01, 00
};
struct rt_ringbuffer* ringbuffer_watercontrol;

static void begin_out_hotwater(unsigned int temperature);

static void *run(void *arg)
{
	int size=0;
	for (;;) {
		size = rt_ringbuffer_data_len(ringbuffer_watercontrol);
		if (size > 0) {
			rt_ringbuffer_get(ringbuffer_watercontrol, data, sizeof(data));
			switch (data[0]) {
			case 0x01:
				rt_kprintf("begin water out control %d %d %d %d\n",data[0],data[1],data[2],data[3]);
				if(*((unsigned char *)data+1) == 0x00){
					out_water_cnt = ((data[3]<<8)+data[4]) * 0x10;
					water_flag = data[2];
					if(out_water_cnt>0)stop_flag=1;
					rt_kprintf("ask water out control %d %d\n", out_water_cnt, water_flag);
				}
			break;
			case 0x03:
				out_water_cnt = 0;
				rt_kprintf("stop out water\n");
//				begin_out_hotwater(0);
				break;
			}
		}
		else{
			msleep(100);
		}
		if(out_water_cnt>0){
			out_water_cnt--;
			if(water_flag == 0x01){
				rt_kprintf("out hot water\n");
				begin_out_hotwater(100);
			}
			else{
				rt_kprintf("out cold water\n");
				begin_out_hotwater(0);
			}
			if(out_water_cnt == 0){
				unsigned char done = 1;
				rt_kprintf("out  water  OK\n");
				rt_ringbuffer_put(ringbuffer_androidTx, &done, sizeof(done));
			}
			if(out_water_cnt == 0)
				stop_out_hotwater();
		}
		else{
		 stop_flag=0;
			stop_out_hotwater();
		}
	}
}

static void start(void *arg)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param sched = {sched_get_priority_max(SCHED_FIFO) - 2};
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sched);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	init();
//	pthread_barrier_init
	pthread_create(&tid, &attr, run, arg);
}

static void init(void)
{
	rt_pin_mode(VALVA_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(MOTOR_PIN, PIN_MODE_OUTPUT);
	rt_pin_write(MOTOR_PIN, PIN_LOW);
	rt_pin_write(VALVA_PIN, PIN_LOW);
	rt_pin_write(VALVA_PIN, PIN_HIGH);
	ringbuffer_watercontrol = rt_ringbuffer_create(10);
}
/*
temperature [1:1:100]
*/
static void begin_out_hotwater(unsigned int temperature)
{
	unsigned int 	temp = temp_tab[temperature];
	unsigned int valva_delay = 500 * temp / (temp + 1);
	unsigned int motor_delay = 500 / (temp + 1);
	if(temp == 0xffffffff){
		valva_delay=500;
		motor_delay = 0;
	}
	if (temperature > 100) {
		rt_kprintf("temperature is higher than 100 :%d \n", temperature);
		return;
	}
	rt_kprintf("valva delay is:%d  motor delay is %d tmp %x\n", valva_delay, motor_delay, temp);
	rt_pin_write(VALVA_PIN, PIN_HIGH);
	msleep(valva_delay);
	rt_pin_write(VALVA_PIN, PIN_LOW);
	rt_pin_write(MOTOR_PIN, PIN_HIGH);
	msleep(motor_delay);
	rt_pin_write(MOTOR_PIN, PIN_LOW);

}

static void stop_out_hotwater(void)
{
	rt_pin_write(VALVA_PIN, PIN_LOW);
	rt_pin_write(MOTOR_PIN, PIN_LOW);
}




