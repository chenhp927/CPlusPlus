//=========================================================================================
//
//							 ProtocolSysUpdate.h
// begin    : 2016-12-27
// describe : 系统升级协议类
//
//=========================================================================================
#ifndef _PROTOCOLSYSUPDATE_H_
#define _PROTOCOLSYSUPDATE_H_

#include "ProtocolHead.h"
#include "ProtocolTime.h"
#include "ProtocolBase.h"
#include "ObjectBase.h"
#include "TypeDef.h"
#include <vector>

namespace ProtocolSysUpdate
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//             CReqSysCurVerPkt
	//             请求主机当前软件数据版本【RmControlMonitor→RmInterHost】
	//
	//==============================================================================
	class MY_EXT_CLASS CReqSysCurVerPkt : public CProtocolHead
	{
	public:
		CReqSysCurVerPkt();
		~CReqSysCurVerPkt();
		
	public:
		// 设置主机名
		void SetHostName(string strHostName);
		// 获取主机名
		string GetHostName() const;		
		// 增加路径
		void AddRoute(string strRoute);
		// 获取路径
		string GetRoute(int nIndex) const;
		// 路径数量
		int GetRouteCount() const;

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// 主机名
		string m_strHostName;
		// 路径结构
		vector<string> m_vecRoute;
	};

	//==============================================================================
	//
	//								CRouteVerInfo
	//             主机软件数据当前版本信息【RmInterHost →RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteVerInfo : public CObjectBase
	{
	public:
		CRouteVerInfo();
		~CRouteVerInfo();

	public:		
		void SetRouteName(string strRouteName);	// 设置路径名称		
		string GetRouteName() const;			// 获取路径名称

		void SetVerID(DWORD dwVerID);			// 设置版本号
		DWORD GetVerID() const;					// 获取版本号

		void SetLogInfo(string strLog);			// 设置日志信息
		string GetLogInfo() const;				// 获取日志信息
		
		void SetTime(time_t tm, WORD wMs);		// 设置发生时间，wMs：毫秒（0~999）
		void SetTime(timeval t);
		CProtocolTime GetProCommitTime() const;	// 获取确认时间
		timeval GetCommitTime() const;			// 获取确认时间

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string	m_strRouteName;		// 路径名称
		DWORD	m_dwVerID;			// 版本号
		string	m_strLogInfo;		// 日志信息
		CProtocolTime m_Time;		// 更新或提交时间
	};

	//==============================================================================
	//
	//								CSysCurVerInfoPkt
	//             主机软件数据当前版本信息【RmInterHost →RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CSysCurVerInfoPkt : public CProtocolHead
	{
	public:
		CSysCurVerInfoPkt();
		~CSysCurVerInfoPkt();
		
	public:
		void SetHostName(string strHostName);	// 设置主机名称		
		string GetHostName() const;				// 获取主机名称

		void AddRoute(CRouteVerInfo& objRouteVerInfo); // 增加路径信息
		CRouteVerInfo* GetRoute(int nIndex);		   // 获取路径信息		
		int GetRouteCount();						   // 获取路径数量
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		string m_strHostName; // 主机名称
		vector<CRouteVerInfo> m_vecRouteInfo; // 路径信息
	};
	

	//==============================================================================
	//
	//						CRouteUpdateInfo
	//             更新主机软件数据版本【RmControlMonitor→RmInterHost】
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteUpdateInfo : public CObjectBase
	{
	public:
		CRouteUpdateInfo();
		~CRouteUpdateInfo();
	public:
		void SetRouteName(string strRouteName);	// 设置路径名称
		string GetRouteName() const;			// 获取路径名称

		void SetVerID(DWORD dwVeID);			// 设置版本号
		DWORD GetVerID() const;					// 获取版本号

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	private:
		string	m_strRouteName;	// 路径名称
		DWORD	m_dwVerID;		// 版本号
	};

	//==============================================================================
	//
	//						CHostUpdateInfo
	//             更新主机软件数据版本【RmControlMonitor→RmInterHost】主机信息
	//
	//==============================================================================
	class MY_EXT_CLASS CHostUpdateInfo : public CObjectBase
	{
	public:
		CHostUpdateInfo();
		~CHostUpdateInfo();
		
	public:
		void SetHostName(string strHostName); // 设置主机名称
		string GetHostName() const;			  // 获取主机名称

		void AddRouteUpdateInfo(CRouteUpdateInfo objRouteUpdateInfo); // 增加路径更新信息
		CRouteUpdateInfo*  GetRouteUpdateInfo(int nIndex);  // 获取路径更新信息	
		int GetRouteCount() const;							// 获取路径数量

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		string m_strHostName;	//主机名称
		vector<CRouteUpdateInfo> m_vecRouteUpdateInfo; // 路径更新信息
	};
	
	//==============================================================================
	//
	//						CSysUpdatePkt
	//             更新主机软件数据版本【RmControlMonitor→RmInterHost】
	//
	//==============================================================================
	class MY_EXT_CLASS CSysUpdatePkt : public CProtocolHead
	{
	public:
		CSysUpdatePkt();
		~CSysUpdatePkt();

	public:
		void SetUpdatePara(BYTE byUpdatePara);						// 设置更新参数
		BYTE GetUpdatePara() const;									// 获取更新参数

		void AddHostUpdateInfo(CHostUpdateInfo& objHostUpdateInfo);	// 增加主机更新信息
		CHostUpdateInfo*  GetHostUpdateInfo(int nIndex);			// 获取主机更新信息		
		int GetHostCount() const;									// 获取主机数量

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:

		BYTE m_byUpdatePara;							//更新参数
		vector<CHostUpdateInfo> m_vecHostUpdateInfo;	// 主机信息
	};

	//==============================================================================
	//
	//						CRouteUpdateRep
	//             更新主机软件数据版本回执【RmInterHost→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteUpdateRep : public CObjectBase
	{
	public:
		CRouteUpdateRep();
		~CRouteUpdateRep();

	public:		
		void SetRouteName(string strRouteName); // 设置路径名称
		string GetRouteName() const;		    // 获取路径名称

		void SetRsltValue(BYTE byRsltValue);	// 设置更新结果
		BYTE GetRsltValue() const;				// 获取更新结果

		void SetVerID(DWORD dwVerID);			// 设置版本号
		DWORD GetVerID() const;					// 获取版本号
		

		void SetRsltString(string strRslt);		// 设置回执内容
		string  GetRsltString() const;			// 获取回执内容

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:		
		string	m_strRouteName;		// 路径名称
		BYTE	m_byRslt;			// 更新结果
		DWORD	m_dwVerID;			// 版本号
		string  m_strRslt;			// 回执内容 
	};

	//==============================================================================
	//
	//							CHostUpdateRepPkt
	//             更新主机软件数据版本回执【RmInterHost→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CHostUpdateRepPkt : public CProtocolHead
	{
	public:
		CHostUpdateRepPkt();
		~CHostUpdateRepPkt();
		
	public:		
		void SetHostName(string strHostName); // 设置主机名	
		string GetHostName() const;			  // 获取主机名	
		
		CRouteUpdateRep* GetRouteUpdateReq(int nIndex);				// 获取指定下标的路径更新回执		
		void AddRouteUpdateRep(const CRouteUpdateRep& objRouteUdateRep);	// 添加路径更新回执
		int GetRouteCount();												// 路径数量
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:		
		string m_strHostName; // 主机名		
		vector<CRouteUpdateRep> m_vecRouteUpdateRep;// 路径更新回执信息
	};

	
	
	//==============================================================================
	//
	//							CVerInfo
	//          存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	class MY_EXT_CLASS CVerInfo : public CObjectBase
	{
	public:
		CVerInfo();
		~CVerInfo();

	public:
		void SetVerID(DWORD dwVerID); // 设置版本号
		DWORD GetVerID() const;		  // 获取版本号

		void SetUpdateTime(time_t tm, WORD wMs);	// 设置更新时间，wMs：毫秒（0~999）
		void SetUpdateTime(timeval t);				// 设置更新时间
		CProtocolTime GetUpateTime() const;				// 获取更新时间，返回ProtocolTime对象
		timeval GetUpdateTimeVal() const;					// 获取更新时间，返回timeval对象

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		DWORD m_dwVerID;			// 版本号
		CProtocolTime m_tmUpdate;	// 更新时间		

	};

	//==============================================================================
	//
	//							CRouteHisVerInfo
	//          存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteHisVerInfo : public CObjectBase
	{
	public:
		CRouteHisVerInfo();
		~CRouteHisVerInfo();

	public:
		void SetRouteName(string strRouteName); // 设置路径名称
		string GetRouteName() const;		    // 获取路径名称

		void AddVerInfo(CVerInfo objVerInfo);	// 增加历史版本
		CVerInfo* GetVerInfo(int nIndex);		// 历史版本数据
		int GetVerCount();						// 版本数量
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		string m_strRouteName;					// 路径名称
		vector<CVerInfo> m_vecHisVerInfo;		// 历史版本

	};

	//==============================================================================
	//
	//							CSaveHostVerPkt
	//				存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	class MY_EXT_CLASS CSaveHostVerPkt : public CProtocolHead
	{
	public:
		CSaveHostVerPkt();
		~CSaveHostVerPkt();
	public:
		void SetHostName(string strHostName); // 设置主机名	
		string GetHostName() const;			  // 获取主机名	
		
		void AddRouteInfo(const CRouteHisVerInfo objRouteHisVerInfo);	// 添加路径信息
		CRouteHisVerInfo* GetRouteInfo(int nIndex);			// 获取路径信息
		int GetRouteCount();												// 路径数量
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		string m_strHostName; // 主机名				
		vector<CRouteHisVerInfo> m_vecRouteHisVer;
	};

	//==============================================================================
	//
	//							CSaveHostVerRepPkt
	//				主机软件数据版本存储回执【工程数据存取服务→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CSaveHostVerRepPkt : public CProtocolHead
	{
	public:
		CSaveHostVerRepPkt();
		~CSaveHostVerRepPkt();
	public:
		void SetRslt(DWORD dwRslt);						// 设置结果
		DWORD GetRslt() const;							// 获取路径信息

		void SetRsn(string strRsn);						// 设置失败原因
		string GetRsn() const;							// 获取失败原因

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		DWORD m_dwSaveRslt;		// 保存结果
		string m_strRsn;		// 失败原因内容
	};


	//==============================================================================
	//
	//				            CReqHostHisVerPkt
	//            请求主机软件数据历史版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	class MY_EXT_CLASS CReqHostHisVerPkt : public CProtocolHead
	{
	public:
		CReqHostHisVerPkt();
		~CReqHostHisVerPkt();

	public:			
		void SetHostName(string strHostName);		// 设置主机名称
		string GetHostName() const;					// 获取主机名称

		string GetRouteName(int nIndex) const;		// 获取指定下标的路径名称		
		void AddRouteName(string& strRouteName);	// 添加路径名称

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strHostName;				// 主机名
		vector<string> m_vecRouteName;		// 主机路径信息
	};

	//==============================================================================
	//
	//							CHostHisVerInfoPkt
	//          主机软件数据历史版本信息【工程数据存取服务→RmControlMonitor】
	//
	//==============================================================================
	class MY_EXT_CLASS CHostHisVerInfoPkt : public CSaveHostVerPkt
	{
	public:
		CHostHisVerInfoPkt();
		~CHostHisVerInfoPkt();
	};
	
}

#endif
