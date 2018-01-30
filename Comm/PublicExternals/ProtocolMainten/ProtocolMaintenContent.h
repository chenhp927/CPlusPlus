//=========================================================================================
//
//							 ProtocolMaintenContent.h
// begin    : 2016-10-10
// describe : 运行维护协议集
//
//=========================================================================================
#ifndef _PROTOCOLMAINTENCONTENT_H_2016_10_10
#define _PROTOCOLMAINTENCONTENT_H_2016_10_10

#include <vector>
#include "TypeDef.h"
#include "ProtocolBase.h"
#include "ProtocolHead.h"
#include "ProtocolDevState.h"	
#include "ProtocolMaintenComm.h"

namespace ProtocolMaintenContent
{
	using namespace std;
	using namespace ProtocolBase;
	using namespace ProtocolIndication;
	
	//==============================================================================
	//
	//             CReqLocalCfgItoAPkt
	//             请求本机监视配置信息【RmInterHost→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CReqLocalCfgItoAPkt : public CProtocolHead
	{
	public:
		CReqLocalCfgItoAPkt();
		~CReqLocalCfgItoAPkt();
		
	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机名
		string m_strHostName;
	};

	//==============================================================================
	//
	//             CMoniInfoBase
	//          监视信息基类，用于派生各个监控类型类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoBase : public CObjectBase
	{
	public:
		CMoniInfoBase();
		virtual ~CMoniInfoBase();

	public:
		void SetMoniType(BYTE byMoniType);
		BYTE GetMoniType() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	protected:
		BYTE m_byMoniType;		// 监控类型
	};
	
	//==============================================================================
	//
	//             CLocalProcItoA
	//             进程信息
	//
	//==============================================================================
	class MY_EXT_CLASS CLocalProcItoA : public CObjectBase
	{
	public:
		CLocalProcItoA();
		~CLocalProcItoA();
		
	public:
		// 设置进程名
		void SetProcName(string strProcName);
		// 获取进程名
		string GetProcName() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 进程名
		string m_strProcName;
	};
	
	//==============================================================================
	//
	//             CSingleHostCfg
	//             单机监视配置信息
	//
	//==============================================================================
	class MY_EXT_CLASS CSingleHostCfg : public CObjectBase
	{
	public:
		CSingleHostCfg();
		~CSingleHostCfg();
		
	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;
		
		// 获取监控类型个数
		int GetMoniTypeInfoCount() const;
		// 获取指定下标的监控类型
		const CMoniInfoBase* GetMoniTypeInfo(int nIndex) const;
		// 添加监控类型
		void Add(const CMoniInfoBase& objMoniTypeInfo);
		void Clear();

		// 获取进程信息个数
		int GetLocalProcItoACount() const;
		// 获取指定下标的进程信息
		const CLocalProcItoA* GetLocalProcItoA(int nIndex) const;
		// 添加进程信息
		void Add(const CLocalProcItoA& objLocalProcItoA);
		void CLocalProcItoAClear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机名
		string m_strHostName;
		// 监控类型
		vector<CMoniInfoBase> m_vecMoniTypeInfo;
		// 进程信息
		vector<CLocalProcItoA> m_vecLocalProcItoA;
	};

	//==============================================================================
	//
	//             CHostsCfgAtoIPkt
	//             发布监视配置信息【RmAppHostMgr→RmInterHost】
	//
	//==============================================================================
	class MY_EXT_CLASS CHostsCfgAtoIPkt : public CProtocolHead
	{
	public:
		CHostsCfgAtoIPkt();
		~CHostsCfgAtoIPkt();

	public:
		// 获取主机个数
		int GetHostsCount() const;
		// 获取指定下标的主机配置信息
		const CSingleHostCfg* GetSingleHostCfg(int nIndex) const;
		// 添加监控类型
		void Add(const CSingleHostCfg& objSingleHostCfg);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 主机监视配置信息
		vector<CSingleHostCfg> m_vecHostsCfg;
	};

	//==============================================================================
	//
	//						CPU负载子类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoCpuLoad : public CMoniInfoBase
	{
	public:
		CMoniInfoCpuLoad();
		~CMoniInfoCpuLoad();

	public:
		// 设置CPU占用率
		void SetCPULoadPer(float fCPULoadPer);
		// 获取CPU占用率
		float GetCPULoadPer() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		float m_fCPULoadPer; // CPU占用率
	};

	//==============================================================================
	//
	//						内存子类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoMemLoad : public CMoniInfoBase
	{
	public:
		CMoniInfoMemLoad();
		~CMoniInfoMemLoad();

	public:
		// 设置内存占用率
		void SetMemLoadPer(float fMemLoadPer);
		// 获取内存占用率
		float GetMemLoadPer() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 内存占用率
		float m_fMemLoadPer;
	};

