//=========================================================================================
//
//							 ProtocolMaintenSysInfo.h
// begin    : 2016-12-16
// describe : 系统状态信息协议集-从RmInterSys发送到RmAppHostMgr
//
//=========================================================================================
#ifndef _PROTOCOLMAINTENSYSINFO_H_2016_12_16
#define _PROTOCOLMAINTENSYSINFO_H_2016_12_16

#include <vector>
#include "TypeDef.h"
#include "ProtocolBase.h"
#include "ProtocolHead.h"
#include "ProtocolDevState.h"
#include "ProtocolAlarmInfo.h"
#include "ProtocolMaintenComm.h"
#include "ProtocolMaintenContent.h"

namespace ProtocolMaintenContent
{
	using namespace std;
	using namespace ProtocolBase;
	using namespace ProtocolAlarm;
	using namespace ProtocolIndication;

	//==============================================================================
	//
	//			CMoniInfoSvrConnStateItoA
	//				 服务连接状态
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoSvrConnStateItoA : public CMoniInfoBase
	{
	public:
		CMoniInfoSvrConnStateItoA();
		~CMoniInfoSvrConnStateItoA();

	public:
		// 获取服务端个数
		int GetSvrConnStateCount() const;
		// 获取指定下标的服务端信息
		CSvrConnState* GetSvrConnState(int nIndex);
		// 添加服务端信息
		void Add(const CSvrConnState& objSvrConnState);
		// 根据服务端ID获取服务连接状态
		CSvrConnState* GetSvrConnStateBySvrId(WORD wSvrId);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//服务连接状态
		vector<CSvrConnState> m_vecSvrConnState;
	};

	//==============================================================================
	//
	//				CSvrAllInfo
	//				服务器信息类
	//
	//==============================================================================
	class MY_EXT_CLASS CSvrAllInfo : public CObjectBase
	{
	public:
		CSvrAllInfo();
		~CSvrAllInfo();

		// 重载运算符=，深拷贝
		CSvrAllInfo& operator=(const CSvrAllInfo& objSvrAllInfo);
		// 拷贝构造函数，深拷贝
		CSvrAllInfo(const CSvrAllInfo& objSvrAllInfo);

	public:
		// 设置服务器设备信息
		void SetDevice(CDeviceInfo& objDevice);
		// 获取服务器设备信息
		CDeviceInfo* GetDevice();

		// 设置信息有效性
		void SetValidity(BYTE byFault);
		// 获取信息有效性
		BYTE GetValidity() const;

		// 获取监控信息个数
		int GetMoniInfoCount() const;
		// 获取指定下标的监控信息
		CMoniInfoBase* GetMoniInfo(int nIndex) const;
		// 添加监控信息
		void AddMoniInfo(const CMoniInfoBase* pMoniInfoBase);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// 添加服务器主备状态
		void AddSvrActStbyState(const CMoniInfoStandbyState* pMoniSvcActStdbySta);
		// 添加服务连接状态
		void AddSvcConnState(const CMoniInfoSvrConnStateItoA* pSvrConnState);
		// 添加通道状态
		void AddChanState(CMoniInfoStaChannelState* pStaCommDev);

		// 释放内存
		void Clear();

	private:
		// 服务器设备信息
		CDeviceInfo m_objDevice;
		// 信息有效性
		BYTE m_byValidity;
		// 监控信息
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//				CMainSysAllInfoPkt
	//				全体主系统状态信息【RmInterSys→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMainSysAllInfoPkt : public CProtocolHead
	{
	public:
		CMainSysAllInfoPkt();
		~CMainSysAllInfoPkt();

	public:
		// 清除全部服务器信息
		void ClearAllSvr();
		// 添加服务器信息
		void AddSvr(const CSvrAllInfo& objSvr);
		// 获取服务端的个数
		BYTE GetSvrCount() const;
		// 获得指定下标的服务端对象指针
		CSvrAllInfo* GetSvr(int nIndex);

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CSvrAllInfo*> m_vecSvr;		// 服务器列表
	};

	//==============================================================================
	//
	//				CMainSysChangedInfoPkt
	//				变化主系统状态信息【RmInterSys→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMainSysChangedInfoPkt : public CMainSysAllInfoPkt
	{
	public:
		CMainSysChangedInfoPkt();
		~CMainSysChangedInfoPkt();
	};

	//==============================================================================
	//
	//				CMainSysAlarmPkt
	//				主系统报警信息【RmInterSys→RmAppHostMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMainSysAlarmPkt : public CProtocolHead
	{
	public:
		CMainSysAlarmPkt();
		~CMainSysAlarmPkt();

	public:
		// 设置服务器主机名
		void SetName(const string& strName);
		// 获取服务器主机名
		string GetName() const;

		// 获取设备信息
		CDeviceInfo* GetDevice();

		// 获取报警个数
		int GetAlarmCount() const;
		// 获取指定下标的报警信息
		CAlarmAllEvt* GetAlarm(int nIndex);
		// 添加报警信息
		void Add(const CAlarmAllEvt& pMoniInfoBase);
		// 清除报警信息
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strHostName;				// 服务器主机名
		CDeviceInfo m_objDecive;			// 设备信息
		vector<CAlarmAllEvt> m_vecAlarm;	// 报警列表
	};
}

#endif	// !_PROTOCOLMAINTENSYSINFO_H_2016_12_16
