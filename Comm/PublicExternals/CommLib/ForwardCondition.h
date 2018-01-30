//==============================================================================
//                      ForwardCondition.h
//
//begin   : 2016-09-01
//describe:
//==============================================================================
#ifndef SELFDEF_FORWARDCONDITION_H 
#define SELFDEF_FORWARDCONDITION_H 

#pragma once

#include <vector>
#include <string>
#include <string.h>

#include "tinyxml.h"
#include "TypeDef.h"
#include "ProtocolForward.h"

using namespace std;
//==============================================================================
//                 
//                    CForwardCondition
//                     转发条件类
//
//==============================================================================
class MY_EXT_CLASS CForwardCondition
{ 
public: 
	CForwardCondition(); 
	virtual ~CForwardCondition(); 
	
	//==============================================================================
	//                 
	//                    CCondAddr
	//                   消息地址条件类
	//每个条件是一个数组，数组元素之间的是或的关系，不同条件之间是与的关系
	//如果某个条件为空，那么意味着该条件不受限制
	//==============================================================================
	class MY_EXT_CLASS CCondAddr
	{
	public: 
		CCondAddr(); 
		virtual ~CCondAddr(); 
		
	public:
		bool IsMatched(CProtocolForward::CForwardAddr& objAddr);
		bool IsEmpty();

	protected:
		bool IsBureauIdMatched(BYTE byBureauId);
		bool IsUnitTypeMatched(BYTE byUnitType);
		bool IsUnitIdMatched(WORD wUnitId);
		bool IsDevTypeMatched(BYTE byDevType);
		bool IsDevIdMatched(WORD wDevId);
		bool IsCltIdMatched(WORD wCltId);

	public:
		vector<BYTE> m_vecCondBureauId;
		vector<BYTE> m_vecCondUnitType;
		vector<WORD> m_vecCondUnitId;

		// unitId的范围数组，比如1，2，3，8表示第一个范围：1-2，第二个范围：3-8，
		// 该vector的size应该是偶数，范围是递增的关系，该条件和上面的m_vecUnitId条件一起构成unitId的条件，二者是或的关系
		vector<WORD> m_vecCondUnitIdRange;

		vector<BYTE> m_vecCondDevType;
		vector<WORD> m_vecCondDevId;
		vector<WORD> m_vecCondCltId;
	};

	//==============================================================================
	//                 
	//                    CCondTypeFunc
	//                消息类型码功能码条件类
	//
	//==============================================================================
	class MY_EXT_CLASS CCondTypeFunc
	{
	public: 
		CCondTypeFunc(); 
		virtual ~CCondTypeFunc(); 
		
	public:
		bool IsMatched(CProtocolForward::CForwardTypeFunc& objTypeFunc);
		bool IsEmpty();

	protected:
		bool IsTypFuncMatched(BYTE byType, BYTE byFunc);
		bool IsNotTypFuncMatched(BYTE byType, BYTE byFunc);
	
	public:
	   //types和funcs的个数一致，并且是一一对应的，也即第一个type对应第一个func，二者共同确定了一个418包类型
	   vector<BYTE> m_vecCondType;
	   vector<BYTE> m_vecCondFunc;
   
	   //表示418非的条件，必须满足，各个非条件是与的关系
	   vector<BYTE> m_vecCondNotType;
	   vector<BYTE> m_vecCondNotFunc;
	};

	//==============================================================================
	//                 
	//                    CCondRec
	//                 消息条件记录类
	//一个CCondRec代表注册信息里的一个rec
	//==============================================================================
	class MY_EXT_CLASS CCondRec
	{
	public: 
		CCondRec(); 
		virtual ~CCondRec(); 
   
	public:
	   bool IsMatched(CProtocolForward::CForwardHead& objForwardHead);
	   bool IsEmpty();

	public:
	   CCondAddr m_objSrcAddrCond;
	   CCondTypeFunc m_objTypeFuncCond;
	   BYTE m_byPlugId;
	};

public:
	bool Init(const char* szRegXml);
	bool Init(TiXmlHandle& txhReg);
	
	string& GetXmlStr();
	
	bool IsMatched(CProtocolForward::CForwardHead& objForwardHead);

	int GetCount();
	CCondRec& Get(int i);
	int Add(const CCondRec& msgCond);
	void Clear();

	// m_vecCondRec集合是否为空，为空时受m_bIsEmptyAllowAll控制，默认为禁止放行
	bool IsEmpty(); 
	void SetEmptyAllowAll(bool bIsEmptyAllowAll = true);

	string GetDumpStr();
	
protected:
	// 初始化一条消息记录，当xml文件<Rec>节点内无任何信息时，则代表全体放行，即任何信息都是匹配成功的
	bool InitCondRec(CCondRec& objCondRec, TiXmlElement* pTxeElem);
	bool InitCondAddr(CCondAddr& objCondAddr, TiXmlElement* pTxeElem);
	bool InitCondTypeFunc(CCondTypeFunc& objCondTypeFunc, TiXmlElement* pTxeElem);
	bool InitCondNotTypeFunc(CCondTypeFunc& objCondTypeFunc, TiXmlElement* pTxeElem);
	
	string GetCondRecDumpStr(CCondRec& objCondRec);
	string GetCondAddrDumpStr(CCondAddr& objCondAddr, int nFlag); // nFlag 1-src_addr
	string GetCondTypeFuncDumpStr(CCondTypeFunc& objCondTypeFunc);
	string GetCondNotTypeFuncDumpStr(CCondTypeFunc& objCondTypeFunc);
	
private:
	bool m_bIsEmptyAllowAll;		// 当消息条件集合为空时，控制是否匹配
	string m_strInitRegXml;			// 转发条件原始xml字符串，从配置文件中来，或者从注册包中来
	vector<CCondRec> m_vecCondRec;	// 一个CCondRec代表注册信息里的一个rec	
};

  
#endif 