	//==============================================================================
	//
	//             CCPUAndMEMOfProcItoA
	//             进程信息
	//
	//==============================================================================
	class MY_EXT_CLASS CCPUAndMEMOfProcItoA : public CObjectBase
	{
	public:
		CCPUAndMEMOfProcItoA();
		~CCPUAndMEMOfProcItoA();
		
	public:
		// 设置进程名
		void SetProcName(string strProcName);
		// 获取进程名
		string GetProcName() const;
		
		// 设置进程ID
		void SetPID(DWORD dwPID);
		// 获取进程ID
		DWORD GetPID() const;
		
		// 设置CPU占用率
		void SetCPULoadPer(float fCPULoadPer);
		// 获取CPU占用率
		float GetCPULoadPer() const;
		
		// 设置内存占用率
		void SetMemLoadPer(float fMemLoadPer);
		// 获取内存占用率
		float GetMemLoadPer() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 进程名
		string m_strProcName;
		
		// 进程ID
		DWORD m_dwPID;
		
		// CPU占用率
		float m_fCPULoadPer;
		
		// 内存占用率
		float m_fMemLoadPer;
	};
	
	//==============================================================================
	//
	//					进程子类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoProcess : public  CMoniInfoBase
	{
	public:
		CMoniInfoProcess();
		~CMoniInfoProcess();

	public:
		// 获取进程信息个数
		int GetCPUAndMEMOfProcItoACount() const;
		// 获取指定下标的进程信息
		CCPUAndMEMOfProcItoA* GetCPUAndMEMOfProcItoA(int nIndex);
		// 添加进程信息
		void Add(const CCPUAndMEMOfProcItoA objCPUAndMEMOfProcItoA);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector <CCPUAndMEMOfProcItoA> m_vecMoniInfoProcess;		//进程信息
	};

