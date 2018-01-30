//=========================================================================================
//	
//							 ProtocolIndication.h
//							   
// begin    : 2016-08-23
// describe : 协议信息定义
//
//=========================================================================================

#ifndef _PROTOCOL_INDICATION_H
#define _PROTOCOL_INDICATION_H

namespace ProtocolIndication
{
	//=====================================================================================
	//	
	//							  类型码定义
	//
	//=====================================================================================
	/*********************** 遥信信息相关类型码定义 ****************************************/
	#define TYPE_SIGNAL_INFO				0X11	// 遥信信息

	/*********************** 遥测信息相关类型码定义 ****************************************/
	#define TYPE_MEASURE_INFO				0X12	// 遥测信息

	/*********************** 设备状态相关类型码定义 ****************************************/
	#define TYPE_DEVSTATE					0X14	// 设备状态

	//=====================================================================================
	//	
	//							  功能码定义
	//
	//=====================================================================================
	/************************ 遥信信息相关功能码定义 ***************************************/ 
	#define FUNC_SIGNALALL_INTERTOSVR		0X01	// 遥信全体信息【InterSvr104→AppSignalMgr】
	#define FUNC_SIGNALCHG_INTERTOSVR		0X02	// 遥信变化信息【interSvr104→AppSignalMgr】
	#define FUNC_SIGNALALL_SVRTOTERM		0X03	// 遥信全体信息【AppSignalMgr→操作台】
	#define FUNC_SIGNALCHG_SVRTOTERM		0X04	// 遥信变化信息【AppSignalMgr→操作台】
	#define FUNC_SIGNALCALL_INTERTOSVR		0X05	// 遥信总召全体信息【InterSvr104→AppSignalMgr】
	#define FUNC_SIGNALREQ_TERMTOSVR		0X10	// 遥信请求信息【操作台→AppSignalMgr】
	#define FUNC_SIGNALREQ_SVRTOINTER		0X11	// 遥信请求信息【AppSignalMgr→InterSvr104】

	/********************** 遥信变位通知信息相关功能码定义 **********************************/
	#define FUNC_SIGNALCHGSTATE_SVRTOOTHER	0X21	// 遥信变位信息【AppSignalMgr→实时通道】

	/************************* SOE信息相关功能码定义 ***************************************/
	#define FUNC_COLLECTSOE					0X30	// SOE信息【InterSvr104→AppSignalMgr】
	#define FUNC_SOEREPLY					0X31	// SOE信息回执【AppSignalMgr→InterSvr104】
	#define FUNC_SOESAVE					0X32	// SOE存盘信息【AppSignalMgr→OSM模块】
	#define FUNC_SOESAVEREPLY				0X33	// SOE存盘信息回执【OSM模块→AppSignalMgr】
	#define FUNC_SOEQUERYREQ				0X40	// SOE查询请求【其他模块→OSM模块】
	#define	FUNC_SOEQUERYRSLT				0X50	// SOE查询结果【OSM模块→其他模块】

	/************************* 设备状态相关功能码定义 **************************************/
	#define FUNC_STACOMM_SVRTOINTER			0X01	// 请求站通信设备状态【其他模块→InterSvr104】
	#define FUNC_STACOMM_INTERTOSVR			0X02	// 站通信设备状态发布【InterSvr104→其他模块】
	#define FUNC_OPERALL_TERMTOSVR			0X21	// 请求全体操控设备状态【操作台→AppStatusMgr】
	#define FUNC_OPERALL_SVRTOTERM			0X22	// 全体操控设备状态【AppStatusMgr→操作台】
	#define FUNC_OPERCHG_SVRTOTERM			0X23	// 变化操控设备状态【AppStatusMgr→操作台】
	#define FUNC_DISPALL_TERMTOSVR			0X31	// 请求全体电显设备状态【操作台→AppStatusMgr】
	#define FUNC_DISPALL_SVRTOTERM			0X32	// 全体电显设备状态【AppStatusMgr→操作台】
	#define FUNC_DISPCHG_SVRTOTERM			0X33	// 变化电显设备状态【AppStatusMgr→操作台】
	#define FUNC_DISPCHG_SVRRTOSVR			0X34	// 变化电显设备状态【AppElecCalc→AppStatusMgr】
	#define FUNC_DISPCHG_TERMTOSVR			0X35	// 重新拓扑电显设备状态【操作台→AppElecCalc】

