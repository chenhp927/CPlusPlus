//===================================================================================================
//	
//							 ProtocolDevState.h
//							   
//begin	  : 2016-09-18
//describe: 协议设备状态类
//
//===================================================================================================
#ifndef _PROTOCOL_DEV_STATE_H_
#define _PROTOCOL_DEV_STATE_H_

#include "TypeDef.h"
#include "ObjectBase.h"
#include "ProtocolHead.h"
#include "ProtocolTime.h"
#include "ProtocolMeasure.h"
#include <vector>

namespace ProtocolIndication
{
	using namespace ProtocolBase;

	//===============================================================================================
	//	
	//	CReqStaCommDevPkt
	//	请求站通信设备状态包【其他模块→InterSvr104/101】				   
	//
	//===============================================================================================
	class MY_EXT_CLASS CReqStaCommDevPkt : public CProtocolHead
	{
	public:
		CReqStaCommDevPkt();
		virtual ~CReqStaCommDevPkt();

	public:
		void Add(WORD wStaId);										// 添加车站Id
		WORD GetStaIdCount() const;									// 获取车站数量
		WORD GetStaId(int nIndex) const;							// 获取指定下标的车站Id								

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<WORD> m_vecStaId;									// 车站Id
	};

	//===============================================================================================
	//	
	//	CStaCommDev
	//	站通信设备状态【InterSvr104/101→其他模块】				   
	//
	//===============================================================================================
	class MY_EXT_CLASS CStaCommDev : public CObjectBase
	{
	public:
		CStaCommDev();
		virtual ~CStaCommDev();

		bool operator == (const CStaCommDev& objOther);

	public:
		void SetStaId(WORD wStaId);								 	// 设置车站ID
		void SetStaName(string strStaName);							// 设置车站名称
		void SetABFlag(BYTE byABFlag);							 	// 设置AB机标识
		void SetMainStandby1(BYTE byMainStandby);				 	// 设置车站通道1主备状态
		void SetMainStandby2(BYTE byMainStandby);				 	// 设置车站通道2主备状态
		void SetChannel1(BYTE byChannel1);						 	// 设置站通道1通信状态
		void SetChn1Ip1(string strIp);								// 设置站通道1Ip1
		void SetChn1Ip2(string strIp);								// 设置站通道1Ip2
		void SetChannel2(BYTE byChannel2);						 	// 设置站通道2通信状态
		void SetChn2Ip1(string strIp);								// 设置站通道2Ip1
		void SetChn2Ip2(string strIp);								// 设置站通道2Ip2
																 	
		void SetChannel1ChgTime(time_t t, WORD wMS);			 	// 设置站通道1通信状态变化时间
		void SetChannel1ChgTime(timeval t);						 	// 设置站通道1通信状态变化时间
																 	  
		void SetChannel2ChgTime(time_t t, WORD wMs);			 	// 设置站通道2通信状态变化时间
		void SetChannel2ChgTime(timeval t);						 	// 设置站通道2通信状态变化时间

		WORD GetStaId() const;									 	// 获取车站ID	
		string GetStaName() const;									// 获取车站名称
		BYTE GetABFlag() const;									 	// 获取AB机标识
		BYTE GetMainStandby1() const;							 	// 获取车站通道1主备状态
		BYTE GetMainStandby2() const;							 	// 获取车站通道2主备状态
		BYTE GetChannel1() const;								 	// 获取站通道1通信状态
		string GetChn1Ip1() const;									// 获取站通道1Ip1
		string GetChn1Ip2() const;									// 获取站通道1Ip2
		BYTE GetChannel2() const;								 	// 获取站通道2通信状态
		string GetChn2Ip1() const;									// 获取站通道2Ip1
		string GetChn2Ip2() const;									// 获取站通道2Ip2

		CProtocolTime GetChannel1ProChgTime() const;				// 获取站通道1通信状态变化时间
		time_t GetChannel1ChgTime() const;						 	// 获取站通道1通信状态变化时间

