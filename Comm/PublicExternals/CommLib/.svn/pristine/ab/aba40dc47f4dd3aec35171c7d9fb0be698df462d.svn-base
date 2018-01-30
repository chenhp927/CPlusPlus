//==============================================================================
//                      CommAddr.h
//
//begin   : 2016-09-12
//describe:
//==============================================================================
#ifndef SELFDEF_COMMADDR_H 
#define SELFDEF_COMMADDR_H 

#pragma once

#include <string.h>
#include <string>
#include <stdio.h>

#include "TypeDef.h"

using namespace std;
//==============================================================================
//                 
//                  CCommAddr
//                  通信地址类
//
//==============================================================================
class MY_EXT_CLASS CCommAddr
{
public:
	CCommAddr();
	virtual ~CCommAddr();
	
public:
	BYTE GetBureauId();
	void SetBureauId(BYTE by);
	BYTE GetUnitType();
	void SetUnitType(BYTE by);
	WORD GetUnitId();
	void SetUnitId(WORD w);
	BYTE GetDevType();
	void SetDevType(BYTE by);
	WORD GetDevId();
	void SetDevId(WORD w);
	BYTE GetAbFlag();
	void SetAbFlag(BYTE byAbFlag);
	WORD GetCltId();
	void SetCltId(WORD w);
	
	// val是否为一个无效值(或者全体值)
	static bool IsInvalidBureauId(BYTE by);
	static bool IsInvalidUnitType(BYTE by);
	static bool IsInvalidUnitId(WORD w);
	static bool IsInvalidDevType(BYTE by);
	static bool IsInvalidDevId(WORD w);
	static bool IsInvalidCltId(WORD w);
	static bool IsInvalidAbFlag(BYTE byAbFlag);

	string GetDumpStr();
	
public:
	bool operator == (const CCommAddr& objOther);
	bool IsMatched(CCommAddr& obj);

protected:
	BYTE m_byBureauId;	// 局码
	BYTE m_byUnitType;	// 单位类型
	WORD m_wUnitId;		// 单位ID
	BYTE m_byDevType;	// 设备类型
	WORD m_wDevId;		// 设备ID
	BYTE m_byAbFlag;    // Ab机标识
	WORD m_wCltId;		// 客户连接ID
};

  
#endif 
