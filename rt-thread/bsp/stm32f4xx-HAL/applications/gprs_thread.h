#ifndef __GPRS_THREAD_H
#define __GPRS_THREAD_H

#define SERVER_IP	"192.168.0.1"
#define SERVER_PORT	"8080"
#define SETServerInfo_Stream	"AT+CIPSTART=TCP,$SERVER_IP,$SERVER_PORT"


/*
#define CMD_CHECK(cmd, cmp_value) 

static void cmd##_check(){
	static char buffer[20]={0};
	printf("initial system $cmd");
	for(;;){
		write("$cmd\r\n");
		read(buffer);
		if(strncmp(buffer, cmp_value, strlen(cmp_value)) == 0){
			break;
		}
		else{
			printf(".");
		}
	}
	printf("\r\n");
}
*/

#endif