		CProtocolTime GetChannel2ProChgTime() const;			 	// 获取站通道2通信状态变化时间
		time_t GetChannel2ChgTime() const;						 	// 获取站通道2通信状态变化时间

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;											 	// 车站ID	
		string m_strStaName;										// 车站名称
		BYTE m_byABFlag;										 	// AB机标识
		BYTE m_byMainStandby1;									 	// 车站通道1主备状态
		BYTE m_byMainStandby2;									 	// 车站通道2主备状态
		BYTE m_byChannel1;										 	// 站通道1通信状态
		string m_strChn1Ip1;										// 站通道1Ip1
		string m_strChn1Ip2;										// 站通道1Ip2
		CProtocolTime m_Channel1ChgTime;						 	// 站通道1通信状态变化时间
		BYTE m_byChannel2;										 	// 站通道2通信状态
		string m_strChn2Ip1;										// 站通道2Ip1
		string m_strChn2Ip2;										// 站通道2Ip2
		CProtocolTime m_Channel2ChgTime;						 	// 站通道2通信状态变化时间
	};

	//===============================================================================================
	//	
	//	CStaCommDevPkt
	//	站通信设备状态发布包【InterSvr104/101→AppStatusMgr】					   
	//
	//===============================================================================================
	class MY_EXT_CLASS CStaCommDevPkt : public CProtocolHead
	{
	public:
		CStaCommDevPkt();
	    virtual ~CStaCommDevPkt();

	public:
		void Add(CStaCommDev State);								// 添加车站站通信设备状态
		WORD GetStaCount() const;									// 获取车站数量
		CStaCommDev* GetSta(int nIndex);							// 获取指定下标车站

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CStaCommDev> m_vecSta;								// 车站
	};

	//===============================================================================================
	//	
	//	CReqAllOperDevPkt
	//	请求全体操控设备状态包【操作台→AppStatusMgr】			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CReqAllOperDevPkt : public CReqStaCommDevPkt
	{
	public:
		CReqAllOperDevPkt();
		virtual ~CReqAllOperDevPkt();
	};

	//==============================================================================
	//
	//	CAllOperDevMeasure
	//	全体操控设备状态中遥测点【AppMeasureMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CAllOperDevMeasure : public CObjectBase
	{
	public:
		CAllOperDevMeasure();
	    virtual ~CAllOperDevMeasure();

	public:
		void SetId(DWORD dwId);										// 设置遥测点Id
		void SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag);				// 设置质量标志
		void SetValue(float fValue);								// 设置遥测值