	/************************* 遥测信息相关功能码定义 **************************************/
	#define FUNC_MEASREQ_SVRTOINTER			0X01	// 请求遥测信息【AppMeasureMgr→ InterSvr104】
	#define FUNC_MEASREQ_TERMTOSVR			0X02	// 请求遥测信息【操作台→AppMeasureMgr】
	#define FUNC_MEASALL_INTERTOSVR			0X03	// 全体遥测信息【InterSvr104→AppMeasureMgr】
	#define FUNC_MEASCHG_INTERTOSVR			0X04	// 变化遥测信息【InterSvr104→AppMeasureMgr】
	#define FUNC_MEASALL_SVRTOTERM			0x05	// 全体遥测信息【AppMeasureMgr→操作台】
	#define FUNC_MEASCHG_SVRTOTERM			0X06	// 变化遥测信息【AppMeasureMgr→操作台】
	#define FUNC_MEACALL_INTERTOSVR			0X07	// 总召遥测信息【InterSvr104→AppMeasureMgr】
	#define FUNC_MEASVAL_SVRTOOSM			0X10	// 遥测值信息存储【AppMeasureMgr→OSM模块】
	#define FUNC_MEASLIMIT_SVRTOOSM			0X11	// 遥测越限信息存储【AppMeasureMgr→OSM模块】
	#define FUNC_MEASSAVE_OSMTOSVR			0X12	// 遥测信息存储回执【OSM模块→AppMeasureMgr】
	#define FUNC_MEASHIS_TERMTOOSM			0X13	// 遥测历史信息查询【操作台→OSM模块】
	#define FUNC_MEASHIS_OSMTOTERM			0X14	// 遥测信息查询回执【OSM模块→操作台】
	#define FUNC_MEASHIS_OSMFAILREPLY		0X15	// 遥测信息查询失败回执【OSM模块→操作台】

	/************************** 协议中遥信点的相关定义 *************************************/
	#define PROTOCOL_SIGNALSTANUM_LEN		2		// 协议中遥信车站数量长度
	#define PROTOCOL_SIGNALSTAID_LEN		2		// 协议中遥信车站Id长度
	#define PROTOCOL_SIGNALNUM_LEN			2		// 协议中遥信数量的长度
	#define	PROTOCOL_SIGNALVALUE_LEN		1		// 协议中遥信点值的长度
	#define PROTOCAL_SIGNALADDR_LEN			4		// 协议中遥信地址的长度

	/************************** 协议中遥测点的相关定义 *************************************/
	#define PROTOCOL_MEASURESTANUM_LEN		2		// 协议中遥测车站数量长度
	#define PROTOCOL_MEASURESTAID_LEN		2		// 协议中遥测车站Id长度
	#define PROTOCOL_MEASURENUM_LEN			2		// 协议中遥测数量长度	
	#define PROTOCOL_MEASUREADDR_LEN		4		// 协议中遥测地址长度
	#define PROTOCOL_MEASUREFLAG_LEN		1		// 协议中遥测有效性标识长度
	#define PROTOCOL_MEASUREVALUE_LEN		4		// 协议中遥测点值得长度
	
	// 遥测值有效性标识
	#define MEASUREFLAG_INEFFECTIVE			0		// 遥测值有效性标识，0：无效效
	#define MEASUREFLAG_EFFECTIVE			1		// 遥测值有效性标识，1：有效

	/************************** 协议中设备状态的相关定义 ***********************************/
	// AB机标识
	#define ABFLAG_A						0X00	// AB机标识-A机
	#define ABFLAG_B						0X01	// AB机标识-B机

	// 通道主备状态
	#define CHANNEL_SPARE					0X00	// 通道主备状态-通道备机
	#define CHANNEL_MAIN					0X01	// 通道主备状态-通道主机	

	// 站通道通信状态
	#define STACHANNELCOMM_ABNORMAL			0X00	// 站通道通信状态：连接异常
	#define STACHANNELCOMM_NORMAL			0X01	// 站通道通信状态：连接正常
	#define STACHANNELCOMM_NOCHANNEL		0X02	// 站通道通信状态：该通道不存在

	// 参数设置
	#define STA_SIGNAL_MAX_NUM				3000	// 站遥信最大数量
	#define STA_MEASURE_MAX_NUM				1000	// 站遥测最大数量
}

#endif	 // _PROTOCOL_INDICATION_H