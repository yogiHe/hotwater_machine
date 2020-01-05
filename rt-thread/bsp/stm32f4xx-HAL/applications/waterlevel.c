#include "waterlevel.h"
#include "stdbool.h" 
#include "user_def.h"
#include "pthread.h"
#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <pthread.h>

ThreadDef_Init(WaterLevel_class);

extern int L_LEVEL;
extern int H_LEVEL;

static void init(void);
static void open_valva(void);
static void close_valva(void);
static pthread_t tid;

static void *run(void *arg)
{
	msleep(1000);
	rt_pin_write(oe1_pin, PIN_HIGH);
	for(;;){
		if(L_LEVEL>0xE00){
			/*begin let cold water come in*/
			rt_kprintf("water level is lower than 45%\n");
			open_valva();
		}
		if(H_LEVEL<0xE00){
			rt_kprintf("water level is full\n");
			close_valva();
		}
		msleep(1000);
	}
}

static void start(void *arg)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param sched={sched_get_priority_max(SCHED_FIFO)-3};
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sched);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	init();
//	pthread_barrier_init
	pthread_create(&tid, &attr, run, arg);
}

static void init(void)
{
//	rt_pin_mode(LOW_LEVEL_PIN, PIN_MODE_INPUT);
//	rt_pin_mode(HIGH_LEVEL_PIN, PIN_MODE_INPUT);
	rt_pin_mode(VALVE_RCOLD_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(PULLUP_LEVEL_PIN, PIN_MODE_OUTPUT);
	rt_pin_write(PULLUP_LEVEL_PIN, PIN_HIGH);
	rt_pin_write(VALVE_RCOLD_PIN, PIN_LOW);
	rt_pin_mode(oe1_pin, PIN_MODE_OUTPUT);
	rt_pin_write(oe1_pin, PIN_LOW);
	
	/*
	rt_pin_mode(10, PIN_MODE_OUTPUT);
	rt_pin_mode(20, PIN_MODE_OUTPUT);
	rt_pin_mode(21, PIN_MODE_OUTPUT);
	rt_pin_mode(22, PIN_MODE_OUTPUT);
	rt_pin_write(10, PIN_LOW);
	rt_pin_write(20, PIN_LOW);
	rt_pin_write(21, PIN_LOW);
	rt_pin_write(22, PIN_LOW);
	*/
	
}

static void open_valva(void)
{
	rt_pin_write(VALVE_RCOLD_PIN, PIN_HIGH);
}

static void close_valva()
{
	rt_pin_write(VALVE_RCOLD_PIN, PIN_LOW);
}