		DWORD GetId() const;										// 获取遥测点Id
		ENUM_QUALITY_FLAG_TYPE GetQltFalg() const;					// 获取质量标志
		float GetValue() const;										// 获取遥测值

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwId;												// 遥测点Id
		BYTE m_byQltFlag;											// 质量标志
		float m_fValue;												// 遥测值
	};

	//===============================================================================================
	//	
	//	CAllOperDevCard
	//	全体操控设备状态中标志牌【AppStatusMgr→操作台】			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevCard : public CObjectBase
	{
	public:
		CAllOperDevCard();
		virtual ~CAllOperDevCard();

	public:
		void SetCardType(BYTE byCardType);							// 设置标志牌类型
		void SetCmdCode(string strSet);								// 设置作业命令编号
		void SetWorkRange(string strSet);							// 设置作业范围
		void SetDevId(WORD wSet);									// 设置设备ID
		void SetDispatcherName(string strDispatcherName);		 	// 设置调度台名称
		void SetOperName(string strOperName);					 	// 设置操作员名称
		void SetOperLevel(BYTE byOperLevel);						// 设置操作员级别
		void SetPassWord(string strSet);							// 设置密码
		void SetQuoteTime(DWORD dwQuoteTime);					 	// 设置挂牌时间
		void SetOffsetX(int nOffsetX);							 	// 设置偏移位置X
		void SetOffsetY(int nOffsetY);							 	// 设置偏移位置Y
		void SetContent(string strContent);						 	// 设置工作内容

		BYTE GetCardType() const;								 	// 获取标志牌类型
		string GetCmdCode() const;									// 获取作业命令编号
		string GetWorkRange() const;								// 获取作业范围
		WORD GetDevId() const;										// 获取设备Id
		string GetDispatcherName() const;						 	// 获取调度台名称
		string GetOperName() const;								 	// 获取操作员名称
		BYTE   GetOperLevel() const;								// 获取操作员级别
		string GetPassWord() const;									// 获取密码
		DWORD GetQuoteTime() const;								 	// 获取挂牌时间
		int GetOffsetX() const;									 	// 获取偏移位置X
		int GetOffsetY() const;									 	// 获取偏移位置Y
		string GetContent() const;								 	// 获取工作内容

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		BYTE m_byCardType;										 	// 标志牌类型
		string m_strCmdCode;										// 作业命令编号
		string m_strWorkRange;										// 作业范围
		WORD m_wDevId;												// 设置设备Id
		string m_strDispatcherName;								 	// 调度台名称
		string m_strOperName;									 	// 操作员名称
		BYTE   m_byOperLevel;										// 操作员级别
		string m_strPassWord;										// 密码
		DWORD m_dwQuoteTime;									 	// 挂牌时间
		int m_nOffsetX;											 	// 偏移位置X
		int m_nOffsetY;											 	// 偏移位置Y
		string m_strContent;									 	// 工作内容
	};

	//===============================================================================================
	//	
	//	CHandSet
	//	全体操控设备状态中手动设置信息【AppStatusMgr→操作台】	   
	//
	//===============================================================================================
	class MY_EXT_CLASS CHandSet : public CObjectBase
	{
	public:
		CHandSet();
		virtual ~CHandSet();

	public:
		void SetCollPointId(DWORD dwCollPointId);				 	// 设置手动设置采集点Id
		void SetType(ENUM_HAND_SET_TYPE byType);				 	// 设置设置类型
		void SetDispatcherName(string strDispatcherName);		 	// 设置调度台名称
		void SetOperName(string strOperName);					 	// 设置操作员名称
		void SetTime(DWORD dwTm);								 	// 设置设置时间
		void SetValue(float fValue);							 	// 设置设置值

		DWORD GetCollPointId() const;							 	// 获取手动设置采集点Id
		ENUM_HAND_SET_TYPE GetType() const;						 	// 获取设置类型
		string GetDispatcherName() const;						 	// 获取调度台名称
		string GetOperName() const;								 	// 获取操作员名称
		DWORD GetTime() const;									 	// 获取设置时间
		float GetValue() const;									 	// 获取设置值

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwCollPointId;									 	// 手动设置采集点Id
		BYTE m_byType;											 	// 设置类型
		string m_strDispatcherName;								 	// 调度台名称
		string m_strOperName;									 	// 操作员名称
		DWORD m_dwTime;											 	// 设置时间
		float m_fValue;										 		// 设置值
	};


	//===============================================================================================
	//	
	//	CAllOperDevInfo
	//	全体操控设备状态中操控设备【AppStatusMgr→操作台】	   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevInfo : public CObjectBase
	{
	public:
		CAllOperDevInfo();
		virtual ~CAllOperDevInfo();

	public:
		void SetOperDevId(DWORD dwOperDevId);					 	// 设置操控设备Id
		void SetExpress(BYTE* pExpress);						 	// 设置表示信息

		DWORD GetOperDevId() const;								 	// 获取操控设备Id
		BYTE* GetExpress();										 	// 获取表示信息

		void AddMesure(CAllOperDevMeasure Measure);					 	// 添加遥测点
		WORD GetMeasureCount() const;							 	// 获取遥测点数量
		CAllOperDevMeasure* GetMeasure(int nIndex);					 	// 获取指定下标遥测点

		void AddCard(CAllOperDevCard Card);						 	// 添加标志牌
		BYTE GetCardCount() const;								 	// 获取标志牌数量
		CAllOperDevCard* GetCard(int nIndex);					 	// 获取指定下标标志牌

		void AddHandSet(CHandSet HandSet);						 	// 添加手动设置信息
		BYTE GetHandSetCount() const;							 	// 获取手动设置信息数量
		CHandSet* GetHandSet(int nIndex);						 	// 获取指定下标的手动设置信息

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwOperDevId;									 	// 操控设备Id
		BYTE m_byExpress[8];									 	// 表示信息
		vector<CAllOperDevMeasure> m_vecMeasure;				 	// 遥测点信息
		vector<CAllOperDevCard> m_vecCard;						 	// 标志牌信息
		vector<CHandSet> m_vecHandSet;							 	// 手动设置信息
	};

	//===============================================================================================
	//	
	//	CAllOperDevSta
	//	全体操控设备状态中车站【AppStatusMgr→操作台】		   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevSta : public CObjectBase
	{
	public:
		CAllOperDevSta();
		virtual ~CAllOperDevSta();

	public:
		void SetStaId(WORD wStaId);									// 设置车站Id
		WORD GetStaId() const;										// 获取车站Id

		void AddOperDev(CAllOperDevInfo OperDev);					// 添加操控设备
		WORD GetOperDevCount() const;								// 获取操控设备数量
		CAllOperDevInfo* GetOperDev(int nIndex);					// 获取指定下标的操控设备

		virtual int StreamSize();									
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// 车站Id
		vector<CAllOperDevInfo> m_vecOperDev;						// 操控设备
	};

	//===============================================================================================
	//	
	//	CAllOperDevPkt
	//	全体操控设备状态包【AppStatusMgr→操作台】			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevPkt : public CProtocolHead
	{
	public:
		CAllOperDevPkt();
		virtual ~CAllOperDevPkt();

	public:
		void AddSta(CAllOperDevSta Sta);							// 添加车站
		WORD GetStaCount() const;									// 获取车站数量
		CAllOperDevSta* GetSta(int nIndex);							// 获取指定下标车站

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CAllOperDevSta> m_vecSta;							// 车站
	};

	//===============================================================================================
	//	
	//	CChgOperDevPkt
	//	变化操控设备状态包【AppStatusMgr→操作台】			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CChgOperDevPkt : public CAllOperDevPkt
	{
	public:
		CChgOperDevPkt();
		virtual ~CChgOperDevPkt();
	};

	//===============================================================================================
	//	
	//	CReqAllDispDevPkt
	//	请求全体电显设备状态包【操作台→AppStatusMgr】		   
	//
	//===============================================================================================
	class MY_EXT_CLASS CReqAllDispDevPkt : public CReqStaCommDevPkt
	{
	public:
		CReqAllDispDevPkt();
		virtual ~CReqAllDispDevPkt();
	};

	//===============================================================================================
	//	
	//	CAllDispDevInfo
	//	全体电显设备状态中电显设备【AppStatusMgr→操作台】
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllDispDevInfo : public CObjectBase
	{
	public:
		CAllDispDevInfo();
		virtual ~CAllDispDevInfo();

	public:
		void SetId(DWORD dwId);										// 设置电显设备Id
		void SetValue(BYTE byValue);								// 设置电显设备状态值

		DWORD GetId() const;										// 获取电显设备Id
		BYTE GetValue() const;										// 获取电显设备状态值

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwId;												// 电显设备Id
		BYTE m_byValue;												// 电显设备状态值
	};

	//===============================================================================================
	//	
	//	CAllDispDevSta
	//	全体电显设备状态中车站【AppStatusMgr→操作台】
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllDispDevSta : public CObjectBase
	{
	public:
		CAllDispDevSta();
		virtual ~CAllDispDevSta();

	public:
		void SetStaId(WORD wStaId);									// 设置车站Id
		WORD GetStaId();											// 获取车站Id

		void AddDispDev(CAllDispDevInfo DispDev);					// 添加电显设备
		WORD GetDispDevCount() const;								// 获取电显设备数量
		CAllDispDevInfo* GetDispDev(int nIndex);					// 获取指定下标电显设备

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// 车站Id
		vector<CAllDispDevInfo> m_vecDispDev;						// 电显设备
	};

	//===============================================================================================
	//	
	//	CAllDispDevPkt
	//	全体电显设备状态包【AppStatusMgr→操作台】
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllDispDevPkt : public CProtocolHead
	{
	public:
		CAllDispDevPkt();
		virtual ~CAllDispDevPkt();

	public:
		void AddSta(CAllDispDevSta Sta);							// 添加车站
		WORD GetStaCount() const;									// 获取车站数量
		CAllDispDevSta* GetSta(int nIndex);							// 获取指定下标车站

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CAllDispDevSta> m_vecSta;							// 车站
	};

	//===============================================================================================
	//	
	//	CChgDispDevPkt
	//	变化电显设备状态包【AppStatusMgr→操作台】
	//
	//===============================================================================================
	class MY_EXT_CLASS CChgDispDevPkt : public CAllDispDevPkt
	{
	public:
		CChgDispDevPkt();
		virtual ~CChgDispDevPkt();
	};

	//===============================================================================================
	//	
	//	CChgDispDevFromSvrPkt
	//	变化电显设备状态包【AppElecCalc→AppStatusMgr】
	//
	//===============================================================================================
	class MY_EXT_CLASS CChgDispDevFromSvrPkt : public CAllDispDevPkt
	{
	public:
		CChgDispDevFromSvrPkt();
		virtual ~CChgDispDevFromSvrPkt();
	};
}

#endif // !_PROTOCOL_DEV_STATE_H_