#include "androidCmdHandle_thread.h"
#include "user_def.h"
#include <board.h>
#include <rtdevice.h>
#include "pthread.h"

#define DEBUG(...)  rt_kprintf(__VA_ARGS__)
static rt_device_t uart_dev;
ThreadDef_Init(AndroidCmdHandlel_class);

static uint16_t crc16_calculate(const uint8_t *data, uint16_t length);
static int stream_data_handle(unsigned char *pdata);
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

	uart_dev = (rt_device_t)rt_device_find("uart1");
	if(uart_dev == NULL){
		rt_kprintf("uart2 run failed! can't find %s device!\n", "uart1");
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
	unsigned char buffer[20];
	unsigned int length;
	int ret;
	for(;;){
		length = rt_device_read(uart_dev, 0, buffer, sizeof(buffer));
		if(length>0){
			DEBUG("usart1 rx data length is %d data : %x %x\n", length, buffer[0], buffer[1]);
			ret = stream_data_handle(buffer);
			if(ret<0){
				DEBUG("stream_data_handle return error %d\n", ret);
			}			
		}
		else{
			msleep(10);
		}
	}
}

static void start(void *arg)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param sched={sched_get_priority_max(SCHED_FIFO)-1};
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sched);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	open_uart();
	pthread_create(&tid, NULL, run, arg);
}

static void send_cmd_ack(unsigned char cmd)
{
	unsigned char data[]={0x7e, DIRDIRECTION_CTS, CHANNEL_ONE, 0x01, 0x02,cmd,0x03,0x00,0x00,0x7e};
	uint16_t crc = crc16_calculate(data+1, sizeof(data)-4);
	data[sizeof(data)-3] = crc>>8;
	data[sizeof(data)-2] = crc&0xff;
}

static void send_result_ack(unsigned char cmd, unsigned char result)
{
	unsigned char data[]={0x7e, DIRDIRECTION_CTS, CHANNEL_ONE, 0x01, 0x02,cmd,result,0x00,0x00,0x7e};
	uint16_t crc = crc16_calculate(data+1, sizeof(data)-4);
	data[sizeof(data)-3] = crc>>8;
	data[sizeof(data)-2] = crc&0xff;
}
static void send_heart_once(void)
{
	unsigned char data[]={0x7e, DIRDIRECTION_CTS, CHANNEL_ONE, 0x01, 0xff,0,0,0x7e};
	uint16_t crc = crc16_calculate(data, sizeof(data)-4);
	data[sizeof(data)-3] = crc>>8;
	data[sizeof(data)-2] = crc&0xff;
}

static void send_out_water()
{
	unsigned char cmd = SENDOUTWATER_CMD;
	unsigned int ret=-1;
	DEBUG("send out water\n");
	send_cmd_ack(SENDOUTWATER_CMD);
	if(*(status_enum_tydef *)WaterControl_class.pParameter == run){
		send_result_ack(cmd, 0x03);
	}
	else{
		ret = rt_ringbuffer_put(ringbuffer_watercontrol, &cmd, sizeof(cmd));
		if(ret = sizeof(cmd)){
			send_result_ack(STOPOUTWATER_CMD, 0x02);
		}
		else{
			send_result_ack(STOPOUTWATER_CMD, 0x01):
		}		
	}
}

static void stop_out_water()
{
	unsigned char cmd = STOPOUTWATER_CMD;
	unsigned int ret=-1;
	DEBUG("stop out water\n");
	send_cmd_ack(STOPOUTWATER_CMD);
	
	if(*(status_enum_tydef *)WaterControl_class.pParameter == stop){
		send_result_ack(STOPOUTWATER_CMD, 0x02);
	}
	else{
		ret = rt_ringbuffer_put(ringbuffer_watercontrol, &cmd, sizeof(cmd));
		if(ret = sizeof(cmd)){
			send_result_ack(STOPOUTWATER_CMD, 0x03);
		}
		else{
			send_result_ack(STOPOUTWATER_CMD, 0x01):
		}		
	}

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
static int data_hanlde(unsigned char *pdata)
{
	DEBUG("first data is %x\n", *pdata);
	switch(&pdata){
		case STOPOUTWATER_CMD:{stop_out_water();}break;
	}
}
/*


*/
static int stream_data_handle(unsigned char *pdata)
{
	int ret = NO_ERR;
	unsigned char pck_length = 0;
	unsigned char data_length = 0;
	uint16_t crc_value;
	unsigned char length = *(pdata + 2);
	if (*pdata != DIRDIRECTION_CTS) {
		ret = ERR_DIRDIRECTION;
		goto end;
	}
	if ((*(pdata + 2) == CHANNEL_ONE) ||
	        (*(pdata + 2) == CHANNEL_ONE) ||
	        (*(pdata + 2) == CHANNEL_ONE)) {

		switch (length) {
		case CHANNEL_ONE: {
			data_length = *(pdata + 3);
		} break;
		case CHANNEL_TWO: {
			data_length = *(pdata + 3) << 8 + *(pdata + 4);
		} break;
		case CHANNEL_FOUR: {
			data_length = *(pdata + 3) << 8 + *(pdata + 4) + *(pdata + 5) + *(pdata + 6);
		} break;
		default: break;
		}

	}
	else {
		ret = ERR_CHANNEL;
		goto end;
	}
	pck_length = 1 + 1 + data_length+length;
	crc_value = crc16_calculate(pdata, pck_length);
	if(crc_value != *(uint16_t *)(pdata + pck_length-2))
		ret = ERR_CRC;
	else{
		data_hanlde(pdata+1 +1+1+data_length);

	}
end:
	return ret;
}






