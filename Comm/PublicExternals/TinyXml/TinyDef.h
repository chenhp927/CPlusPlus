//==============================================================================
//                      TinyDef.h
//
//begin   : 2016-09-22
//describe: 
//==============================================================================
#ifndef SELFDEF_TINYDEF_H 
#define SELFDEF_TINYDEF_H

/* ���еĺ�ΪTinyXml�⵼�롢��������ض��� */
#ifdef UNIX	// linux
	#define MY_EXT_XML_CLASS			// linux�¶�����ɾ�̬�ģ����Բ���Ҫ���뵼��
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