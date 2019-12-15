#include "androidCmdHandle_thread.h"
#include "user_def.h"
#include <board.h>
#include <rtdevice.h>
#include "pthread.h"

static rt_device_t uart_dev;
ThreadDef_Init(AndroidCmdHandlel_class);

static uint16_t crc16_calculate(const uint8_t *data, uint16_t length);
/**/
static void start(void *arg);
static void *run(void *arg);
/**/
static pthread_t tid;

static void open_uart()
{
	struct serial_configure cfg={
		.baud_rate = 115200,
		.data_bits = DATA_BITS_8,
		.stop_bits = UART_STOPBITS_1,
		.parity = PARITY_NONE,

	};

	uart_dev = (rt_device_t)rt_device_find("uart2");
	if(uart_dev == NULL){
		rt_kprintf("uart2 run failed! can't find %s device!\n", "uart2");
	}
//	res = rt_device_set_rx_indicate(uart_device, uart_intput);
	int res = rt_device_open(uart_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
//	rt_event_init(&even t, "event", RT_IPC_FLAG_FIFO);

	((rt_serial_t *)uart_dev)->ops->configure((rt_serial_t *)uart_dev , &cfg);
	((rt_serial_t *)uart_dev)->ops->control((rt_serial_t *)uart_dev ,RT_DEVICE_CTRL_SET_INT, NULL);
}
static void init()
{

}



static void *run(void *arg)
{
	for(;;){
	
	}
}

static void start(void *arg)
{
	pthread_create(&tid, NULL, run, arg);
}


static void send_heart_once(void)
{
	unsigned char data[]={0x7e, DIRDIRECTION_STC, CHANNEL_ONE, 0x01, 0xff,0,0,0x7e};
	uint16_t crc = crc16_calculate(data, sizeof(data)-2);
	data[sizeof(data)-2] = crc>>8;
	data[sizeof(data)-1] = crc&0xff;
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






