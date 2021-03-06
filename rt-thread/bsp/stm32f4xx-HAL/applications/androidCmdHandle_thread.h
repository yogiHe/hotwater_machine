#ifndef __ANDROIDCMDHANDLE_THREAD_H
#define __ANDROIDCMDHANDLE_THREAD_H


#define START_FRAME		0x7E
#define DIRDIRECTION_STC	0x00 //embed to android
#define DIRDIRECTION_CTS	0x01 //android to embed
#define CHANNEL_ONE			0x01
#define CHANNEL_TWO			0x02
#define CHANNEL_FOUR		0x04

#define STOPOUTWATER_CMD	0x03
#define CALIBRATION_CMD	0x02
#define SENDOUTWATER_CMD	0x01
#define NO_ERR				0
#define ERR_DIRDIRECTION	-1
#define ERR_CHANNEL			-2
#define ERR_CRC				-3

typedef enum status_control
{
	water_=0,
	water_out,
}status_enum_tydef;
#endif