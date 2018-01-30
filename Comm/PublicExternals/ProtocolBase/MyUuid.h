//==============================================================================
//	
//							 MyUuid.h
//							   
//begin	  : 2016-08-10
//describe: 协议基础类
//
//==============================================================================

#pragma warning(disable : 4786)
#pragma warning(disable : 4100)
#pragma warning(disable : 4663)
#pragma warning(disable : 4018)
#pragma warning(disable : 4146)

#ifndef _MY_UUID_H
#define _MY_UUID_H

#include <assert.h>
#include <string>
#include "TypeDef.h"
#include "ProtocolBase.h"
#ifdef UNIX
#include "Buffer.h" 
#include "Utility.h"
#else
#include<afx.h>
#include "Buffer.h"
#endif

namespace ProtocolBase
{
	using namespace std; 

	//==========================================================================
	//	
	//							  CMyUuid
	//							  UUid对象
	//
	//==========================================================================
	class MY_EXT_CLASS CMyUuid
	{
	public:
		CMyUuid();
		virtual ~CMyUuid();
		bool operator == (const CMyUuid& other) const;
		bool operator<(const CMyUuid& other) const;

	public:
		int ToStreamBuf(char* pBuf, int& nBufLen); 
		int FromStreamBuf(const char* pBuf, int nBufLen);
		static int StreamSize();
	
	public:
		void Generate();		
		std::string GetStrUuid();
		void Init();

	public:
		bool GetUuid(unsigned char* uuid, int nLen);
		bool SetUuid(string strUuid);

	private:
		unsigned char m_cUuid[UUID_LENGTH];
	};
}

#endif	 // _MY_UUID_H
