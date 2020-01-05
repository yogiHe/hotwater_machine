#include "androidCmdHandle_thread.h"
#include "user_def.h"
#include <board.h>
#include <rtdevice.h>
#include "pthread.h"
#include "waterout_control.h"

#define DEBUG(...)  rt_kprintf(__VA_ARGS__)
static rt_device_t uart_dev;

ThreadDef_Init(AndroidCmdHandlel_class);
static uint16_t crc16_calculate(const uint8_t *data, uint16_t length);
static int stream_data_handle(unsigned char *pdata);
static void send_result_ack(unsigned char cmd, unsigned char result);
struct rt_ringbuffer* ringbuffer_androidTx;
/**/
static pthread_t tid;
static status_enum_tydef status=water_;

static void open_uart()
{
	struct serial_configure cfg={
		.baud_rate = 115200,
		.data_bits = DATA_BITS_8,
		.stop_bits = UART_STOPBITS_1,
		.parity = PARITY_NONE,

	};

	uart_dev = (rt_device_t)rt_device_find("uart6");
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
	unsigned char buffer[40];
	unsigned int length;
	unsigned char cnt=0;
	int ret;
	int test_len;
	for(;;){
		length = rt_device_read(uart_dev, 0, buffer+cnt, sizeof(buffer)-cnt);
		if(length>0){
			cnt = cnt + length;
			if((buffer[0] == 0x7e) && buffer[cnt-1] == 0x7e){
				DEBUG("usart1 rx  all data of package\n");
				test_len =cnt;
				cnt = 0;
			}
			else{
				if(cnt >= sizeof(buffer))
					cnt = 0;
				DEBUG("usart1 rx data not all data of package %d %d %d %d\n",buffer[0], buffer[cnt-1], buffer[cnt-2],cnt);
				continue;
			}
			DEBUG("usart1 rx data length is %d data :", test_len);
			for(unsigned int i; i<sizeof(buffer); i++){
				DEBUG("%d", buffer[i]);
				
			}
			DEBUG("\n");
			ret = stream_data_handle(buffer);
			if(ret<0){
				DEBUG("stream_data_handle return error %d\n", ret);
			}			
		}
		else{
			if(rt_ringbuffer_data_len(ringbuffer_androidTx)>0){
				unsigned char data;
				rt_ringbuffer_get(ringbuffer_androidTx, &data, sizeof(data));
				switch(data){
					case 0x01:
						DEBUG("make done\n", ret);
					  send_result_ack(SENDOUTWATER_CMD, 0x02);
						break;
					default:
						DEBUG("MAKE DONE ERROR\n", ret);
					break;
				}
			}
			msleep(10);
		}
	}
}

static void start(void *arg)
{
	pthread_attr_t attr;
	ringbuffer_androidTx = rt_ringbuffer_create(20);
	pthread_attr_init(&attr);
	struct sched_param sched={sched_get_priority_max(SCHED_FIFO)-1};
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sched);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	open_uart();
	pthread_create(&tid, &attr, run, arg);
}

static void send_cmd_ack(unsigned char cmd)
{
	unsigned char data[]={0x7e, DIRDIRECTION_STC, CHANNEL_ONE , 0x02,cmd,0x03,0x00,0x00,0x7e};
	uint16_t crc = crc16_calculate(data+1, sizeof(data)-4);
	data[sizeof(data)-3] = crc>>8;
	data[sizeof(data)-2] = crc&0xff;
	rt_device_write(uart_dev, 0, data, sizeof(data));
}

static void send_result_ack(unsigned char cmd, unsigned char result)
{
	unsigned char data[]={0x7e, DIRDIRECTION_STC, CHANNEL_ONE, 0x02,cmd,result,0x00,0x00,0x7e};
	uint16_t crc = crc16_calculate(data+1, sizeof(data)-4);
	data[sizeof(data)-3] = crc>>8;
	data[sizeof(data)-2] = crc&0xff;
	rt_device_write(uart_dev, 0, data, sizeof(data));
}
static void send_heart_once(void)
{
	unsigned char data[]={0x7e, DIRDIRECTION_CTS, CHANNEL_ONE, 0x01, 0xff,0,0,0x7e};
	uint16_t crc = crc16_calculate(data, sizeof(data)-4);
	data[sizeof(data)-3] = crc>>8;
	data[sizeof(data)-2] = crc&0xff;
	rt_device_write(uart_dev, 0, data, sizeof(data));
}

static void send_out_water(unsigned char *pdata, unsigned int len)
{
	unsigned char cmd = SENDOUTWATER_CMD;
	unsigned int ret=-1;
	send_cmd_ack(SENDOUTWATER_CMD);
	if(*(status_enum_tydef *)WaterOut_class.pParameter == water_out){
		send_result_ack(cmd, 0x03);
	}
	else{
		DEBUG("rt_ringbuffer_put len %d\n", len);
		ret = rt_ringbuffer_put(ringbuffer_watercontrol, pdata, len);
		if(ret == len){
	//		send_result_ack(SENDOUTWATER_CMD, 0x02);
		}
		else{
			send_result_ack(SENDOUTWATER_CMD, 0x01);
		}		
	}
}

static void stop_out_water()
{
	unsigned char cmd = STOPOUTWATER_CMD;
	unsigned int ret=-1;
	DEBUG("stop out water\n");
	send_cmd_ack(STOPOUTWATER_CMD);
	
	if(*(status_enum_tydef *)WaterOut_class.pParameter == water_){
		send_result_ack(STOPOUTWATER_CMD, 0x02);
	}
	else{
		ret = rt_ringbuffer_put(ringbuffer_watercontrol, &cmd, sizeof(cmd));
		if(ret == sizeof(cmd)){
			DEBUG("stop out water success\n");
			send_result_ack(STOPOUTWATER_CMD, 0x02);
		}
		else{
			send_result_ack(STOPOUTWATER_CMD, 0x01);
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
static int data_hanlde(unsigned char *pdata, unsigned len)
{
	DEBUG("data_hanlde *pdata is %x %x\n", *pdata, *(pdata+1));
	switch(*pdata){
		case STOPOUTWATER_CMD:{stop_out_water();}break;
		case SENDOUTWATER_CMD:{
			send_out_water(pdata, len);
		}break;
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
	if (*(pdata+1) != DIRDIRECTION_CTS) {
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
	pck_length = 6 + data_length+length;
	crc_value = crc16_calculate(pdata+1, pck_length-4);
	
	if(crc_value != *(uint16_t *)(pdata + pck_length-3))
		ret = ERR_CRC;
	else{
		data_hanlde(pdata+3+length, data_length);

	}
	DEBUG("crc data is %x  %x %d %d\n", crc_value, *(uint16_t *)(pdata + pck_length-3), pck_length,data_length);
end:
	return ret;
}






