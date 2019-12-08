#include "gprs_thread.h"
#include "pthread.h"
#include "user_def.h"

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
	pthread_create(&tid, NULL, run, arg);
}

static void cmd_check(char *cmd, char *check_value, int count){
	static char buffer[20]={0};
	printf("initial system %s", cmd);
	for(int i=0; i<count; i++){
		write(cmd);
		read(buffer);
		if(strncmp(buffer, check_value, strlen(check_value)) == 0){
			break;
		}
		else{
			printf(".");
		}
	}
	printf("\r\n");
}



static void gprs_system_on(void)
{
	cmd_check("AT\r\n", "OK", 0xFFFFFFFF);
	cmd_check("AT+CPIN?\r\n", );
}
static void tcp_connect(char *Server_IP, char *Server_Port)
{
	write(SETServerInfo_Stream);
}

static void tcp_send(unsigned char *pdata, unsigned int lenght)
{

}