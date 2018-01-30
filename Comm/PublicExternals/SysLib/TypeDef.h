//==============================================================================
//                      TypeDef.h
//
//begin   : 2016-08-23
//describe:系统变量的定义
//==============================================================================
#ifndef SELFDEF_TYPEDEF_H
#define SELFDEF_TYPEDEF_H

// 类的导入、导出、数据类型等相关宏定义
#ifdef UNIX	// linux
	#define MY_EXT_CLASS			// linux下都编译成静态的，所以不需要导入导出
	
	// 类型的重定义，linux下没有这些
	typedef unsigned char	BYTE;		// 无符号单字节
	typedef unsigned short	WORD;		// 无符号双字节
	typedef unsigned int	DWORD;		// 无符号四字节
	typedef bool BOOL;					// bool型变量	
#else		// windows
	#include "afx.h"
	#ifdef _MYEXT
		#define MY_EXT_CLASS __declspec(dllexport)
	#else
		#define MY_EXT_CLASS __declspec(dllimport)
	#endif

	typedef WPARAM	DPARAM;				// 用于插件间消息的第三个参数
#endif

// 宏定义MAX MIN
#ifndef HP_UX
	#define MAX(x, y)	(((x) > (y)) ? (x) : (y))
	#define MIN(x, y)	(((x) < (y)) ? (x) : (y))
#endif

// 使用小端格式，内部协议使用时，不能大端小端混用
#define GETSTREAMINT(p)	((unsigned char)(p)[0] | (unsigned char)(p)[1] << 8 |\
						(unsigned char)(p)[2] << 16 | (unsigned char)(p)[3] << 24)

#define GETSTREAMSHORT(p) ((unsigned char)(p)[0] | (unsigned char)(p)[1] << 8)

#define SETSTREAMINT(p, i) do { (p)[0] = (char)i; (p)[1] = (char)(i >> 8); \
							(p)[2] = (char)(i >> 16); (p)[3] = (char)(i >> 24);	}while(0)

#define SETSTREAMSHORT(p, s) do{(p)[0]=(char)s;(p)[1]=(char)(s>>8);}while(0)


#define GETSTREAMFLOAT(f, p) do { ((char*)&f)[0] = (p)[0]; ((char*)&f)[1] = (p)[1]; \
							((char*)&f)[2] = (p)[2]; ((char*)&f)[3] = (p)[3];   }while(0)

#define SETSTREAMFLOAT(p, f) do { (p)[0] = ((char*)&f)[0]; (p)[1] = ((char*)&f)[1]; \
							(p)[2] = ((char*)&f)[2]; (p)[3] = ((char*)&f)[3];	}while(0)

// 使用大端格式，内部协议使用时，不能大端小端混用
#define GETSTREAMSHORT2(p) ((unsigned char)(p)[1] | (unsigned char)(p)[0] << 8)

#define SETSTREAMSHORT2(p, s) do{(p)[1]=(char)s;(p)[0]=(char)(s>>8);}while(0)

#define GETSTREAMINT2(p) ((unsigned char)(p)[3] | (unsigned char)(p)[2] << 8 |\
						(unsigned char)(p)[1] << 16 | (unsigned char)(p)[0] << 24)

#define SETSTREAMINT2(p, i)	do { (p)[3] = (char)i; (p)[2] = (char)(i >> 8); \
							(p)[1] = (char)(i >> 16); (p)[0] = (char)(i >> 24);	}while(0)

#define GETSTREAMFLOAT2(f, p) do { ((char*)&f)[0] = (p)[3]; ((char*)&f)[1] = (p)[2]; \
							((char*)&f)[2] = (p)[1]; ((char*)&f)[3] = (p)[0];   }while(0)

#define SETSTREAMFLOAT2(p, f) do { (p)[0] = ((char*)&f)[3]; (p)[1] = ((char*)&f)[2]; \
							(p)[2] = ((char*)&f)[1]; (p)[3] = ((char*)&f)[0];	}while(0)


#ifndef UNIX
	#define snprintf _snprintf
	#define vsnprintf _vsnprintf
#endif

/****************	name lens	***************************/ 
#define UNIX_PATH_LEN			255 
#define FILE_NAME_LEN			255
#define NAME_LEN				255
#define IP_LEN					16
#define SOCK_PATH_LEN			128
#define SESSION_NAME_LEN		64

// ===================应用层一个包最大的长度，如果超过此长度需要考虑分包=================
/* 应用层发送一个数据包时需要自行评估是否需要分包的逻辑代码，比如：发送一个查询请求，这种包
一般很小（除非有极特殊情况需要考虑分包），不需要分包，但是类如工程数据查询的回执，这个包一般
都很大，需要考虑分包。！！！！！！注意：超过此大小通信平台会当作无效包丢掉！！！！！！*/
#define MAX_APP_PACKETSIZE		(1000*1024)		// 约为1MB大小



#endif // !defined(SELFDEF_TYPE_DEF_H) 