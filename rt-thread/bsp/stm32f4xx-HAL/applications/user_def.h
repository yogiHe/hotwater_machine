#ifndef __USER_DEF_H
#define __USER_DEF_H

typedef void(*pfunction_t)(void *arg);

typedef struct {
	pfunction_t start;
}ThreadStruct_Tydef;

#define ThreadDef_Init(name)  \
		static void start(void *arg);\
		static void *run(void *arg);\
		ThreadStruct_Tydef name={\
			start,\
		}
#define msleep rt_thread_delay
//extern struct struct_def name


typedef void(*pfunction_t)(void *);














#endif















