//=========================================================================================
//
//							 ProtocolMaintenComm.h
// begin    : 2016-12-16
// describe : 系统状态信息协议集服务连接状态部分，供通讯平台使用
//
//=========================================================================================
#ifndef _PROTOCOLMAINTENCOMM_H_2016_12_20
#define _PROTOCOLMAINTENCOMM_H_2016_12_20

#include <vector>
#include "TypeDef.h"
#include "ProtocolBase.h"
#include "ProtocolHead.h"
#include "ProtocolForward.h"

typedef CProtocolForward::CForwardAddr CForwardAddr;

namespace ProtocolMaintenContent
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//             CDeviceInfo
	//             设备信息
	//
	//==============================================================================
	class MY_EXT_CLASS CDeviceInfo : public CObjectBase
	{
	public:
		CDeviceInfo();
		~CDeviceInfo();

		bool operator == (const CDeviceInfo& objOther);

	public:
		// 设置路局Id
		void SetBureauId(BYTE byBureauId);
		// 获取路局Id
		BYTE GetBureauId() const;

		// 设置单位类型
		void SetUnitType(BYTE byUnitType);
		// 获取单位类型
		BYTE GetUnitType() const;

		// 设置单位Id
		void SetUnitId(WORD wUnitId);
		// 获取单位Id
		WORD GetUnitId() const;

		// 设置设备类型
		void SetDeviceType(BYTE byDeviceType);
		// 获取设备类型
		BYTE GetDeviceType() const;

		// 设置设备Id
		void SetDeviceId(WORD wDeviceId);
		// 获取设备Id
		WORD GetDeviceId() const;

		// 设置AB机标识
		void SetAbFlag(BYTE byAb);
		// 获取AB机标识
		BYTE GetAbFlag() const;

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		string GetDumpStr();

	private:
		BYTE m_byBureauId;				// 路局Id
		BYTE m_byUnitType;				// 单位类型
		WORD m_wUnitId;					// 单位Id
		BYTE m_byDeviceType;			// 设备类型
		WORD m_wDeviceId;				// 设备Id
		BYTE m_byAbFlag;				// Ab机标识
	};

	//==============================================================================
	//
	//			CConnState
	//			客户端连接状态
	//
	//==============================================================================
	class MY_EXT_CLASS CConnState : public CObjectBase
	{
	public:
		CConnState();
		~CConnState();

		bool operator == (const CConnState& objOther);

	public:
		//ip1地址
		void SetIP1(const string& strIP1);
		string GetIP1() const ;
		//网络1端口
		void SetNet1Port(WORD wNet1Port);
		WORD GetNet1Port() const;
		//网络1连接状态
		void SetNet1Conn(BYTE byNet1Conn);
		BYTE GetNet1Conn() const;
		//网络1连接状态改变时间
		void SetNet1ConnStaChgTime(time_t& tm, WORD wMs);	//	wMs：毫秒（0~999）
		void SetNet1ConnStaChgTime(const CProtocolTime& objChgTime);
		CProtocolTime GetNet1ConnStaChgTime() const;

		//ip2地址
		void SetIP2(const string& strIP2);
		string GetIP2() const ;
		//网络2端口
		void SetNet2Port(WORD wNet2Port);
		WORD GetNet2Port() const;
		//网络2连接状态
		void SetNet2Conn(BYTE byNet2Conn);
		BYTE GetNet2Conn() const;
		//网络2连接状态改变时间
		void SetNet2ConnStaChgTime(time_t& tm, WORD wMs);	//	wMs：毫秒（0~999）
		void SetNet2ConnStaChgTime(const CProtocolTime& objChgTime);
		CProtocolTime GetNet2ConnStaChgTime() const;

		//AF_UNIX连接状态
		void SetAF_UNIXState(BYTE BYAF_UNIXState);
		BYTE GetAF_UNIXState() const;

		// AF_UNIX连接状态改变时间
		void SetAF_UNIXChgTime(time_t& tm, WORD wMs);	//	wMs：毫秒（0~999）
		void SetAF_UNIXChgTime(const CProtocolTime& objChgTime);
		CProtocolTime GetAF_UNIXChgTime() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strIP1;					// IP1地址
		WORD m_wNet1Port;					// 网络1端口
		BYTE m_byNet1Conn;					// 网络1连接状态
		CProtocolTime m_Net1ConnStaChgTime;	// 网络1连接状态改变时间
		string m_strIP2;					// IP2地址
		WORD m_wNet2Port;					// 网络2端口
		BYTE m_byNet2Conn;					// 网络2连接状态
		CProtocolTime m_Net2ConnStaChgTime;	// 网络2连接状态改变时间
		BYTE m_byAF_UNIXState;				// AF_UNIX连接状态
		CProtocolTime m_AF_UNIXChgTime;		// AF_UNIX连接状态改变时间
	};

	//==============================================================================
	//
	//			CClientConnMainCommtoSys
	//			客户端连接信息
	//
	//==============================================================================
	class MY_EXT_CLASS CClientConnMainCommtoSys : public CObjectBase
	{
	public:
		CClientConnMainCommtoSys();
		~CClientConnMainCommtoSys();

	public:
		// 设置客户端设备信息和客户端ID
		void SetForwardAddr(const CForwardAddr& objForwardAddr);
		// 获取客户端设备信息和客户端ID
		CForwardAddr* GetForwardAddr();
		// 设置客户端连接状态
		void SetConnState(const CConnState& objConnState);
		// 获取客户端连接状态
		CConnState* GetConnState();

	public:
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		CForwardAddr m_objForwardAddr;		// 局码 单位类型 单位id 设备类型 设备ID 客户端id
		CConnState m_objConnState;			// 连接状态
	};

	//==============================================================================
	//
	//             CConnInfo
	//             服务进程连接地址信息类
	//
	//==============================================================================
	class MY_EXT_CLASS CConnInfo : public CObjectBase
	{
	public:
		CConnInfo();
		~CConnInfo();

		bool operator != (const CConnInfo& objOther);

	public:
		// 获取连接类型
		BYTE GetConnType() const;
		// 设置连接类型
		void SetConnType(BYTE byType);

		// 获取Ip1地址
		string GetIp1() const;
		// 设置Ip1地址
		void SetIp1(const string& strIp);

		// 获取网络1端口
		WORD GetNet1Port() const;
		// 设置网络1端口
		void SetNet1Port(WORD wPort);

		// 获取Ip2地址
		string GetIp2() const;
		// 设置Ip2地址
		void SetIp2(const string& strIp);

		// 获取网络2端口
		WORD GetNet2Port() const;
		// 设置网络2端口
		void SetNet2Port(WORD wPort);

		// 获取AF_UNIX连接路径
		string GetConnPath() const;
		// 设置AF_UNIX连接路径
		void SetConnPath(const string& strPath);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		string GetDumpStr();

	private:
		// 连接类型
		BYTE m_byConnType;			
		//IP1地址
		string m_strIP1;
		//网络1端口
		WORD m_wNet1Port;
		//IP2地址
		string m_strIP2;
		//网络2端口
		WORD m_wNet2Port;
		// AF_UNIX连接路径
		string m_strConnPath;
	};

	//==============================================================================
	//
	//             CProgramServer
	//             服务程序信息类
	//
	//==============================================================================
	class MY_EXT_CLASS CProgramServer : public CObjectBase
	{
	public:
		CProgramServer();
		~CProgramServer();

		bool operator == (const CProgramServer& objOther);

	public:
		// 获取服务端Id
		WORD GetSvcID() const;
		// 设置服务端Id
		void SetSvcID(WORD wId);

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wSvcID;					// 服务端Id
	};

	//==============================================================================
	//
	//			CSvrConnState
	//			服务进程的连接状态
	//
	//==============================================================================
	class MY_EXT_CLASS CSvrConnState : public CObjectBase
	{
	public:
		CSvrConnState();
		~CSvrConnState();

	public:
		// 设置服务端程序信息
		void SetProgramServer(CProgramServer& objSvc);
		// 获取服务端程序信息
		CProgramServer* GetProgramServer();

		// 设置服务端连接信息
		void SetConnInfo(CConnInfo& objConnInfo);
		// 获取服务端连接信息
		CConnInfo* GetConnInfo();
		
		// 清空客户端信息
		void Clear();
		// 添加客户端信息
		void AddClient(const CClientConnMainCommtoSys& objClient);
		// 获得客户端数量
		WORD GetClientCount() const;
		// 获得指定下标的客户端信息
		CClientConnMainCommtoSys* GetClient(int nIndex);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		vector<CClientConnMainCommtoSys>& GetClientVector();

	private:
		CProgramServer m_objSvc;
		CConnInfo m_objConnInfo;
		vector<CClientConnMainCommtoSys> m_vecClientConnMainCommtoSys;
	};

	//==============================================================================
	//
	//			CMainSysSvcConnStatusPkt
	//			主系统服务连接状态【主系统通信平台→RmInterSys】
	//
	//==============================================================================
	class MY_EXT_CLASS CMainSysSvcConnStatusPkt : public CProtocolHead
	{
	public:
		CMainSysSvcConnStatusPkt();
		~CMainSysSvcConnStatusPkt();

	public:
		// 设置服务器的地址信息
		void SetDeviceInfo(const CDeviceInfo& objDeviceInfo);
		// 获取服务器的地址信息
		CDeviceInfo* GetDeviceInfo();
		// 设置服务的连接状态
		void SetSvrConnState(const CSvrConnState& objSvrConnState);
		// 获取服务的连接状态
		CSvrConnState* GetSvrConnState();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 局码\单位类型\单位ID\设备类型\设备ID\AB机标识
		CDeviceInfo m_objDeviceInfo;
		// 服务连接状态
		CSvrConnState m_objSvrConnSt;
	};
}

#endif	// !_PROTOCOLMAINTENCOMM_H_2016_12_20
