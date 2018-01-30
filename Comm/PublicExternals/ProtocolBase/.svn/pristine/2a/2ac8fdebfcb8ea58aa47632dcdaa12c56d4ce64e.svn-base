//==============================================================================
//	
//							 ProtocolHead.h
//							   
//begin	  : 2016/08/13
//describe: 协议基础类
//
//==============================================================================

#pragma warning(disable : 4786)
#pragma warning(disable : 4100)
#pragma warning(disable : 4663)
#pragma warning(disable : 4018)
#pragma warning(disable : 4146)

#ifndef _PROTOCOL_HEAD_H
#define _PROTOCOL_HEAD_H

#include "TypeDef.h"

#ifdef UNIX
#include "Buffer.h" 
#include "Utility.h"
#include "PubMacro.h"
#else
#include<afx.h>
#include "Buffer.h"
#endif

#include "ObjectBase.h"
#include "ProtocolTime.h"

namespace ProtocolBase
{
	using namespace std; 


	//==============================================================================
	//	
	//						   CReferenceCount
	//							 引用计数
	//
	//==============================================================================
	class MY_EXT_CLASS CReferenceCount
	{
	public:
		CReferenceCount();
		virtual ~CReferenceCount();
		
	public:
		int AddRef();
		int Release();
		int GetRef();
		void Reset();
		
	protected:
		int m_nRefCount;
	};

	//==============================================================================
	//	
	//							  CProtocolHead
	//							    协议包包头
	//
	//==============================================================================
	class MY_EXT_CLASS CProtocolHead : public CObjectBase
	{ 
	public: 
		CProtocolHead(); 
		CProtocolHead(BYTE byType, BYTE byFunc); 
		CProtocolHead(const CProtocolHead &other);
		virtual ~CProtocolHead(); 
		CProtocolHead & operator = (const CProtocolHead& other);
		
	public: 
		virtual BYTE GetType();
		virtual BYTE GetFunc();
		virtual BYTE GetVersion();
		virtual BYTE GetPktCount();
		virtual BYTE GetCurPkt();
		virtual DWORD GetLen();
		virtual DWORD GetRequestCode();
		virtual CProtocolTime GetProtocolTime() const;
		virtual timeval GetTime() const;

		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 
		virtual CProtocolHead * Clone();
		virtual void Release();
		virtual void AddRef();
		static int HeaderSize();
		virtual int StreamSize();
		
		string GetProErrDescrip(BYTE byErrType);
		int CheckValid(const char* pBuf, int nBufLen);
		
	public: 
		void SetType(BYTE byType);
		void SetFunc(BYTE byFunc);
		void SetVersion(BYTE byVer);
		void SetPktCount(BYTE byPktCount);
		void SetCurPkt(BYTE byCurPkt);
		void SetRequestCode(DWORD dwReqCode);
		void SetLen(DWORD dwPktLen);
		void SetTime(BYTE wYear, BYTE byMonth, BYTE byDay, BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetTime(time_t t, WORD wMilliSec);
		void SetTime(DWORD dwDate, DWORD dwTime);		// dwDate：年月日：20160912；dwTime：时分秒：101010
		void SetTime(timeval t);
		
	public:
		static BYTE GetType(const char* pBuf);
		static BYTE GetFunc(const char* pBuf); 
		static BYTE PacketCount(const char* pBuf); 
		static BYTE PacketNum(const char* pBuf); 
		static DWORD PacketLen(const char* pBuf); 
		static DWORD GetRequestCode(const char* pBuf);

		static int UpdatePktCount(BYTE byPktCount, CBuffer* pBuf);
		static int UpdateCurPkt(BYTE byCurPkt, CBuffer* pBuf);
		static int UpdateLen(DWORD dwPktLen, CBuffer* pBuf);
		
	protected:
		BYTE m_byType;					// 类型码
		BYTE m_byFunc;					// 功能码
		BYTE m_byVersion;				// 版本号
		BYTE m_byPktCount;				// 总包数 
		BYTE m_byCurPkt;				// 当前包号
		DWORD m_dwRequestCode;			// 请求方所发出操作命令的统一顺序编号，此编号由请求方生成，应答方回复时进行回填，若无请求方时填0。
		CProtocolTime m_Time;			// 发送时间
		DWORD m_dwPktLen;				// 数据包信息内容总字节数(不包括类型码、功能码、版本号、字长、总包数、当前包号、请求命令编号、发送时间)，不能超过0x100000
		CReferenceCount m_RefCount;		// 数据包引用次数
	};
}

#endif		// _PROTOCOL_HEAD_H
