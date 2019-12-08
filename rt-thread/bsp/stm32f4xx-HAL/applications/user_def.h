#ifndef __USER_DEF_H
#define __USER_DEF_H

typedef void(*pfunction_t)(void *arg);

#define ThreadStructName_def(A) A_##CLASS

#define ThreadStruct(A)  struct A{\
		pfunction_t start;\
}
#define ThreadDef_Init(struct_def, name)  \
		static void start(void *arg);\
		static void *run(void *arg);\
		struct struct_def name={\
			start,\
		};\
		extern struct struct_def name


typedef void(*pfunction_t)(void *);















#endif















