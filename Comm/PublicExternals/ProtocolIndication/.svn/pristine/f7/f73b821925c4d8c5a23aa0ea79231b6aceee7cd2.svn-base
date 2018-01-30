//=========================================================================================
//	
//						ProtocolSignal.h
//							   
// begin    : 2016-08-24
// describe : 协议遥信类
//
//=========================================================================================
#ifndef	_PROTOCOL_SIGNAL_H
#define _PROTOCOL_SIGNAL_H

#include "ProtocolTime.h"
#include "TypeDef.h"
#include "PubDefSystem.h"
#include "ProtocolHead.h"
#include "ObjectBase.h"
#include <vector>
#include "ProtocolIndication.h"

namespace ProtocolIndication
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//	
	//	CStaCollAll
	//	遥信全体信息包中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CStaCollAll : public CObjectBase
	{
	public:
		CStaCollAll();
		virtual ~CStaCollAll();

	public:	
		void SetStaId(WORD wStationId);								// 设置车站Id
		WORD GetStaId() const;										// 获取车站Id

		void SetSigStartAddr(DWORD dwStart);						// 设置遥信起始地址
		DWORD GetSigStartAddr() const;								// 获取遥信起始地址

		// 单遥信操作
		WORD GetSignalCount() const;								// 获取遥信数量
		BYTE GetSignal(int nIndex) const;							// 获取遥信值
		void AddSignal(BYTE signal);								// 添加遥信值
		void Clear();												// 清空遥信值

		// 全站遥信操作
		WORD GetSigRealCnt() const;									// 站内遥信数量
		string GetAllSigs() const;									// 获取全站遥信
		void SetAllSigs(string strData);							// 获取全站遥信
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// 全站遥信按内存拷贝方式组包/解包
		virtual int StreamSize2();
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen);	
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen);

	private:
		WORD m_wStationId;											// 车站Id
		DWORD m_dwSigStartAddr;										// 遥信起始地址
		vector<BYTE> m_vecSignal;									// 遥信信息

		// 站内实际遥信数量
		WORD m_wRealSigCnt;
		// 站遥信集合，用于进行内存拷贝
		string m_strData;
	};

	//==============================================================================
	//	
	//									CSignalCollALLPkt
	//						遥信全体信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollAllPkt : public CProtocolHead
	{
	public:
		CSignalCollAllPkt();
		virtual ~CSignalCollAllPkt();

	public:
		WORD GetStaCount() const;									// 获取车站数量
		CStaCollAll* GetSta(int nIndex);							// 获取指定下标车站
		void Add(CStaCollAll &sta);									// 添加车站
		void Clear();												// 清空车站
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// 每个站采用内存拷贝进行组包、解包
		virtual int StreamSize2();		
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen); 

	private:
		vector<CStaCollAll> m_vecSta;								// 车站信息
	};

	//==============================================================================
	//
	//						遥信变化信息【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalCollChange
	//						遥信变化信息中遥信点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollChange : public CObjectBase
	{
	public:
		CSignalCollChange();
		CSignalCollChange(DWORD dwSignalAddr, BYTE byValue);
		virtual ~CSignalCollChange();

	public:
		void SetAddr(DWORD dwSignalAddr);							// 设置遥信点地址
		DWORD GetAddr() const;										// 获取遥信点地址
		void SetValue(BYTE byValue);								// 设置遥信值
		BYTE GetValue() const;										// 获取遥信值
		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwSignalAddr;										// 遥信点地址
		BYTE  m_byValue;											// 遥信值
	};

	//==============================================================================
	//	
	//									CStaCollChange
	//						遥信变化信息中厂站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CStaCollChange : public CObjectBase
	{
	public:
		CStaCollChange();
		virtual ~CStaCollChange();

	public:
		void SetId(WORD wStationId);								// 设置车站Id
		WORD GetId() const;											// 获取车站Id
		WORD GetSignalCount() const;								// 获取遥信数量
		CSignalCollChange* GetSignal(int nIndex);					// 获取指定下标的遥信
		void Add(CSignalCollChange &signal);						// 添加遥信
		void Clear();												// 清空遥信
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStationId;											//车站Id
		vector<CSignalCollChange> m_vecSignal;						//遥信信息
	};

	//==============================================================================
	//	
	//									CSignalCollChangePkt
	//						遥信变化信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollChangePkt : public CProtocolHead
	{
	public:
		CSignalCollChangePkt();
		virtual ~CSignalCollChangePkt();

	public:
		WORD GetStaCount() const;									// 获取车站数量
		CStaCollChange* GetSta(int nIndex);							// 获取指定下标车站
		void Add(CStaCollChange &sta);								// 添加车站
		void Clear();												// 清空车站
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaCollChange> m_vecSta;							// 车站信息
	};

	//==============================================================================
	//
	//							遥信全体信息【AppSignalMgr→操作台】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalOfSignalAllInfo
	//						遥信全体信息包中遥信点【AppSignalMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalOfSignalAllInfo : public CObjectBase
	{
	public:
		CSignalOfSignalAllInfo();
		virtual ~CSignalOfSignalAllInfo();

	public:
		void SetSignalId(DWORD dwId);								// 设置遥信点Id
		DWORD GetSignalId() const;									// 获取遥信点Id
		void SetQltFalg(ENUM_QUALITY_FLAG_TYPE eFlag);				// 设置质量标志
		ENUM_QUALITY_FLAG_TYPE GetQltFlag() const;					// 获取质量标志
		void SetValue(BYTE byValue);								// 变位设置遥信值
		BYTE GetValue() const;										// 获取遥信值
		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwSignalId;											// 遥信点Id
		BYTE m_byQltFlag;											// 质量标志
		BYTE  m_byValue;											// 遥信值
	};

	//==============================================================================
	//	
	//										CStaOfSignalAllInfo
	//							遥信全体信息包中车站【AppSignalMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CStaOfSignalAllInfo : public CObjectBase
	{
	public:
		CStaOfSignalAllInfo();
		CStaOfSignalAllInfo(WORD wId);
		virtual ~CStaOfSignalAllInfo();

	public:
		void SetId(WORD wId);											// 设置车站Id
		WORD GetId() const;												// 获取车站Id
		WORD GetSignalCount() const;									// 获取遥信数量
		CSignalOfSignalAllInfo* GetSignal(int nIndex);					// 获取指定下标遥信
		void Add(CSignalOfSignalAllInfo &signal);						// 添加遥信
		virtual void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wId;														//车站Id
		vector<CSignalOfSignalAllInfo> m_vecSignal;						//遥信信息
	};

	//==============================================================================
	//	
	//										CSignalAllPkt
	//							遥信全体信息包【AppSignalMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalAllPkt : public CProtocolHead
	{
	public:
		CSignalAllPkt();
		virtual ~CSignalAllPkt();

	public:		
		WORD GetStaCount() const;									// 获取车站数量
		CStaOfSignalAllInfo* GetSta(int nIndex);					// 获取指定下标车站
		void Add(CStaOfSignalAllInfo &sta);							// 添加车站
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaOfSignalAllInfo> m_vecSta;						// 车站信息
	};

	//==============================================================================
	//
	//							遥信变化信息【AppSignalMgr→操作台】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalChangePkt
	//							遥信变化信息包【AppSignalMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangePkt : public CSignalAllPkt
	{
	public:
		CSignalChangePkt();
	    virtual ~CSignalChangePkt();
	};

	//==============================================================================
	//
	//						遥信请求信息【AppMeasureMgr→InterSvr104】
	//
	//==============================================================================

	//==============================================================================
	//
	//							CSignalCollReqPkt
	//						遥信请求信息包【AppMeasureMgr→InterSvr104】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollReqPkt : public CProtocolHead
	{
	public:
		CSignalCollReqPkt();
		virtual ~CSignalCollReqPkt();

	public:
		WORD GetStaCount() const;									// 获取车站数量
		WORD GetSta(int nIndex) const;								// 获取指定下标车站
		void Add(WORD sta);											// 添加车站
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<WORD> m_vecSta;										// 车站信息
	};

	//==============================================================================
	//
	//						遥信请求信息【操作台→AppSignalMgr】
	//
	//==============================================================================

	//==============================================================================
	//
	//									CSignalReqPkt
	//							遥信请求信息包【操作台→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalReqPkt : public CSignalCollReqPkt
	{
	public:
		CSignalReqPkt();
	    virtual ~CSignalReqPkt();
	};

	//==============================================================================
	//
	//							遥信变位信息【AppSignalMgr→实时通道】
	//
	//==============================================================================

	//==============================================================================
	//
	//									CSignalChangeState
	//						遥信变位信息中遥信点【AppSignalMgr→实时通道】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangeState : public CObjectBase
	{
	public:
		CSignalChangeState();
		CSignalChangeState(DWORD dwId, BYTE byValue);
		virtual ~CSignalChangeState();

	public:
		void SetId(DWORD dwId);										// 设置遥信点Id
		DWORD GetId() const;										// 获取遥信点Id
		void SetValue(BYTE byValue);								// 设置遥信值
		BYTE GetValue() const;										// 获取遥信值

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwId;												// 遥信点Id
		BYTE m_byValue;												// 遥信值
	};

	//==============================================================================
	//
	//									CSignalChangeSta
	//						遥信变位信息车站【AppSignalMgr→实时通道】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangeSta : public CObjectBase
	{
	public:
		CSignalChangeSta();
		CSignalChangeSta(WORD wId);
		virtual ~CSignalChangeSta();

	public:
		void SetId(WORD wId);										// 设置车站Id
		WORD GetId() const;											// 获取车站Id
		WORD GetSignalCount() const;								// 获取遥信数量
		CSignalChangeState* GetSignal(int nIndex);					// 获取指定下标遥信
		void Add(CSignalChangeState &signal);						// 添加遥信
		void Clear();												// 清空遥信

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wId;													// 车站Id
		vector<CSignalChangeState> m_vecSignal;						// 遥信信息 
	};

	//==============================================================================
	//
	//								CSignalChangeStatePkt
	//						遥信变位信息包【AppSignalMgr→实时通道】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangeStatePkt : public CProtocolHead
	{
	public:
		CSignalChangeStatePkt();
		virtual ~CSignalChangeStatePkt();

	public:		
		WORD GetStaCount() const;									// 获取车站数量
		CSignalChangeSta* GetSta(int nIndex);						// 获取指定下标车站
		void Add(CSignalChangeSta &sta);							// 添加车站
		void Clear();												// 清空车站

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CSignalChangeSta> m_vecSta;							// 车站信息
	};

	//==============================================================================
	//
	//								CSignalCallPkt
	//						遥信总召信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCallPkt : public CSignalCollAllPkt
	{
	public:
		CSignalCallPkt();
		virtual ~CSignalCallPkt();
	};

	//==============================================================================
	//
	//						SOE信息【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	
	//==============================================================================
	//	
	//									CSignalSOE
	//						SOE信息包中遥信点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalSOE : public CObjectBase
	{
	public:
		CSignalSOE();
		virtual ~CSignalSOE();

	public:
		void SetAddr(DWORD dwAddr);									// 设置遥信地址			
		DWORD GetAddr() const;										// 获取遥信地址

		void SetValue(BYTE byValue);								// 设置遥信值
		BYTE GetValue() const;										// 获取遥信值

		void SetSOETime(BYTE byYear, BYTE byMonth, BYTE byDay,		// 设置SOE时间：年月日、时分秒、毫秒
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetSOETime(time_t t, WORD wMilliSec);					// 设置SOE时间：time_t、毫秒
		void SetSOETime1(DWORD dwDate, DWORD dwTime);				// 设置SOE时间：wDate：年月日：20160912；wTime：时分秒：101010
		void SetSOETime(timeval t);									// 设置SOE时间：timeval
		void SetSOETime(CProtocolTime objTime);

		CProtocolTime GetProtocolSOETime() const;
		timeval GetSOETime() const;

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwAddr;												// 遥信地址
		BYTE m_byValue;												// 遥信值
		CProtocolTime m_SOETime;									// SOE时间
	};

	//==============================================================================
	//	
	//									  CStaSOE
	//						SOE信息包中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CStaSOE : public CObjectBase
	{
	public:
		CStaSOE();
		virtual ~CStaSOE();

	public:
		void SetStaId(WORD wStaIdSOE);								// 设置车站Id
		WORD GetStaId() const;										// 获取车站Id
		WORD GetSignalCount() const;								// 获取SOE数量
		CSignalSOE* GetSignal(int nIndex);							// 获取指定下标SOE
		void Add(CSignalSOE &signal);								// 添加SOE

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;												// 车站Id
		vector<CSignalSOE> m_vecSignal;								// SOE
	};

	//==============================================================================
	//	
	//										 CSOEPkt
	//						  SOE信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEPkt : public CProtocolHead
	{
	public:
		CSOEPkt();
		virtual ~CSOEPkt();

	public:
		WORD GetStaCount() const;									// 获取车站数量
		CStaSOE* GetSta(int nIndex);								// 获取指定下标车站
		void Add(CStaSOE &sta);										// 添加车站

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaSOE> m_vecSta;									// 车站信息
	};

	//==============================================================================
	//	
	//	CSOEReplyPkt
	//	SOE信息回执包【AppSignalMgr→InterSvr104】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEReplyPkt : public CProtocolHead
	{
	public:
		CSOEReplyPkt();
		virtual ~CSOEReplyPkt();
	};

	//==============================================================================
	//
	//						SOE存盘信息【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalSOESave
	//						SOE存盘信息包中遥信点【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOESave : public CObjectBase
	{
	public:
		CSOESave();
		virtual ~CSOESave();

	public:
		void SetId(DWORD dwId);										// 设置遥信Id
		DWORD GetId() const;										// 获取遥信Id
		
		void SetName(string strSet);								// 设置遥信名称
		string GetName() const;										// 获取遥信名称
		
		void SetLevel(BYTE bySet);									// 设置遥信电压等级
		BYTE GetLevel() const;										// 获取遥信电压等级

		void SetValue(BYTE byValue);								// 设置遥信值
		BYTE GetValue() const;										// 获取遥信值

		void SetRecTime(BYTE byYear, BYTE byMonth, BYTE byDay,		// 设置SOE记录时间：年月日、时分秒、毫秒
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetRecTime(time_t t, WORD wMilliSec);					// 设置SOE记录时间：time_t、毫秒
		void SetRecTime1(DWORD dwDate, DWORD dwTime);				// 设置SOE记录时间：wDate：年月日：20160912；wTime：时分秒：101010
		void SetRecTime(timeval t);									// 设置SOE记录时间：timeval
		void SetRecTime(CProtocolTime objTime);
		
		CProtocolTime* GetRecTime();								// 获取SOE记录时间：CProtocolTime
		timeval GetRecTime1() const;								// 获取SOE记录时间：timeval

		void SetProductTime(BYTE byYear, BYTE byMonth, BYTE byDay,	// 设置SOE厂家产生时间：年月日、时分秒、毫秒
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetProductTime(time_t t, WORD wMilliSec);				// 设置SOE厂家产生时间：time_t、毫秒
		void SetProductTime1(DWORD dwDate, DWORD dwTime);			// 设置SOE厂家产生时间：wDate：年月日：20160912；wTime：时分秒：101010
		void SetProductTime(timeval t);								// 设置SOE厂家产生时间：timeval
		void SetProductTime(CProtocolTime objTime);

		CProtocolTime* GetProductTime();							// 获取SOE厂家产生时间：CProtocolTime
		timeval GetProductTime1() const;							// 获取SOE厂家产生时间：timeval

		int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwId;												// 遥信Id
		string m_strName;											// 遥信名称
		BYTE m_byLevel;												// 遥信电压等级
		BYTE m_byValue;												// 遥信值
		CProtocolTime m_objProductTime;								// SOE时间
		CProtocolTime m_objRecTime;									// SOE记录时间
	};

	//==============================================================================
	//	
	//									  CStaSOESave
	//						SOE存盘信息包中车站【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CStaSOESave : public CObjectBase
	{
	public:
		CStaSOESave();
		virtual ~CStaSOESave();

	public:
		void SetStaId(WORD wStaId);									// 设置车站Id
		WORD GetStaId() const;										// 获取车站Id
		WORD GetSoeCount() const;									// 获取SOE数量
		CSOESave* GetSoe(int nIndex);								// 获取指定下标SOE
		void Add(CSOESave &signal);									// 添加SOE

		void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// 车站Id
		vector<CSOESave> m_vecSoe;									// SOE
	};

	//==============================================================================
	//	
	//									 CSOESavePkt
	//						 SOE存盘信息包【AppSignalMgr→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOESavePkt : public CProtocolHead
	{
	public:
		CSOESavePkt();
		virtual ~CSOESavePkt();

	public:
		WORD GetStaCount() const;									// 获取车站数量
		CStaSOESave* GetSta(int nIndex);							// 获取指定下标车站
		void Add(CStaSOESave &sta);									// 添加车站

		void Clear();
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaSOESave> m_vecSta;								// 车站信息
	};

	//==============================================================================
	//	
	//	CSOESaveReplyPkt
	//	SOE存盘信息回执包【OSM模块→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOESaveReplyPkt : public CProtocolHead
	{
	public:
		CSOESaveReplyPkt();
	 	virtual ~CSOESaveReplyPkt();

	public:
		void SetReceivseRslt(DWORD dwRslt);							// 设置接收结果
		void SetDesc(string strDesc);								// 设置描述
		
		DWORD GetReceiveRslt() const;								// 获取接收结果
		string GetDesc() const;										// 获取描述

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwReceiveRslt;										// 接收结果
		string m_strDesc;											// 描述
	};

	//==============================================================================
	//	
	//	CStaOfSOEQueryReq
	//	SOE查询请求中车站【其他模块→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CStaOfSOEQueryReq : public CObjectBase
	{
	public:
		CStaOfSOEQueryReq();
		virtual ~CStaOfSOEQueryReq();

	public:
		void SetStaId(WORD wStaId);									// 设置车站Id
		WORD GetStaId() const;										// 获取车站Id 

		void Add(DWORD dwSignal);									// 添加遥信点
		WORD GetSignalCount() const;								// 获取遥信点数量
		DWORD GetSignal(int nIndex) const;							// 获取指定下标遥信点

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;												// 车站Id				
		vector<DWORD> m_vecSignal;									// 遥信点
	};

	//==============================================================================
	//	
	//	CSOEQueryReqPkt
	//	SOE查询请求包【其他模块→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEQueryReqPkt : public CProtocolHead
	{
	public:
		CSOEQueryReqPkt();
		virtual ~CSOEQueryReqPkt();

	public:
		void SetStartTime(BYTE byYear, BYTE byMonth, BYTE byDay,	// 设置起始时间：年月日、时分秒、毫秒
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetStartTime(time_t t, WORD wMs);						// 设置起始时间
		void SetStartTime(timeval t);								// 设置起始时间

		void SetEndTime(BYTE byYear, BYTE byMonth, BYTE byDay,		// 设置终止时间：年月日、时分秒、毫秒
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetEndTime(time_t t, WORD wMs);						// 设置终止时间
		void SetEndTime(timeval t);									// 设置终止时间

		CProtocolTime GetStartTime() const;							// 获取起始时间
		time_t GetStartTime_t() const;								// 获取起始时间time_t
		timeval GetStartTimeval() const;							// 获取起始时间timeval

		CProtocolTime GetEndTime() const;							// 获取终止时间
		time_t GetEndTime_t() const;								// 获取终止时间time_t
		timeval GetEndTimeval() const;								// 获取终止时间timeval

		void Add(CStaOfSOEQueryReq Sta);							// 添加下一车站
		WORD GetStaCount() const;									// 获取车站数量
		CStaOfSOEQueryReq* GetSta(int nIndex);						// 获取指定下标车站

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		CProtocolTime m_StartTime;									// 查询起始时间
		CProtocolTime m_EndTime;									// 查询终止时间
		vector<CStaOfSOEQueryReq> m_vecSta;							// 车站
	};

	//==============================================================================
	//	
	//	CSOEQueryRsltPkt
	//	SOE查询结果包【OSM模块→其他模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEQueryRsltPkt : public CSOESavePkt
	{
	public:
		CSOEQueryRsltPkt();
		virtual ~CSOEQueryRsltPkt();
	};
}

#endif	// _PROTOCOL_SIGNAL_H