	//==============================================================================
	//
	//             CCpuAndMemItoAPkt
	//             计算机、进程的CPU、内存实时状态信息【RmInterHost→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CCpuAndMemItoAPkt : public CProtocolHead
	{
	public:
		CCpuAndMemItoAPkt();
		~CCpuAndMemItoAPkt();

		//重载运算符=，深拷贝
		CCpuAndMemItoAPkt& operator=(const CCpuAndMemItoAPkt& objCpuAndMemItoAPkt);
		//拷贝构造函数，深拷贝
		CCpuAndMemItoAPkt(const CCpuAndMemItoAPkt& objCpuAndMemItoAPkt);

	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;
		
		// 获取监控信息个数
		int GetMoniInfoBaseCount() const;
		// 获取指定下标的监控信息
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 添加监控信息
		// 添加CPU负载信息
		void AddCPULoad(const float fCPULoad);

		//添加内存信息
		void AddMEMLoad(const float fMEMLoad);

		//添加进程信息
		void AddProcess(const CMoniInfoProcess& objProcess);
		
	private:
		// 主机名
		string m_strHostName;
		//监控信息
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//				 CMoniInfoOS
	//				 操作系统
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoOS : public CMoniInfoBase
	{
	public:
		CMoniInfoOS();
		~CMoniInfoOS();

	public:
		// 设置操作系统名称
		void SetOSName(string strOSName);
		// 获取操作系统名称
		string GetOSName() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strOSName;	//操作系统名称
	};

	//==============================================================================
	//
	//						CPU个数子类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoCpuNum : public CMoniInfoBase
	{
	public:
		CMoniInfoCpuNum();
		~CMoniInfoCpuNum();

	public:
		// 设置CPU个数
		void SetCPUNum(BYTE byCPUNum);
		// 获取CPU个数
		BYTE GetCPUNum() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		BYTE m_byCPUNum; // CPU个数
	};

	//==============================================================================
	//
	//						内存容量子类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoMemCap : public CMoniInfoBase
	{
	public:
		CMoniInfoMemCap();
		~CMoniInfoMemCap();

	public:
		// 设置内存占用率
		void SetMemCap(float fMemCap);
		// 获取内存占用率
		float GetMemCap() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 内存占用率
		float m_fMemCap;
	};

	//==============================================================================
	//
	//             CDisk
	//             磁盘信息
	//
	//==============================================================================
	class MY_EXT_CLASS CDisk : public CObjectBase
	{
	public:
		CDisk();
		~CDisk();
		
	public:
		// 设置磁盘名称
		void SetDiskName(string strDiskName);
		// 获取磁盘名称
		string GetDiskName() const;
		
		// 设置磁盘容量
		void SetDiskCap(float fDiskCap);
		// 获取磁盘容量
		float GetDiskCap() const;
		
		// 设置磁盘使用率
		void SetDiskLoadPer(float fDiskLoadPer);
		// 获取磁盘使用率
		float GetDiskLoadPer() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 磁盘名称
		string m_strDiskName;
		// 磁盘容量
		float m_fDiskCap;
		// 磁盘使用率
		float m_fDiskLoadPer;
	};
	
	//==============================================================================
	//
	//             CMoniInfoDisk
	//             继承监视信息基类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoDisk : public CMoniInfoBase
	{
	public:
		CMoniInfoDisk();
		~CMoniInfoDisk();

	public:
		// 获取磁盘信息个数
		int GetDiskCount() const;
		// 获取指定下标的磁盘信息
		CDisk* GetDisk(int nIndex);
		// 添加磁盘信息
		void Add(const CDisk& objDisk);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CDisk> m_vecDisk;	//磁盘信息
	};

	//==============================================================================
	//
	//             CIPInfo
	//             IP地址信息
	//
	//==============================================================================
	class MY_EXT_CLASS CIPInfo : public CObjectBase
	{
	public:
		CIPInfo();
		~CIPInfo();
		
	public:
		// 设置IP地址
		void SetIP(string strIP);
		// 获取IP地址
		string GetIP() const;
		
		// 设置子网掩码
		void SetMask(string strMask);
		// 获取子网掩码
		string GetMask() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// IP地址
		string m_strIP;
		// 子网掩码
		string m_strMask;
	};
	
	//==============================================================================
	//
	//             CNetCard
	//             网卡信息
	//
	//==============================================================================
	class MY_EXT_CLASS CNetCard : public CObjectBase
	{
	public:
		CNetCard();
		~CNetCard();
		
	public:
		// 设置网卡名称
		void SetCardName(string strCardName);
		// 获取网卡名称
		string GetCardName() const;
		
		// 设置网卡状态
		void SetCardState(BYTE byCardState);
		// 获取网卡状态
		BYTE GetCardState() const;
		
		// 获取IP地址信息个数
		int GetIPInfoCount() const;
		// 获取指定下标的IP地址信息
		const CIPInfo* GetIPInfo(int nIndex) const;
		// 添加IP地址信息
		void Add(const CIPInfo& objIPInfo);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 网卡名称
		string m_strCardName;
		// 网卡状态
		BYTE m_byCardState;
		// IP地址信息
		vector<CIPInfo> m_vecIPInfo;
	};

	//==============================================================================
	//
	//             CMoniInfoNetCard
	//             继承监视信息基类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoNetCard : public CMoniInfoBase
	{
	public:
		CMoniInfoNetCard();
		~CMoniInfoNetCard();

	public:
		// 获取网卡信息个数
		int GetMoniInfoNetCardCount() const;
		// 获取指定下标的网卡信息
		const CNetCard* GetMoniInfoNetCard(int nIndex) const;
		// 添加网卡信息
		void Add(const CNetCard& objNetCard);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CNetCard> m_vecMoniInfoNetCard;	//网卡信息
	};

	//==============================================================================
	//
	//             CAllHostProcItoA
	//            
	//
	//==============================================================================
	class MY_EXT_CLASS CAllHostProcItoA : public CObjectBase
	{
	public:
		CAllHostProcItoA();
		~CAllHostProcItoA();
		
	public:
		// 设置进程名称
		void SetProcName(const string& strProcName);
		// 获取进程名称
		string GetProcName() const;
		
		// 设置进程ID
		void SetPID(DWORD dwPID);
		// 获取进程ID
		DWORD GetPID() const;
		
		// 设置进程路径
		void SetProcPath(const string& strProcPath);
		// 获取进程路径
		string GetProcPath() const;
		
		// 设置进程版本
		void SetProcVer(const string& strProcVer);
		// 获取进程版本
		string GetProcVer() const;
		
		// 设置进程文件大小
		void SetProcFileSize(float fProcFileSize);
		// 获取进程文件大小
		float GetProcFileSize() const;
		
		// 进程文件创建时间
		void SetCreatTime(time_t& tm, WORD wMs = 0);	//	wMs：毫秒（0~999）
		CProtocolTime GetCreatProtocolTime() const;

		void SetCreatTime(timeval tTime);
		timeval GetCreatTime() const;

		// 进程文件修改时间
		void SetModifyTime(time_t& tm, WORD wMs = 0);	//	wMs：毫秒（0~999）
		CProtocolTime GetModifyProtocolTime() const;

		void SetModifyTime(timeval tTime);
		timeval GetModifyTime() const;

		// 进程启动时间
		void SetStartTime(time_t& tm, WORD wMs = 0);	//	wMs：毫秒（0~999）
		CProtocolTime GetStartProtocolTime() const;
		void SetStartTime(timeval tTime);
		timeval GetStartTime() const;

		// 设置进程状态
		void SetProcState(BYTE byProcState);
		// 获取进程状态
		BYTE GetProcState() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 进程名称
		string m_strProcName;
		// 进程ID
		DWORD m_dwPID;
		// 进程路径
		string m_strProcPath;
		// 进程版本
		string m_strProcVer;
		// 进程文件大小
		float m_fProcFileSize;
		//进程文件创建时间
		CProtocolTime	m_CreatTime;
		//进程文件修改时间
		CProtocolTime	m_ModifyTime;
		// 进程状态
		BYTE m_byProcState;
		//进程启动时间
		CProtocolTime	m_StartTime;
	};

	//==============================================================================
	//
	//             CMoniInfoAllHostProc
	//            继承监视信息基类
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoAllHostProc : public CMoniInfoBase
	{
	public:
		CMoniInfoAllHostProc();
		~CMoniInfoAllHostProc();

	public:
		// 获取进程信息个数
		int GetAllHostProcItoACount() const;
		// 获取指定下标的进程信息
		CAllHostProcItoA* GetAllHostProcItoA(int nIndex);
		// 添加进程信息
		void Add(const CAllHostProcItoA& objNetCard);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CAllHostProcItoA> m_vecAllHostProcItoA;	//进程信息
	};

	//==============================================================================
	//
	//             CHostAllItoAPkt
	//             全体主机状态信息【RmInterHost→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CHostAllItoAPkt : public CProtocolHead
	{
	public:
		CHostAllItoAPkt();
		~CHostAllItoAPkt();

		//重载运算符=，深拷贝
		CHostAllItoAPkt& operator=(const CHostAllItoAPkt& objHostAllItoAPkt);
		//拷贝构造函数，深拷贝
		CHostAllItoAPkt(const CHostAllItoAPkt& objHostAllItoAPkt);

	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;	

		// 获取监控信息个数
		int GetMoniInfoBaseCount() const;
		// 获取指定下标的监控信息
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;
		// 添加监控信息
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//添加操作系统信息
		void AddOS(string sOSName);

		// 添加CPU个数信息
		void AddCPUNum(BYTE byCPUNum);

		//添加内存容量
		void AddMEMCap(float fMEMCap);

		//添加磁盘信息
		void AddDisk(CMoniInfoDisk* pMoniInfoDisk);

		//添加网卡信息
		void AddNetCard(CMoniInfoNetCard* pMoniInfoNetCard);

		//添加进程信息
		void AddProcess(CMoniInfoAllHostProc* pProcess);

	private:
		// 主机名
		string m_strHostName;
		//监控信息
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};
	
	//==============================================================================
	//
	//             CHostCHGItoAPkt
	//             变化主机状态信息【RmInterHost→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CHostCHGItoAPkt : public CHostAllItoAPkt
	{
	public:
		CHostCHGItoAPkt();
		~CHostCHGItoAPkt();
	};

	//==============================================================================
	//
	//             CReqGrpAndHostAllCtoAPkt
	//             请求全体组及主机状态信息【RmControlMonitor→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CReqGrpAndHostAllCtoAPkt : public CProtocolHead
	{
	public:
		CReqGrpAndHostAllCtoAPkt();
		~CReqGrpAndHostAllCtoAPkt();
	};

	//==============================================================================
	//
	//             CHostAllAtoC
	//             主机状态
	//
	//==============================================================================
	class MY_EXT_CLASS CHostAllAtoC : public CObjectBase
	{
	public:
		CHostAllAtoC();
		~CHostAllAtoC();

	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;

		// 设置主机状态
		void SetHostState(BYTE byHostState);
		// 获取主机状态
		BYTE GetHostState() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 主机名
		string m_strHostName;
		// 主机状态
		BYTE m_byHostState;
	};

	//==============================================================================
	//
	//             CGroupAndHostsAtoC
	//             主机组及组内主机状态
	//
	//==============================================================================
	class MY_EXT_CLASS CGroupAndHostsAtoC : public CObjectBase
	{
	public:
		CGroupAndHostsAtoC();
		~CGroupAndHostsAtoC();
		
	public:
		// 设置主机组名
		void SetGroupName(string strGroupName);
		// 获取主机组名
		string GetGroupName() const;
		
		// 设置组状态
		void SetGrpState(BYTE byGrpState);
		// 获取组状态
		BYTE GetGrpState() const;

		// 获取主机信息个数
		int GetHostAllAtoCCount() const;
		// 获取指定下标的主机信息
		CHostAllAtoC* GetHostAllAtoC(int nIndex);
		// 根据主机名获取下标
		int GetHostIndexByName(string strName);
		// 添加主机信息
		void Add(const CHostAllAtoC& objHostAllAtoC);
		void Clear();
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机组名
		string m_strGroupName;
		// 组状态
		BYTE m_byGrpState;
		// 组内主机
		vector<CHostAllAtoC> m_vecHostAllAtoC;
	};
	
	//==============================================================================
	//
	//             CGrpAndHostAllAtoCPkt
	//             全体组及主机状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CGrpAndHostAllAtoCPkt : public CProtocolHead
	{
	public:
		CGrpAndHostAllAtoCPkt();
		~CGrpAndHostAllAtoCPkt();
		
	public:
		// 获取主机组信息个数
		int GetGroupAndHostsAtoCCount() const;
		// 获取指定下标的主机组信息
		CGroupAndHostsAtoC* GetGroupAndHostsAtoC(int nIndex);
		// 根据主机组名获取下标
		int GetHostGrpIndexByName(string strName);
		// 添加主机组信息
		void Add(const CGroupAndHostsAtoC& objGroupAndHostsAtoC);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机组及组内主机状态
		vector<CGroupAndHostsAtoC> m_vecGroupAndHostsAtoC;
	};

	//==============================================================================
	//
	//             CGroupAtoC
	//             主机组状态
	//
	//==============================================================================
	class MY_EXT_CLASS CGroupAtoC : public CObjectBase
	{
	public:
		CGroupAtoC();
		~CGroupAtoC();

	public:
		// 设置主机组名
		void SetGroupName(string strGroupName);
		// 获取主机组名
		string GetGroupName() const;

		// 设置组状态
		void SetGrpState(BYTE byGrpState);
		// 获取组状态
		BYTE GetGrpState() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 主机组名
		string m_strGroupName;
		// 组状态
		BYTE m_byGrpState;
	};

	//==============================================================================
	//
	//             CGrpCHGAtoCPkt
	//             变化组状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CGrpCHGAtoCPkt : public CProtocolHead
	{
	public:
		CGrpCHGAtoCPkt();
		~CGrpCHGAtoCPkt();

	public:
		// 获取主机组信息个数
		int GetGroupAtoCCount() const;
		// 获取指定下标的主机组信息
		CGroupAtoC* GetGroupAtoC(int nIndex);
		// 添加主机组信息
		void Add(const CGroupAtoC& objGroupAtoC);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 主机组状态
		vector<CGroupAtoC> m_vecGroupAtoC;
	};

	//==============================================================================
	//
	//             CGroupAllAtoC
	//             主机组信息
	//
	//==============================================================================
	class MY_EXT_CLASS CGroupAllAtoC : public CObjectBase
	{
	public:
		CGroupAllAtoC();
		~CGroupAllAtoC();
		
	public:
		// 设置主机组名
		void SetGroupName(string strGroupName);
		// 获取主机组名
		string GetGroupName() const;
		
		// 获取主机信息个数
		int GetHostAllAtoCCount() const;
		// 获取指定下标的主机信息
		CHostAllAtoC* GetHostAllAtoC(int nIndex);
		// 添加主机信息
		void Add(const CHostAllAtoC& objHostAllAtoC);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机组名
		string m_strGroupName;
		// 主机信息
		vector<CHostAllAtoC> m_vecHostAllAtoC;
	};
	
	//==============================================================================
	//
	//             CHostInGrpCHGAtoCPkt
	//             变化组内主机状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CHostInGrpCHGAtoCPkt : public CProtocolHead
	{
	public:
		CHostInGrpCHGAtoCPkt();
		~CHostInGrpCHGAtoCPkt();

	public:
		// 获取主机组信息个数
		int GetGroupAllAtoCCount() const;
		// 获取指定下标的主机组信息
		CGroupAllAtoC* GetGroupAllAtoC(int nIndex);
		// 添加主机组信息
		void Add(const CGroupAllAtoC& objGroupAllAtoC);
		void Clear();
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 主机组信息
		vector<CGroupAllAtoC> m_vecGroupAllAtoC;
	};
	
	//==============================================================================
	//
	//             CSglHost
	//             主机信息
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHost : public CObjectBase
	{
	public:
		CSglHost();
		~CSglHost();
		
	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机名
		string m_strHostName;
	};
	
	//==============================================================================
	//
	//             CReqSglHostAllCtoAPkt
	//             请求全体单个主机状态信息【RmControlMonitor→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CReqSglHostAllCtoAPkt : public CProtocolHead
	{
	public:
		CReqSglHostAllCtoAPkt();
		~CReqSglHostAllCtoAPkt();
		
	public:
		// 获取主机信息个数
		int GetSglHostCount() const;
		// 获取指定下标的主机信息
		CSglHost* GetSglHost(int nIndex);
		// 添加主机信息
		void Add(const CSglHost& objSglHost);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机信息
		vector<CSglHost> m_vecSglHost;
	};

	//==============================================================================
	//
	//             CSglCpuAndMem
	//             单个主机的计算机、进程的CPU、内存实时状态信息
	//
	//==============================================================================
	class MY_EXT_CLASS CSglCpuAndMem : public CObjectBase
	{
	public:
		CSglCpuAndMem();
		~CSglCpuAndMem();

		//重载运算符=，深拷贝
		CSglCpuAndMem& operator=(const CSglCpuAndMem& objSglCpuAndMem);
		//拷贝构造函数，深拷贝
		CSglCpuAndMem(const CSglCpuAndMem& objSglCpuAndMem);

	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;

		// 设置信息有效性
		void SetValidity(BYTE byValidity);
		// 获取信息有效性
		BYTE GetValidity() const;

		// 获取监控信息个数
		int GetMoniInfoBaseCount() const;
		// 获取指定下标的监控信息
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 添加监控信息
		// 添加CPU负载信息
		void AddCPULoad(const float fCPULoad);

		//添加内存信息
		void AddMEMLoad(const float fMEMLoad);

		//添加进程信息
		void AddProcess(const CMoniInfoProcess& objProcess);

	private:
		// 主机名
		string m_strHostName;
		// 信息有效性
		BYTE m_byValidity;
		//监控信息
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//             CCpuAndMemAtoCPkt
	//             计算机、进程的CPU、内存实时状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CCpuAndMemAtoCPkt : public CProtocolHead
	{
	public:
		CCpuAndMemAtoCPkt();
		~CCpuAndMemAtoCPkt();
		
	public:
		// 获取主机信息个数
		int GetHostAtoCCount() const;
		// 获取指定下标的主机信息
		CSglCpuAndMem* GetHostAtoC(int nIndex);
		// 添加主机信息
		void Add(const CSglCpuAndMem& objHostAtoC);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机信息
		vector<CSglCpuAndMem> m_vecHostAtoC; //使用5.1.3的类的内容
	};
	
	//==============================================================================
	//
	//                        CSglHostState
	//       主机状态信息。主机状态信息包括：操作系统、CPU、内存、磁盘、网络、进程信息。
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHostState : public CObjectBase
	{
	public:
		CSglHostState();
		~CSglHostState();

		//重载运算符=，深拷贝
		CSglHostState& operator=(const CSglHostState& objSglHostState);
		//拷贝构造函数，深拷贝
		CSglHostState(const CSglHostState& objSglHostAndService);

	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;

		// 设置主机描述
		void SetHostDesc(string strHostDesc);
		// 获取主机描述
		string GetHostDesc() const;

		// 设置信息有效性
		void SetValidity(BYTE byValidity);
		// 获取信息有效性
		BYTE GetValidity() const;
		
		// 获取监控信息个数
		int GetMoniInfoBaseCount() const;
		// 获取指定下标的监控信息
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;

		// 添加监控信息
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear(); 

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//添加操作系统信息
		void AddOS(const string sOSName);
		// 添加CPU个数信息
		void AddCPUNum(const BYTE byCPUNum);
		//添加内存容量
		void AddMEMCap(const float fMEMCap);
		//添加磁盘信息
		void AddDisk(const CMoniInfoDisk* pMoniInfoDisk);
		//添加网卡信息
		void AddNetCard(const CMoniInfoNetCard* pMoniInfoNetCard);
		//添加进程信息
		void AddProcess(const CMoniInfoAllHostProc* pProcess);
		
	private:
		// 主机名
		string m_strHostName;
		// 主机描述
		string m_strHostDesc;
		// 信息有效性
		BYTE m_byValidity;	
		//监控信息
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//             CSglHostAllAtoCPkt
	//             全体单个主机状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHostAllCtoAPkt : public CProtocolHead
	{
	public:
		CSglHostAllCtoAPkt();
		~CSglHostAllCtoAPkt();
		
	public:
		// 获取主机信息个数
		int GetSglHostStateCount() const;
		// 获取指定下标的主机信息
		CSglHostState* GetSglHostState(int nIndex);
		// 添加主机信息
		void Add(const CSglHostState& objSglHostState);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机信息
		vector<CSglHostState> m_vecSglHostState;
	};
	
	//==============================================================================
	//
	//             CSglHostCHGCtoAPkt
	//            变化单个主机状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHostCHGCtoAPkt : public CSglHostAllCtoAPkt
	{
	public:
		CSglHostCHGCtoAPkt();
		~CSglHostCHGCtoAPkt();

	public:
		virtual CProtocolHead* Clone();
	};

	//==============================================================================
	//
	//             CDeviceInfoEx
	//             带设备描述的设备信息
	//
	//==============================================================================
	class MY_EXT_CLASS CDeviceInfoEx : public CDeviceInfo
	{
	public:
		CDeviceInfoEx();
		~CDeviceInfoEx();

		bool operator == (CDeviceInfoEx& objOther);

	public:
		// 设置设备地址名称
		void SetDevName(const string& strName);
		// 获取设备地址名称
		string GetDevName() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strDevName;			// 设备地址名称
	};

	//==============================================================================
	//
	//             CProgramClient
	//             客户端信息类
	//
	//==============================================================================
	class MY_EXT_CLASS CProgramClient : public CForwardAddr, public CObjectBase
	{
	public:
		CProgramClient();
		~CProgramClient();

	public:
		// 设置客户端名称
		void SetCltName(const string& strName);
		// 获取客户端名称
		string GetCltName() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strCltName;				// 客户端名称
	};

	//==============================================================================
	//
	//             CClientConnMainCommtoSysEx
	//             客户端信息类
	//
	//==============================================================================
	class MY_EXT_CLASS CClientConnMainCommtoSysEx : public CObjectBase
	{
	public:
		CClientConnMainCommtoSysEx();
		~CClientConnMainCommtoSysEx();

	public:
		// 设置客户端对象
		void SetProgramClient(CProgramClient& objClt);
		// 获取客户端对象指针
		CProgramClient* GetProgramClient();

		// 设置客户端连接状态
		void SetConnState(CConnState& objConnState);
		// 获取客户端连接状态指针
		CConnState* GetConnState();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		CProgramClient	m_objClt;				// 客户端对象
		CConnState		m_objConnState;			// 客户端连接状态
	};

	//==============================================================================
	//
	//             CProgramServerEx
	//             服务程序信息类
	//
	//==============================================================================
	class MY_EXT_CLASS CProgramServerEx : public CProgramServer
	{
	public:
		CProgramServerEx();
		~CProgramServerEx();

	public:
		// 获取服务端名称
		string GetSvcName() const;
		// 设置服务端名称
		void SetSvcName(const string& strName);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strSvcName;				// 服务端名称
	};

	//==============================================================================
	//
	//			CSvrConnStateEx
	//			服务进程的连接状态
	//
	//==============================================================================
	class MY_EXT_CLASS CSvrConnStateEx : public CObjectBase
	{
	public:
		CSvrConnStateEx();
		~CSvrConnStateEx();

	public:
		// 设置服务端程序信息
		void SetProgramServer(CProgramServerEx& objSvc);
		// 获取服务端程序信息
		CProgramServerEx* GetProgramServer();

		// 设置服务端连接信息
		void SetConnInfo(CConnInfo& objConnInfo);
		// 获取服务端连接信息
		CConnInfo* GetConnInfo();

		// 清空客户端信息
		void Clear();
		// 添加客户端信息
		void AddClient(const CClientConnMainCommtoSysEx& objClient);
		// 获得客户端数量
		WORD GetClientCount() const;
		// 获得指定下标的客户端信息
		CClientConnMainCommtoSysEx* GetClient(int nIndex);
		// 根据客户端地址客户端信息
		CClientConnMainCommtoSysEx* GetClientByAddr(CForwardAddr& objAddr);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		CProgramServerEx m_objSvc;
		CConnInfo m_objConnInfo;
		vector<CClientConnMainCommtoSysEx> m_vecClientConnMainCommtoSys;
	};

	//==============================================================================
	//
	//			CReqSglSysAllCtoAPkt
	//			请求全体单个主机主系统状态信息【RmControlMonitor→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CReqSglSysAllCtoAPkt : public CReqSglHostAllCtoAPkt
	{
	public:
		CReqSglSysAllCtoAPkt();
		~CReqSglSysAllCtoAPkt();
	};

	//==============================================================================
	//
	//			CMoniInfoStandbyState
	//			服务主备状态
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoStandbyState : public CMoniInfoBase
	{
	public:
		CMoniInfoStandbyState();
		~CMoniInfoStandbyState();

	public:
		// 设置服务主备状态
		void SetState(BYTE byActStdbySta);
		// 获取服务主备状态
		BYTE GetState() const;

		// 主备状态改变时间
		void SetChangeTime(time_t tm, WORD wMs = 0);	//	wMs：毫秒（0~999）

		void SetChangeProtocolTime(CProtocolTime* tTime);
		CProtocolTime* GetChangeProtocolTime();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		BYTE m_byState;						// 服务主备状态
		CProtocolTime m_ChangeTime;			// 主备状态改变时间
	};

	//==============================================================================
	//
	//			CMoniInfoSvrConnState
	//			服务连接状态
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoSvrConnState : public CMoniInfoBase
	{
	public:
		CMoniInfoSvrConnState();
		~CMoniInfoSvrConnState();

	public:
		// 获取服务端个数
		int GetSvrConnStateExCount() const;
		// 获取指定下标的服务端信息
		CSvrConnStateEx* GetSvrConnStateEx(int nIndex);
		// 添加服务端信息
		void Add(const CSvrConnStateEx& objSvrConnStateEx);
		// 根据服务端ID获取服务连接状态
		CSvrConnStateEx* GetSvrConnStateBySvrId(WORD wSvrId);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//服务连接状态
		vector<CSvrConnStateEx> m_vecSvrConnStateEx;
	};

	//==============================================================================
	//
	//             CMoniInfoStaChannelState
	//                  站通道状态
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoStaChannelState : public CMoniInfoBase
	{
	public:
		CMoniInfoStaChannelState();
		~CMoniInfoStaChannelState();

		// 重载运算符=，深拷贝
		CMoniInfoStaChannelState& operator=(const CMoniInfoStaChannelState& DeviceSysState);
		// 拷贝构造函数，深拷贝
		CMoniInfoStaChannelState(const CMoniInfoStaChannelState& objDeviceSysState);

	public:
		// 获取车站个数
		int GetStaCommDevCount() const;
		// 获取指定下标的站通道信息
		CStaCommDev* GetStaCommDev(int nIndex);
		// 添加站通道信息
		void Add(const CStaCommDev& objStaCommDev);
		vector<CStaCommDev*>& GetAllStaCommDev();
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//服务连接状态
		vector<CStaCommDev*> m_vecStaCommDev;
	};
	
	//==============================================================================
	//
	//								CDeviceSysState
	//             设备主系统状态信息（主备状态、服务连接状态、站通信状态）
	//
	//==============================================================================
	class MY_EXT_CLASS CDeviceSysState : public CObjectBase
	{
	public:
		CDeviceSysState();
		~CDeviceSysState();

		// 重载运算符=，深拷贝
		CDeviceSysState& operator=(const CDeviceSysState& DeviceSysState);
		// 拷贝构造函数，深拷贝
		CDeviceSysState(const CDeviceSysState& objDeviceSysState);

	public:
		// 设置设备地址信息
		void SetDeviceInfo(CDeviceInfoEx& objDevInfo);
		CDeviceInfoEx* GetDeviceInfo();
		CDeviceInfoEx const* GetDeviceInfo() const;

		// 设置信息有效性
		void SetValidity(BYTE byValidity);
		// 获取信息有效性
		BYTE GetValidity() const;

		// 获取监控信息个数
		int GetMoniInfoCount() const;
		// 获取指定下标的监控信息
		CMoniInfoBase* GetMoniInfo(int nIndex);
		// 添加监控信息
		void AddMoniInfo(const CMoniInfoBase* pMoniInfoBase);

		void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//添加服务器主备状态
		void AddSvrActStbyState(const CMoniInfoStandbyState* pMoniSvcActStdbySta);
		//添加服务连接状态
		void AddSvcConnState(const CMoniInfoSvrConnState* pSvrConnState);
		//添加通道状态
		void AddChanState(const CMoniInfoStaChannelState* pStaCommDev);

	private:
		//设备地址信息
		CDeviceInfoEx m_objDeviceInfoEx;
		//信息有效性
		BYTE m_byValidity;
		//监控信息
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//             CSglSysAll
	//             单个主机主系统信息
	//
	//==============================================================================
	class MY_EXT_CLASS CSglSysAll : public CObjectBase
	{
	public:
		CSglSysAll();
		~CSglSysAll();

	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;

		// 设置主机描述
		void SetHostDesc(string strHostDesc);
		// 获取主机描述
		string GetHostDesc() const;

		// 获取地址个数
		int GetDeviceSysStateCount() const;
		// 获取指定下标的地址信息
		CDeviceSysState* GetDeviceSysState(int nIndex);
		// 添加地址信息
		void Add(const CDeviceSysState& objDeviceSysState);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 主机名
		string m_strHostName;
		// 主机描述
		string m_strHostDesc;

		// 设备主系统状态信息
		vector<CDeviceSysState> m_vecDeviceSysState;
	};

	//==============================================================================
	//
	//             CSglSysAllAtoCPkt
	//             全体单个主机主系统状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CSglSysAllAtoCPkt : public CProtocolHead
	{
	public:
		CSglSysAllAtoCPkt();
		~CSglSysAllAtoCPkt();

	public:
		// 获取主机个数
		int GetSglSysAllCount() const;
		// 获取指定下标的主机信息
		CSglSysAll* GetSglSysAll(int nIndex);
		// 添加主机信息
		void Add(CSglSysAll& objSglSysAll);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CSglSysAll> m_vecSglSysAll;
	};

	//==============================================================================
	//
	//             CSglSysCHGAtoCPkt
	//            变化单个主机主系统状态信息【RmAppHostMgr→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CSglSysCHGAtoCPkt : public CSglSysAllAtoCPkt
	{
	public:
		CSglSysCHGAtoCPkt();
		~CSglSysCHGAtoCPkt();
	};
}

#endif
