#include "gprs_thread.h"
#include "pthread.h"

typedef void(*pfunction_t)(void *);

struct GPRS_THREAD_CLASS{
		pfunction_t start;
};

static void start(void *arg);
static void *run(void *arg);

static pthread_t tid;

struct GPRS_THREAD_CLASS gprs_class={
	start,
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
