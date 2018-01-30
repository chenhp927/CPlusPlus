//==============================================================================
//                      ProtocolForward.h
//
//begin   : 2016-08-31
//describe:
//==============================================================================
#ifndef SELFDEF_PROTOCOFORWARD_H 
#define SELFDEF_PROTOCOFORWARD_H 

#pragma once

#include <vector>
#include <stdio.h>

#include "Buffer.h"
#include "tinyxml.h"
#include "CommAddr.h"

using namespace std;
class CForwardCondition;
//==============================================================================
//                 
//                    CProtocolForward
//                     转发层协议类
//
//==============================================================================
class MY_EXT_CLASS CProtocolForward 
{ 
public: 
	CProtocolForward(); 
	virtual ~CProtocolForward(); 
	
public: 
	//==============================================================================
	//                 
	//                    CForwardDataSeq
	//                     双网数据序号类
	//
	//==============================================================================
	class MY_EXT_CLASS CForwardDataSeq
	{
	public:
		CForwardDataSeq();
		virtual ~CForwardDataSeq();

	public:
		DWORD GetLow();
		DWORD GetHigh();

		// 初始化为无效序列号0
		void Reset();
		bool IsValid();
		bool IsFarawayFrom(const CForwardDataSeq& other);
		
		// 递增，操作后保证有效
		void Inc();	
		CForwardDataSeq GetNextSeq();

		static int Size();
		int ToStreamBuf(char* szBuf);
		int FromStreamBuf(const char* szBuf);
		
	public:
		bool operator == (const CForwardDataSeq& other);
		bool operator < (const CForwardDataSeq& other);
		bool operator > (const CForwardDataSeq& other);

	public:
		string GetDumpStr();
		
	protected:
		// 下面两个属性共同构成了64位数据序列号
		DWORD m_nSeqLow;	// 数据序列号低32位
		DWORD m_nSeqHigh;	// 数据序列号高32位
	};

	//==============================================================================
	//                 
	//                  CForwardAddr
	//                  消息地址类
	//
	//==============================================================================
	class MY_EXT_CLASS CForwardAddr : public CCommAddr
	{
	public:
		CForwardAddr();
		virtual ~CForwardAddr();
		
	public:
		static int Size();
		int ToStreamBuf(char* szBuf);
		int FromStreamBuf(const char* szBuf);

	public:
		bool operator == (const CForwardAddr& objOther);
		bool IsMatched(CCommAddr& objAddr);
	};

	//==============================================================================
	//                 
	//                    CForwardTypeFunc
	//                    消息类型码功能码类
	//
	//==============================================================================
	class MY_EXT_CLASS CForwardTypeFunc
	{
	public:
		CForwardTypeFunc();
		virtual ~CForwardTypeFunc();
		
	public:
		BYTE GetType();
		void SetType(BYTE by);
		BYTE GetFunc();
		void SetFunc(BYTE by);
		
		bool IsMatched(CForwardTypeFunc& obj);

		static int Size();
		int ToStreamBuf(char* szBuf);
		int FromStreamBuf(const char* szBuf);

		string GetTagDumpStr();

		// val是否为一个无效值(或者全体值)
		static bool IsInvalidType(BYTE by);
		static bool IsInvalidFunc(BYTE by);
		
	protected:
		BYTE m_byType;
		BYTE m_byFunc;
	};

	//==============================================================================
	//                 
	//                     CForwardHead
	//                     转发层包头类
	//
	//==============================================================================
	class MY_EXT_CLASS CForwardHead
	{
	public:
		CForwardHead();
		virtual ~CForwardHead();

	public:	
		CForwardDataSeq GetDataSeq();
		void SetDataSeq(CForwardDataSeq& objDataSeq);

		BYTE GetPackType();
		void SetPackType(BYTE by);
		bool IsRegisterPack();
		bool IsForwardPack();
		
		BYTE GetForwardType();
		void SetForwardType(BYTE by);

		// 设置该包支持台间转发
		void SetSupportLocalDispatch(bool bIsSupportLocalDispatch = true); 

		// 是否支持台间转发，如接入到一个tdci服务端的终端之间直接互相转发
		bool IsSupportLocalDispatch(); 
		
		void SetTypeFunc(CForwardTypeFunc& objTypeFunc);
		void SetTypeFunc(BYTE byType, BYTE byFunc);
		CForwardTypeFunc GetTypeFunc();
		BYTE GetMsgType();
		BYTE GetMsgFunc();

		void SetSrcAddr(CForwardAddr& objSrcAddr);
		CForwardAddr GetSrcAddr();

		vector<CForwardAddr>& GetDestAddrs();
		void AddDest(CForwardAddr& objDestAddr); 

		void SetMsgBodyLen(DWORD dwLen);
		DWORD GetMsgBodyLen();

		// 根据设置的属性计算打包后的包长度
		int StreamSize(); 
		int FromStreamBuf(const char* szBuf, int nBufLen);
		int ToStreamBuf(char* szBuf, int& nBufLen);
		
		// 消息头是否有效
		bool IsValid(char* szErrInfo = "", int nErrLen = 0);
		
	protected:
		CForwardDataSeq m_objDataSeq;		// 双网序列号
		BYTE m_byPackType;					// 包类型，如注册类型、转发类型等
		WORD m_wForwardInfoLen;				// 转发信息长度
		BYTE m_byForwardType;				// 转发类型
		CForwardTypeFunc m_objTypeFunc;		// 类型码功能码
		CForwardAddr m_objSrcAddr;			// 源地址
		vector<CForwardAddr> m_vecDestAddr;	// 目标地址集合
		DWORD m_dwMsgBodyLen;				// 消息体长度
	};

	//==============================================================================
	//                 
	//                     CForwardRegPkt
	//                      注册包类
	//
	//==============================================================================
	class MY_EXT_CLASS CForwardRegPkt
	{
	public:
		CForwardRegPkt();
		virtual ~CForwardRegPkt();

	public:
		DWORD GetReqId();
		void SetReqId(DWORD dw);
		CForwardCondition& GetForwardCond();

		int FromStreamBuf(const char* szBuf, int nLen);
		int ToStreamBuf(char* szBuf, int& nLen);
		int ToStreamBuf(CBuffer* pBuf);

	private:
		DWORD m_dwReqId;				// 请求编号
		CForwardCondition* m_pForwardCond;
	};

public:
	static bool IsValid(const char* szBuf, int nBufLen, char* szErrInfo = 0, int nErrLen = 0);  
	
	static bool IsMsgForwardPkt(const char* szBuf, int nLen);
	static bool IsMsgRegisterPkt(const char* szBuf, int nLen);
	
	static bool GetDataSeq(const char* szBuf, int nLen, CForwardDataSeq& objDataSeq);
	static bool SetDataSeq(char* szBuf, int nLen, CForwardDataSeq& objDataSeq);
	static bool GetSrcAddr(const char* szBuf, int nLen, CCommAddr& objSrcAddr);
	static bool SetSrcAddr(char* szBuf, int nLen, CCommAddr& objSrcAddr);
	static bool UpdateTypeFunc(char* szBuf, int nLen);
	static bool GetForwardHeadTypeFunc(char* szBuf, int nLen, CForwardTypeFunc& objTypeFunc);

	static bool MakeRegPack(CBuffer* pBuf, CForwardCondition& objForwardCondition, DWORD dwReqId = 0);
	static string GetXmlRegStr(char* szBuf, int nLen);
	static bool SetXmlRegStr(CBuffer* pBuf, const char* szXmlReg);
};

  
#endif 
