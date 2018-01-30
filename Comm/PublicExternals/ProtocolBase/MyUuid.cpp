//==============================================================================
//	
//							 MyUuid.cpp
//							   
//begin	  : 2016-08-10
//describe: UUID对象
//
//==============================================================================

#include "ProtocolBase.h"
#include "MyUuid.h"
#include <stdio.h>

#ifdef UNIX
#include <uuid/uuid.h>
#else
#include <objbase.h>
#endif

namespace ProtocolBase
{
	//==========================================================================
	//	
	//							  CMyUuid
	//							  UUid对象
	//
	//==========================================================================
	CMyUuid::CMyUuid()
	{
		memset(m_cUuid, 0, sizeof(m_cUuid));
	}

	CMyUuid::~CMyUuid()
	{
		
	}

	void CMyUuid::Init()
	{
		memset(m_cUuid, 0, sizeof(m_cUuid));
	}
	
	bool CMyUuid::operator==(const CMyUuid& other) const
	{
		for (int i = 0; i < UUID_LENGTH; i++)
		{
			if (m_cUuid[i] != other.m_cUuid[i])
			{
				return false;
			}
		}

		return true;
	}

	bool CMyUuid::operator<(const CMyUuid& other) const
	{
		for (int i = 0; i < UUID_LENGTH; i++)
		{
			if (m_cUuid[i] > other.m_cUuid[i])
			{
				return false;
			}
			else if (m_cUuid[i] < other.m_cUuid[i])
			{
				return true;
			}
		}

		return false;
	}

	int CMyUuid::StreamSize()
	{
		return UUID_LENGTH;
	}

	int CMyUuid::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if(nBufLen < StreamSize())
		{
			return -1;
		}

		int nOffset = 0;

		for (int i = 0; i < UUID_LENGTH; i++)
		{
			m_cUuid[i] = pBuf[nOffset];
			nOffset += 1;
		}
		
		return nOffset;
	}

	int CMyUuid::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			nBufLen = StreamSize();
			return -1;
		}
		
		int nOffset = 0;
		for (int i = 0; i < UUID_LENGTH; i++)
		{
			pBuf[nOffset] = m_cUuid[i];
			nOffset += 1;
		}
		
		return nOffset;
	}

	void CMyUuid::Generate()
	{
		// 生成UUID
#ifdef UNIX
		uuid_t uuid;
		uuid_generate_time(uuid);
		
		for (int i = 0; i < UUID_LENGTH; i++)
		{
			m_cUuid[i] = uuid[i];
		}
#else	// WIN32
		GUID guid;
		CoCreateGuid(&guid);
		
		// DWORD Data1
		WORD wHiWordData1 = HIWORD(guid.Data1);
		m_cUuid[0] = HIBYTE(wHiWordData1);
		m_cUuid[1] = LOBYTE(wHiWordData1);
		
		WORD wLoWordData1 = LOWORD(guid.Data1);
		m_cUuid[2] = HIBYTE(wLoWordData1);
		m_cUuid[3] = LOBYTE(wLoWordData1);
		
		// WORD Data2
		m_cUuid[4] = HIBYTE(guid.Data2);
		m_cUuid[5] = LOBYTE(guid.Data2);
		
		// WORD Data3
		m_cUuid[6] = HIBYTE(guid.Data3);
		m_cUuid[7] = LOBYTE(guid.Data3);
		
		// BYTE Data4[8]
		for (int a = 8, b = 0; a < UUID_LENGTH && b < 8; a++, b++)
		{
			m_cUuid[a] = guid.Data4[b];
		}
#endif
		return;
	}

	std::string CMyUuid::GetStrUuid()
	{
		std::string strUuid = "";

#ifdef UNIX
		char buf[64] = {0};
		uuid_unparse(m_cUuid, buf);
		strUuid.append(buf);
		
#else  // WIN32
		int counter = 0;
		for (; counter < 4; counter++)
		{
			char cTmp1[20] = {0};
			sprintf(cTmp1, "%02x", m_cUuid[counter]);
			strUuid.append(cTmp1);
		}
		strUuid.append(1, '-');

		counter = 4;
		for (int a = 0; a < 3; a++)
		{
			char cTmp2[20] = {0};
			sprintf(cTmp2, "%02x", m_cUuid[counter]);
			counter++;
			strUuid.append(cTmp2);
			
			memset(cTmp2, 0, sizeof(cTmp2));
			sprintf(cTmp2, "%02x", m_cUuid[counter]);
			counter++;
			strUuid.append(cTmp2);

			strUuid.append(1,'-');
		}

		counter = 10;
		for (; counter < UUID_LENGTH; counter++)
		{
			char cTmp3[20] = {0};
			sprintf(cTmp3, "%02x", m_cUuid[counter]);
			strUuid.append(cTmp3);
		}
#endif
		return strUuid;
	}

	bool CMyUuid::GetUuid(unsigned char* uuid, int nLen)
	{
		if (nLen < UUID_LENGTH)
		{
			return false;
		}

		for (int i = 0; i < UUID_LENGTH; i++)
		{
			uuid[i] = m_cUuid[i];
		}

		return true;
	}
	
	bool CMyUuid::SetUuid(std::string strUuid)
	{
		if (strUuid.length() < (2*UUID_LENGTH + 4))
		{
			return false;
		}

		int j = 0;
		for (int i = 0; i < (2*UUID_LENGTH + 4); i++)
		{
			if ((i == 8) || (i == 13) || (i == 18) || (i == 23))
			{
				//跳过连字符
				continue;
			}
			if (j%2 == 0)
			{	
				unsigned int n = 0;
				char chTemp[2];
				chTemp[0] = strUuid[i];
				chTemp[1] = strUuid[i + 1];
				sscanf(chTemp, "%x", &n);
				m_cUuid[j/2] = n;
			}
			j++;
		}

		return true;
	}
}


