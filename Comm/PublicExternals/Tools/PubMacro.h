//==============================================================================
//                      PubMaro.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_MACRO_H 
#define SELFDEF_MACRO_H 

#define NOT_FINISH 
#define UNUSED_PARAM(x)			 

//库的版本方便输出工具宏定义
#define LIBVER(LIBNAME) \
	string("Lib: ") + LIBNAME##_GetVersion() + "\n"
//	string("need ") + LIBNAME##_VERSION + "; current " + LIBNAME##_GetVersion() + "\n"


#endif 
