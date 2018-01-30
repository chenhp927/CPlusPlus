//==============================================================================
//                      TinyDef.h
//
//begin   : 2016-09-22
//describe: 
//==============================================================================
#ifndef SELFDEF_TINYDEF_H 
#define SELFDEF_TINYDEF_H

/* 下列的宏为TinyXml库导入、导出的相关定义 */
#ifdef UNIX	// linux
	#define MY_EXT_XML_CLASS			// linux下都编译成静态的，所以不需要导入导出
#else		// windows
	#ifdef _MYEXT_XML
		#ifndef MY_EXT_XML_CLASS
		#define MY_EXT_XML_CLASS __declspec(dllexport)
		#endif
	#else
		#ifndef MY_EXT_XML_CLASS
		#define MY_EXT_XML_CLASS __declspec(dllimport)
		#endif
	#endif
#endif




#endif