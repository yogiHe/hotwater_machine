#include "WaterControl_thread.h"
#include "stdbool.h" 
#include "user_def.h"

ThreadStruct(ThreadStructName_def(WaterControl_THREAD));
ThreadDef_Init(ThreadStructName_def(WaterControl_THREAD), WaterControl_class);

static pthread_t tid;
static void *run(void *arg)
{
	for(;;){
	
	}
}

static void start(void *arg)
{
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