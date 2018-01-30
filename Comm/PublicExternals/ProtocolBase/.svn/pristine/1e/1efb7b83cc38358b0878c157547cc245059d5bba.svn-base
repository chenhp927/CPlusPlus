//==============================================================================
//	
//							 ProtocolBase.h
//							   
//begin	  : 2016-08-10
//describe: 协议基础类
//
//==============================================================================

#ifndef _PROTOCOL_BASE_H
#define _PROTOCOL_BASE_H

namespace ProtocolBase
{
	//==============================================================================
	//	
	//							  类型码定义
	//
	//==============================================================================
	#define TYPE_REGISTER			0X98	// 注册信息包 
	
	//==============================================================================
	//	
	//							  功能码定义
	//
	//==============================================================================
	/********************* 注册信息相关功能码定义 *****************/ 
	#define FUNC_REGISTER			0X01	// 注册 
	#define FUNC_UNREGISTER			0x02	// 取消注册 
	#define FUNC_CLEARREG			0x03	// 取消all注册 
		
	//==============================================================================
	//	
	//							  常量定义
	//
	//==============================================================================
	/********************* 注册信息相关常量定义 *****************/ 
	#define MC_ALLLINE				0XFF			// 全部线路 
	#define MC_ALLNODETYPE			0XFF			// 全部节点类型
	#define MC_ALLNODE				0XFFFF			// 全部节点ID
	#define MC_ALLTYPE				0XFF			// 全部类型码 
	#define MC_ALLFUNC				0XFF			// 全部功能码
	#define MC_ALLOPT				0XFF
		
	#define MC_IGNLINE				0
	#define MC_IGNNODETYPE			0
	#define MC_IGNNODEID			0
		
	/********************* 节点信息相关常量定义 *****************/ 
	#define NODE_TYPE_STA			0x01		// 节点类型：车站
	#define NODE_TYPE_CENTER		0x02		// 节点类型：中心
	#define NODE_ADDR_LEN			0x04		// 节点地址长度

	/********************* 协议头相关常量定义 *****************/ 
	#define PROTOCOL_VERSION		0x01		// 协议版本号
	#define PROTOCOL_HEAD_LEN		0x0d		// 协议包头正确
	#define PROTOCOL_INFO_LEN		0x100000	// 数据包信息内容总字节数(不包括类型码、功能码、版本号、字长、总包数、当前包号、请求命令编号)，不能超过0x100000
	#define PROTOCOL_OK				0x00		// 协议包头正确
	#define VERSION_ERROR			0X01		// 版本错误
	#define PACKSUM_ERROR			0X02		// 包数错误
	#define PACKLEN_ERROR			0X03		// 包长错误

	/******************** UUID相关定义 ***********************/
	#define	UUID_LENGTH				16			// UUID的字节长度

	/******************** 协议中传输的时间相关定义 ***********************/
	#define	PROTOCOL_TIME_LEN		8			// 协议中传输的时间长度

	/*******************************************************************/
	// 字符串长度
	#define MAX_WORDSIZE			65535				// 2字节 
	#define MAX_BYTESIZE			255				// 1字节
}

#endif	 // _PROTOCOL_BASE_H
