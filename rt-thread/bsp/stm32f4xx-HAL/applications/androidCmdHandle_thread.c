#include "androidCmdHandle_thread.c"
#include "user_def.h"
ThreadStruct(AndroidCmdHandle_thread);

/**/
static void start(void *arg);
static void *run(void *arg);
/**/
static pthread_t tid;

struct AndroidCmdHandle_thread_class={
	start;
};

static void *run(void *arg)
{
	for(;;){
	
	}
}

static void start(void *arg)
{
	pthread_create(&tid, NULL, run, arg);
}



struct AndroidCmdHandle_thread_Class
{
	
};

static void send_heart_once(void)
{

}

static void send_out_water()
{


}

static void stop_out_water()
{

}

static void calibration()
{

}

static void err_alarm()
{


}

static void set_temp()
{

}

static void set_time()
{

}

static void set_poweroff_time()
{

}

static void get_dev_info()
{

}
static uint16_t crc16_calculate(const uint8_t *data, uint16_t length)
{
	uint16_t crc, i;
	crc = 0;
	while (length--)
	{
		crc = crc ^ (uint16_t) * data++ << 8;
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x8000)
				crc = crc << 1 ^ 0x1021;
			else
				crc = crc << 1;
		}
	}
	return (crc & 0xFFFF);
}
/*


*/
static int stream_data_handle(unsigned char *pdata)
{
	int ret = NO_ERR;
	unsigned char pck_length = 0;
	unsigned char data_length = 0;
	uint16_t crc_value;
	if (*pdata != DIRDIRECTION_CTS) {
		ret = ERR_DIRDIRECTION;
		goto end;
	}
	if ((*(pdata + 1) == CHANNEL_ONE) ||
	        (*(pdata + 1) == CHANNEL_ONE) ||
	        (*(pdata + 1) == CHANNEL_ONE)) {

		unsigned char length = *(pdata + 1);
		switch (length) {
		case CHANNEL_ONE: {
			data_length = *(pdata + 2) + length;
		} break;
		case CHANNEL_TWO: {
			data_length = *(pdata + 2) << 8 + *(pdata + 3) + length;
		} break;
		case CHANNEL_FOUR: {
			data_length = *(pdata + 2) << 8 + *(pdata + 3) + *(pdata + 4) + *(pdata + 5) + length;
		} break;
		default: break;
		}

	}
	else {
		ret = ERR_CHANNEL;
		goto end;
	}
	pck_length = 1 + 1 + data_length;
	crc_value = crc16_calculate(pdata, pck_length);
	if(crc_value != *(uint16_t *)(pdata + pck_length-2))
		ret = ERR_CRC;
end:
	return ret;
}






