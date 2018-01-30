//===================================================================================================
//	
//							 ProtocolMainten.h
//							   
//begin	  : 2016-10-07
//describe: 运行维护类
//
//===================================================================================================
#ifndef _PROTOCOL_MAINTEN_H
#define _PROTOCOL_MAINTEN_H

namespace ProtocolMaintenContent
{
	//===============================================================================================
	//	
	//										类型码定义
	//
	//===============================================================================================
	/*********************** 运行维护类型码定义 ****************************************/
	#define TYPE_MAINTEN					0X71	// 运行维护信息

	//===============================================================================================
	//	
	//										功能码定义
	//
	//===============================================================================================
	/********************************运行维护功能码定义***************************************/
	#define FUNC_LOCALMONICFG_REQ_INTERTOAPP			0X01	//请求本机监视配置信息【RmInterHost→RmAppHostMgr】
	#define FUNC_HOSTSMONICFG_APPTOINTER				0X02	//发布监视配置信息【RmAppHostMgr→RmInterHost】
	#define FUNC_CPUANDMEMREALTIME_INTERTOAPP			0X11	//计算机、进程的CPU、内存实时状态信息【RmInterHost→RmAppHostMgr】
	#define FUNC_HOSTALL_INTERTOAPP						0X12	//全体主机状态信息【RmInterHost→RmAppHostMgr】
	#define FUNC_HOSTCHG_INTERTOAPP						0X13	//变化主机状态信息【RmInterHost→RmAppHostMgr】
	#define FUNC_SYSSVRCONNSTATUS_SYSTOINTER			0X20	//主系统服务连接状态【主系统通信平台→RmInterSys】
	#define FUNC_SYSALL_INTERTOAPP						0X21	//全体主系统状态信息【RmInterSys→RmAppHostMgr】
	#define FUNC_SYSCHG_INTERTOAPP						0X22	//变化主系统状态信息【RmInterSys→RmAppHostMgr】
	#define FUNC_SYSALM_INTERTOAPP						0X23	//主系统报警信息【RmInterSys→RmAppHostMgr】
	#define FUNC_GROUPANDHOSTALL_REQ_CTRLTOAPP			0X31	//请求全体组及主机状态信息【RmControlMonitor→RmAppHostMgr】
	#define FUNC_GROUPHOSTALL_APPTOCTRL					0X32	//全体组及主机状态信息【RmAppHostMgr→RmControlMonitor】
	#define FUNC_GROUPCHG_APPTOCTRL						0X33	//变化组状态信息【RmAppHostMgr→RmControlMonitor】
	#define	FUNC_HOSTINGROUPCHG_APPTOCTRL				0X34	//变化组内主机状态信息【RmAppHostMgr→RmControlMonitor】
	#define	FUNC_SIGLEHOSTANDSERVICEALL_REQ_CTRLTOAPP	0X35	//请求全体单个主机状态信息【RmControlMonitor→RmAppHostMgr】
	#define FUNC_CPUANDMEMREALTIME_APPTOCTRL			0X36	//计算机、进程的CPU、内存实时状态信息【RmAppHostMgr→RmControlMonitor】
	#define	FUNC_SIGLEHOSTANDSERVICEALL_APPTOCTRL		0X37	//全体单个主机状态信息【RmAppHostMgr→RmControlMonitor】
	#define	FUNC_SIGLEHOSTANDSERVICECHG_APPTOCTRL		0X38	//变化单个主机状态信息【RmAppHostMgr→RmControlMonitor】
	#define FUNC_SYSINFO_REQ_CTRLTOAPP					0X39	//请求全体单个主机主系统状态信息【RmControlMonitor→RmAppHostMgr】
	#define	FUNC_SYSINFO_ALL_APPTOCTRL					0X3A	//全体单个主机主系统状态信息【RmAppHostMgr→RmControlMonitor】
	#define	FUNC_SYSINFO_CHG_APPTOCTRL					0X3B	//变化单个主机主系统状态信息【RmAppHostMgr→RmControlMonitor】

