#include "waterlevel.h"
#include "stdbool.h" 
#include "user_def.h"
#include "pthread.h"
#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <pthread.h>

ThreadDef_Init(WaterLevel_class);


static void init(void);
static void open_valva(void);
static void close_valva(void);
static pthread_t tid;

static void *run(void *arg)
{
	for(;;){
		if(rt_pin_read(LOW_LEVEL_PIN) == PIN_HIGH){
			/*begin let cold water come in*/
			open_valva();
		}
		if(rt_pin_read(VALVE_RCOLD_PIN)==PIN_LOW){
			close_valva();

		}
		msleep(100);
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
	rt_pin_mode(LOW_LEVEL_PIN, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(HIGH_LEVEL_PIN, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(VALVE_RCOLD_PIN, PIN_MODE_OUTPUT);
}

static void open_valva(void)
{
	rt_pin_write(VALVE_RCOLD_PIN, PIN_HIGH);
}

static void close_valva()
{
	rt_pin_write(VALVE_RCOLD_PIN, PIN_LOW);
}

