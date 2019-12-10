#include "gprs_thread.h"
#include "pthread.h"
#include "user_def.h"
#include <rtdevice.h>

#define write(A) //rt_device_write();
#define read(A)

static void open_uart();
ThreadStruct(ThreadStructName_def(GPRS_THREAD));
ThreadDef_Init(ThreadStructName_def(GPRS_THREAD), gprs_class);

static pthread_t tid;
static void *run(void *arg)
{
	for(;;){
	
	}
}

static void start(void *arg)
{
	pthread_attr_t attr;
	gprs_system_on();
	pthread_attr_init(&attr);
	struct sched_param sched={sched_get_priority_max(SCHED_FIFO)};
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &sched);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, run, arg);
}

static void cmd_check(char *cmd, char *check_value, int count){
	static char buffer[20]={0};
	rt_kprintf("initial system %s", cmd);
	for(int i=0; i<count; i++){
		write(cmd);
		read(buffer);
		if(strncmp(buffer, check_value, strlen(check_value)) == 0){
			break;
		}
		else{
			rt_kprintf(".");
		}
	}
	rt_kprintf("\r\n");
}



static void gprs_system_on(void)
{
	open_uart();
	cmd_check("AT\r\n", "OK", 0xFFFFFFFF);
	cmd_check("AT+CPIN?\r\n", "OK", 0x50);
	cmd_check("AT+CIICR\r\n", "OK", 0x50);
	cmd_check("AT+CIFSR\r\n", "OK", 0x50);
}
static void tcp_connect(char *Server_IP, char *Server_Port)
{
	write(SETServerInfo_Stream);
}

static void tcp_send(unsigned char *pdata, unsigned int lenght)
{

}
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