	#define	FUNC_HOSTCURVER_REQ_CTRLTOAPP				0X51	// 请求主机当前软件数据版本【RmControlMonitor→RmInterHost】
	#define	FUNC_HOSTCURVER_REPLY_APPTOCTRL				0X52	// 主机软件数据当前版本信息【RmInterHost→RmControlMonitor】
	#define FUNC_HOSTVERUPDATE_REQ_CTRLTOAPP			0X53	// 更新主机软件数据版本【RmControlMonitor→RmInterHost】
	#define	FUNC_HOSTVERUPDATE_REPLY_APPCTRL			0X54	// 更新主机软数据版本更新回执【RmInterHost→RmControlMonitor】
	#define	FUNC_HOSTHISVER_REQ_CTRLTOAPP				0X55	// 请求主机软件数据历史版本信息【RmControlMonitor→工程数据存取服务】
	#define FUNC_HOSTHISVER_REPLY_APPTOCTRL				0X56	// 主机软件数据历史版本信息【工程数据存取服务→RmControlMonitor】
	#define	FUNC_HOSTSAVEVER_REQ_CTRLTOOSM				0X57	// 存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	#define	FUNC_HOSTSAVEVER_REPLY_OSMTOCTRL			0X58	// 主机软件数据版本存储回执【工程数据存取服务→RmControlMonitor】
	
	// 更新结果回执
	enum ENUM_UPDATE_RESULT
	{
		EUR_SUCCEED	= 0X01,							// 成功
		EUR_FAIL	= 0X02,							// 失败
	};

	// 更新参数
	enum ENUM_UPDATE_PARA
	{
		EUP_UPDATE = 0X01,							// 更新
		EUP_REVISION = 0X02,						// 更新到指定版本
	};

	// 网络连接类型
	#define	UNKNOW_CONN									0x00	// 未定义
	#define AF_INET_CONN								0x01	// AF_INET
	#define AF_UNIX_CONN								0x02	// AF_UNIX

	// 服务器主备状态
	#define SVR_BACK									0x00	// 备机
	#define SVR_MAIN									0x01	// 主机
	#define SVR_STANDBY_UNKNOW							0x02	// 未知
	#define SVR_NO_STANDBY								0x03	// 无效

	// 站通信状态
	#define STACOMM_ERROR								0x00	// 异常
	#define STACOMM_OK									0x01	// 连接正常
	#define STACOMM_NO_EXIST							0x02	// 不存在
	#define STACOMM_UNKNOW								0x03	// 未知

	// 运维系统监控类型定义
	enum ENUM_MAINTAIN_TYPE
	{
		EMTT_INVALID		= 0,			// 未定义
		EMTT_OS				= 1,			// 操作系统
		EMTT_CPU			= 2,			// CPU
		EMTT_MEM			= 3,			// 内存
		EMTT_DISK			= 4,			// 磁盘
		EMTT_NETWORK		= 5,			// 网络
		EMTT_PROGRAM		= 6,			// 进程
		EMTT_STANDBY		= 7,			// 应用服务主备状态
		EMTT_SERVICECONN	= 8,			// 应用服务连接状态
		EMTT_CHAN			= 9,			// 通道状态
		EMTT_ALARM			= 10,			// 报警状态

		EMTT_MAXMONITYPE,					// 监控类型最大值，不允许在此枚举后定义新类型
	};

	// 信息有效性
	#define	VALIDITY_NO									0x00	// 无效
	#define	VALIDITY_YES								0x01	// 有效

	// 主机组状态
	#define HOST_GROUP_STATE_UNKNOW						0x00	// 未知
	#define HOST_GROUP_STATE_ERROR						0x01	// 故障
	#define HOST_GROUP_STATE_NORMAL						0x02	// 正常
	#define HOST_GROUP_STATE_DISCONNECT					0x03	// 通信故障
}

#endif