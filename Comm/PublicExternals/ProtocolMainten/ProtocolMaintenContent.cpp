//=========================================================================================
//
//							 ProtocolMaintenContent.cpp
//
// begin    : 2016-10-10
// describe : ����ά��Э�鼯
//
//=========================================================================================
#include "ProtocolMainten.h"
#include "ProtocolMaintenContent.h"

namespace ProtocolMaintenContent
{
	
	//==============================================================================
	//
	//                CReqLocalCfgItoAPkt
	//                ���󱾻�����������Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	CReqLocalCfgItoAPkt::CReqLocalCfgItoAPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_LOCALMONICFG_REQ_INTERTOAPP)
	{
	}
	
	CReqLocalCfgItoAPkt::~CReqLocalCfgItoAPkt()
	{
	
	}
	
	void CReqLocalCfgItoAPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}
	
	string CReqLocalCfgItoAPkt::GetHostName() const
	{
		return m_strHostName;
	}
	
	CProtocolHead* CReqLocalCfgItoAPkt::Clone()
	{
		CReqLocalCfgItoAPkt* pClone = new CReqLocalCfgItoAPkt();
		*pClone = *this;
		return pClone;
	}
	
	int CReqLocalCfgItoAPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ����������
		nSize += sizeof(BYTE);
		
		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;
		
		return nSize;
	}
	
	int CReqLocalCfgItoAPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqLocalCfgItoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);
		
		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;
		
		return nOffset;
	}
	
	int CReqLocalCfgItoAPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqLocalCfgItoAPkt HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqLocalCfgItoAPkt HostName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostName = str0xff;
		nOffset += byLen;
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//             CMoniInfoBase
	//          ������Ϣ���࣬���������������������
	//
	//==============================================================================
	CMoniInfoBase::CMoniInfoBase()
		:m_byMoniType(0)
	{
		
	}

	CMoniInfoBase::~CMoniInfoBase()
	{

	}

	void CMoniInfoBase::SetMoniType(BYTE byMoniType)
	{
		m_byMoniType = byMoniType;
	}

	BYTE CMoniInfoBase::GetMoniType() const
	{
		return m_byMoniType;
	}

	int CMoniInfoBase::StreamSize()
	{
		//�������
		return 1;
	}

	int CMoniInfoBase::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < CMoniInfoBase::StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoBase len\n");
			nBufLen = CMoniInfoBase::StreamSize();
			return -1;
		}

		int nOffset = 0;
		//�������
		pBuf[nOffset] = m_byMoniType;
		nOffset += 1;

		return nOffset;
	}

	int CMoniInfoBase::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		//�������
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMoniInfoBase m_byMoniType len\n");
			return -1;
		}

		m_byMoniType = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//
	//                CLocalProcItoA
	//                ������Ϣ
	//
	//==============================================================================
	CLocalProcItoA::CLocalProcItoA()
	{

	}
	
	CLocalProcItoA::~CLocalProcItoA()
	{
	
	}
	
	void CLocalProcItoA::SetProcName(string strProcName)
	{
		m_strProcName = strProcName;
	}
	
	string CLocalProcItoA::GetProcName() const
	{
		return m_strProcName;
	}
	
	int CLocalProcItoA::StreamSize()
	{
		int nSize = 0;
		
		// ����������
		nSize += sizeof(BYTE);
		
		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strProcName.size());
		nSize += byLen;
		
		return nSize;
	}
	
	int CLocalProcItoA::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CLocalProcItoA len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		int nOffset = 0;
		
		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strProcName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������
		memcpy(pBuf + nOffset, m_strProcName.c_str(), byLen);
		nOffset += byLen;
		
		return nOffset;
	}
	
	int CLocalProcItoA::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CLocalProcItoA ProcName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CLocalProcItoA ProcName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strProcName = str0xff;
		nOffset += byLen;
		
		return nOffset;
	}

	//==============================================================================
	//
	//                CSingleHostCfg
	//                ��������������Ϣ
	//
	//==============================================================================
	CSingleHostCfg::CSingleHostCfg()
	{
		m_vecMoniTypeInfo.clear();
		m_vecLocalProcItoA.clear();
	}

	CSingleHostCfg::~CSingleHostCfg()
	{

	}

	void CSingleHostCfg::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CSingleHostCfg::GetHostName() const
	{
		return m_strHostName;
	}

	int CSingleHostCfg::GetMoniTypeInfoCount() const
	{
		return (int)m_vecMoniTypeInfo.size();
	}

	const CMoniInfoBase* CSingleHostCfg::GetMoniTypeInfo(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecMoniTypeInfo.size());
		return &m_vecMoniTypeInfo[nIndex];
	}

	void CSingleHostCfg::Add(const CMoniInfoBase& objMoniTypeInfo)
	{
		if (m_vecMoniTypeInfo.size() < 0xff)
		{
			m_vecMoniTypeInfo.push_back(objMoniTypeInfo);
		}
	}

	void CSingleHostCfg::Clear()
	{
		m_vecMoniTypeInfo.clear();
	}

	int CSingleHostCfg::GetLocalProcItoACount() const
	{
		return (int)m_vecLocalProcItoA.size();
	}

	const CLocalProcItoA* CSingleHostCfg::GetLocalProcItoA(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecLocalProcItoA.size());
		return &m_vecLocalProcItoA[nIndex];
	}

	void CSingleHostCfg::Add(const CLocalProcItoA& objLocalProcItoA)
	{
		if (m_vecLocalProcItoA.size() < 0xff)
		{
			m_vecLocalProcItoA.push_back(objLocalProcItoA);
		}
	}

	void CSingleHostCfg::CLocalProcItoAClear()
	{
		m_vecLocalProcItoA.clear();
	}

	int CSingleHostCfg::StreamSize()
	{
		// ����������
		int nSize = sizeof(BYTE);

		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;

		// ������͸���
		nSize += sizeof(BYTE);

		// �������
		byLen = (BYTE)MIN(0xff, m_vecMoniTypeInfo.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecMoniTypeInfo.at(i).StreamSize();
		}
		// ������Ϣ����
		nSize += sizeof(BYTE);

		// ������Ϣ
		byLen = (BYTE)MIN(0xff, m_vecLocalProcItoA.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecLocalProcItoA.at(i).StreamSize();
		}

		return nSize;
	}

	int CSingleHostCfg::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSingleHostCfg len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// ������͸���
		byLen = (BYTE)MIN(0xff, m_vecMoniTypeInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// �������
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniTypeInfo.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		// ������Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecLocalProcItoA.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecLocalProcItoA.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSingleHostCfg::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;

		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSingleHostCfg HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSingleHostCfg HostName len\n");
			return -1;
		}
		char strHostName[0xff+1];
		memcpy(strHostName, pBuf + nOffset, byLen);
		strHostName[byLen] = 0;
		m_strHostName = strHostName;
		nOffset += byLen;

		// ������͸���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSingleHostCfg MoniTypeInfo_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CMoniInfoBase objMoniTypeInfo;
			nRet = objMoniTypeInfo.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}

			m_vecMoniTypeInfo.push_back(objMoniTypeInfo);
			nOffset += nRet;
		}

		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSingleHostCfg LocalProcItoA_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CLocalProcItoA objLocalProcItoA;
			nRet = objLocalProcItoA.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}

			m_vecLocalProcItoA.push_back(objLocalProcItoA);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//                CHostsCfgAtoIPkt
	//                ��������������Ϣ��RmAppHostMgr��RmInterHost��
	//
	//==============================================================================
	CHostsCfgAtoIPkt::CHostsCfgAtoIPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_HOSTSMONICFG_APPTOINTER)
	{
		m_vecHostsCfg.clear();
	}

	CHostsCfgAtoIPkt::~CHostsCfgAtoIPkt()
	{

	}

	int CHostsCfgAtoIPkt::GetHostsCount() const
	{
		return (int)m_vecHostsCfg.size();
	}

	const CSingleHostCfg* CHostsCfgAtoIPkt::GetSingleHostCfg(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_vecHostsCfg.size())
		{
			return &m_vecHostsCfg[nIndex];
		}

		return NULL;
	}

	void CHostsCfgAtoIPkt::Add(const CSingleHostCfg& objSingleHostCfg)
	{
		if (m_vecHostsCfg.size() < 0xff)
		{
			m_vecHostsCfg.push_back(objSingleHostCfg);
		}
	}
	void CHostsCfgAtoIPkt::Clear()
	{
		m_vecHostsCfg.clear();
	}

	CProtocolHead* CHostsCfgAtoIPkt::Clone()
	{
		CHostsCfgAtoIPkt* pClone = new CHostsCfgAtoIPkt();
		*pClone = *this;
		return pClone;
	}

	int CHostsCfgAtoIPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();

		// ��������
		nSize += sizeof(BYTE);

		// ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecHostsCfg.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecHostsCfg.at(i).StreamSize();
		}

		return nSize;
	}

	int CHostsCfgAtoIPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostsCfgAtoIPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_vecHostsCfg.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// �������
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecHostsCfg.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CHostsCfgAtoIPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// ������ͳ���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostsCfgAtoIPkt MoniTypeInfo_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CSingleHostCfg objSingleHostCfg;
			nRet = objSingleHostCfg.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}

			m_vecHostsCfg.push_back(objSingleHostCfg);
			nOffset += nRet;
		}

		return nOffset;
	}
	
	//==============================================================================
	//
	//						CPU��������
	//
	//==============================================================================
	CMoniInfoCpuLoad::CMoniInfoCpuLoad()
		:m_fCPULoadPer(0.0f)
	{
		m_byMoniType = EMTT_CPU;
	}

	CMoniInfoCpuLoad::~CMoniInfoCpuLoad()
	{

	}

	void CMoniInfoCpuLoad::SetCPULoadPer(float fCPULoadPer)
	{
		m_fCPULoadPer = fCPULoadPer;
	}

	// ��ȡCPUռ����
	float CMoniInfoCpuLoad::GetCPULoadPer() const
	{
		return m_fCPULoadPer;
	}

	int CMoniInfoCpuLoad::StreamSize()
	{
		//CPUռ����
		return CMoniInfoBase::StreamSize() + 4;
	}

	int CMoniInfoCpuLoad::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CCpu len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��2��CPU
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//����CPU����ֵ
		SETSTREAMFLOAT(pBuf + nOffset, m_fCPULoadPer);
		nOffset += 4;		

		return nOffset;
	}

	int CMoniInfoCpuLoad::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��2��CPU
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		if (nBufLen < nOffset + 4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpu m_fCPULoadPer len\n");
			return -1;
		}

		//ȡCPU����ֵ
		GETSTREAMFLOAT(m_fCPULoadPer, pBuf + nOffset);
		nOffset += 4;

		return nOffset;
	}

	//==============================================================================
	//
	//						�ڴ�����
	//
	//==============================================================================
	CMoniInfoMemLoad::CMoniInfoMemLoad()
		:m_fMemLoadPer(0.0f)
	{
		m_byMoniType = EMTT_MEM;
	}

	CMoniInfoMemLoad::~CMoniInfoMemLoad()
	{

	}

	// �����ڴ�ռ����
	void CMoniInfoMemLoad::SetMemLoadPer(float fMemLoadPer)
	{
		m_fMemLoadPer = fMemLoadPer;
	}

	// ��ȡ�ڴ�ռ����
	float CMoniInfoMemLoad::GetMemLoadPer() const
	{
		return m_fMemLoadPer;
	}

	int CMoniInfoMemLoad::StreamSize()
	{
		//�ڴ�ռ����
		return CMoniInfoBase::StreamSize() + 4;
	}

	int CMoniInfoMemLoad::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMem len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��3��MEM
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//�ڴ�
		SETSTREAMFLOAT(pBuf + nOffset, m_fMemLoadPer);
		nOffset += 4;		

		return nOffset;
	}

	int CMoniInfoMemLoad::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��3��MEM
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		//�ڴ�
		if (nBufLen < nOffset + 4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMem m_fMemLoadPer len\n");
			return -1;
		}

		GETSTREAMFLOAT(m_fMemLoadPer, pBuf + nOffset);
		nOffset += 4;

		return nOffset;
	}

	//==============================================================================
	//
	//                CCPUAndMEMOfProcItoA
	//                ������Ϣ
	//
	//==============================================================================
	CCPUAndMEMOfProcItoA::CCPUAndMEMOfProcItoA()
		:m_dwPID(0),
		m_fCPULoadPer(0.0f),
		m_fMemLoadPer(0.0f)
	{

	}

	CCPUAndMEMOfProcItoA::~CCPUAndMEMOfProcItoA()
	{

	}

	void CCPUAndMEMOfProcItoA::SetProcName(string strProcName)
	{
		m_strProcName = strProcName;
	}

	string CCPUAndMEMOfProcItoA::GetProcName() const
	{
		return m_strProcName;
	}

	void CCPUAndMEMOfProcItoA::SetPID(DWORD dwPID)
	{
		m_dwPID = dwPID;
	}

	DWORD CCPUAndMEMOfProcItoA::GetPID() const
	{
		return m_dwPID;
	}

	void CCPUAndMEMOfProcItoA::SetCPULoadPer(float fCPULoadPer)
	{
		m_fCPULoadPer = fCPULoadPer;
	}

	float CCPUAndMEMOfProcItoA::GetCPULoadPer() const
	{
		return m_fCPULoadPer;
	}

	void CCPUAndMEMOfProcItoA::SetMemLoadPer(float fMemLoadPer)
	{
		m_fMemLoadPer = fMemLoadPer;
	}

	float CCPUAndMEMOfProcItoA::GetMemLoadPer() const
	{
		return m_fMemLoadPer;
	}

	int CCPUAndMEMOfProcItoA::StreamSize()
	{
		int nSize = 0;

		// ����������
		nSize += sizeof(BYTE);

		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strProcName.size());
		nSize += byLen;

		// ����ID
		nSize += sizeof(DWORD);

		// CPUռ����
		nSize += sizeof(float);

		// �ڴ�ռ����
		nSize += sizeof(float);

		return nSize;
	}

	int CCPUAndMEMOfProcItoA::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CCPUAndMEMOfProcItoA len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;
		int nOffset = 0;

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strProcName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������
		memcpy(pBuf + nOffset, m_strProcName.c_str(), byLen);
		nOffset += byLen;

		// ����ID
		SETSTREAMINT(pBuf + nOffset, m_dwPID);
		nOffset += sizeof(DWORD);

		// CPUռ����
		SETSTREAMFLOAT(pBuf + nOffset, m_fCPULoadPer);
		nOffset += sizeof(float);

		// �ڴ�ռ����
		SETSTREAMFLOAT(pBuf + nOffset, m_fMemLoadPer);
		nOffset += sizeof(float);

		return nOffset;
	}

	int CCPUAndMEMOfProcItoA::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCPUAndMEMOfProcItoA ProcName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCPUAndMEMOfProcItoA ProcName len\n");
			return -1;
		}
		char strProcName[0xff+1];
		memcpy(strProcName, pBuf + nOffset, byLen);
		strProcName[byLen] = 0;
		m_strProcName = strProcName;
		nOffset += byLen;

		// ����ID
		if(nBufLen < (nOffset + sizeof(DWORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCPUAndMEMOfProcItoA PID len\n");
			return -1;
		}
		m_dwPID = GETSTREAMINT(pBuf + nOffset);
		nOffset += sizeof(DWORD);

		// CPUռ����
		if(nBufLen < (nOffset + sizeof(float)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCPUAndMEMOfProcItoA CPULoadPer len\n");
			return -1;
		}
		GETSTREAMFLOAT(m_fCPULoadPer, pBuf + nOffset);
		nOffset += sizeof(float);

		// �ڴ�ռ����
		if(nBufLen < (nOffset + sizeof(float)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCPUAndMEMOfProcItoA MemLoadPer len\n");
			return -1;
		}
		GETSTREAMFLOAT(m_fMemLoadPer, pBuf + nOffset);
		nOffset += sizeof(float);

		return nOffset;
	}

	//==============================================================================
	//
	//					��������
	//
	//==============================================================================
	CMoniInfoProcess::CMoniInfoProcess()
	{
		m_byMoniType = EMTT_PROGRAM;
	}

	CMoniInfoProcess::~CMoniInfoProcess()
	{

	}

	int CMoniInfoProcess::GetCPUAndMEMOfProcItoACount() const
	{
		return (int)m_vecMoniInfoProcess.size();
	}

	CCPUAndMEMOfProcItoA* CMoniInfoProcess::GetCPUAndMEMOfProcItoA(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecMoniInfoProcess.size())
		{
			return &m_vecMoniInfoProcess[nIndex];
		}

		return NULL;
	}

	void CMoniInfoProcess::Add(const CCPUAndMEMOfProcItoA objCPUAndMEMOfProcItoA)
	{
		if (m_vecMoniInfoProcess.size() < 0xff)
		{
			m_vecMoniInfoProcess.push_back(objCPUAndMEMOfProcItoA);
		}
	}

	void CMoniInfoProcess::Clear()
	{
		m_vecMoniInfoProcess.clear();
	}

	int CMoniInfoProcess::StreamSize()
	{
		int nSize = CMoniInfoBase::StreamSize() + 1;

		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfoProcess.size());
		for (int i = 0 ; i < byLen ; i++)
		{
			nSize +=  m_vecMoniInfoProcess.at(i).StreamSize();
		}

		return nSize;
	}

	int CMoniInfoProcess::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CProcess len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��6��PROCESS
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLen);

		//������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfoProcess.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//������Ϣ
		for (int i=0; i<byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfoProcess.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoProcess::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecMoniInfoProcess.clear();
		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��6��PROCESS
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ������Ϣ����
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess byProcessNum len\n");
			return -1;
		}
		BYTE  byProcessNum = pBuf[nOffset];
		nOffset += 1;		

		// ������Ϣ
		for (int i=0; i<byProcessNum; i++)
		{
			CCPUAndMEMOfProcItoA objCPUAndMEMOfProcItoA;
			nRet = objCPUAndMEMOfProcItoA.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecMoniInfoProcess.push_back(objCPUAndMEMOfProcItoA);
			nOffset += nRet;
		}

		return nOffset;
	}
	
	//==============================================================================
	//
	//                CCpuAndMemItoAPkt
	//                ����������̵�CPU���ڴ�ʵʱ״̬��Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	CCpuAndMemItoAPkt::CCpuAndMemItoAPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_CPUANDMEMREALTIME_INTERTOAPP)
	{

	}
	
	CCpuAndMemItoAPkt::CCpuAndMemItoAPkt(const CCpuAndMemItoAPkt& objCpuAndMemItoAPkt)
	{
		*this = objCpuAndMemItoAPkt;
	}

	CCpuAndMemItoAPkt::~CCpuAndMemItoAPkt()
	{
		Clear();
	}
	
	CCpuAndMemItoAPkt& CCpuAndMemItoAPkt::operator=(const CCpuAndMemItoAPkt& objCpuAndMemItoAPkt)
	{
		*((CProtocolHead*)this) = *((CProtocolHead*)&objCpuAndMemItoAPkt);

		m_strHostName = objCpuAndMemItoAPkt.GetHostName();

		const CMoniInfoBase* pMoniInfoBase;
		int i;
		for (i = 0; i < objCpuAndMemItoAPkt.GetMoniInfoBaseCount(); i++)
		{
			pMoniInfoBase = objCpuAndMemItoAPkt.GetMoniInfoBase(i);
			Add(pMoniInfoBase);
		}
		return *this;
	}

	void CCpuAndMemItoAPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}
	
	string CCpuAndMemItoAPkt::GetHostName() const
	{
		return m_strHostName;
	}
	
	int CCpuAndMemItoAPkt::GetMoniInfoBaseCount() const
	{
		return (int)m_vecMoniInfo.size();
	}
	
	CMoniInfoBase* CCpuAndMemItoAPkt::GetMoniInfoBase(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecMoniInfo.size());
		return m_vecMoniInfo[nIndex];
	}
	
	void CCpuAndMemItoAPkt::Add(const CMoniInfoBase* pMoniInfoBase)
	{
		assert(pMoniInfoBase);

		CMoniInfoCpuLoad* pMoniInfoCpuLoad;
		CMoniInfoMemLoad* pMoniInfoMemLoad;
		CMoniInfoProcess* pMoniInfoProcess;

		switch (pMoniInfoBase->GetMoniType())
		{
		case EMTT_CPU:			//CPU����
			pMoniInfoCpuLoad = (CMoniInfoCpuLoad*)pMoniInfoBase;
			AddCPULoad(pMoniInfoCpuLoad->GetCPULoadPer());
			break;

		case EMTT_MEM:			//�ڴ�ռ��
			pMoniInfoMemLoad = (CMoniInfoMemLoad*)pMoniInfoBase;
			AddMEMLoad(pMoniInfoMemLoad->GetMemLoadPer());
			break;

		case EMTT_PROGRAM:			//����
			pMoniInfoProcess = (CMoniInfoProcess*)pMoniInfoBase;
			AddProcess(*pMoniInfoProcess);
			break;

		default:
			break;
		}
	}

	void CCpuAndMemItoAPkt::AddCPULoad(const float fCPULoad)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoCpuLoad* pInfo = new CMoniInfoCpuLoad;
		pInfo->SetCPULoadPer(fCPULoad);
		m_vecMoniInfo.push_back(pInfo);
	}

	void CCpuAndMemItoAPkt::AddMEMLoad(const float fMEMLoad)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoMemLoad* pInfo = new CMoniInfoMemLoad;
		pInfo->SetMemLoadPer(fMEMLoad);
		m_vecMoniInfo.push_back(pInfo);
	}

	void CCpuAndMemItoAPkt::AddProcess(const CMoniInfoProcess& objProcess)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoProcess* pInfo = new CMoniInfoProcess;
		*pInfo  = objProcess;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CCpuAndMemItoAPkt::Clear()
	{
		CMoniInfoBase* pItem;
		vector<CMoniInfoBase*>::iterator it;
		for (it = m_vecMoniInfo.begin(); it != m_vecMoniInfo.end(); it++)
		{
			pItem = *it;
			assert(pItem);
			delete pItem;
			pItem = NULL;
		}
		m_vecMoniInfo.clear();
	}
	
	CProtocolHead* CCpuAndMemItoAPkt::Clone()
	{
		CCpuAndMemItoAPkt* pClone = new CCpuAndMemItoAPkt();
		for (int i = 0; i < m_vecMoniInfo.size(); i++)
		{
			BYTE byType = m_vecMoniInfo.at(i)->GetMoniType();
			switch (byType)
			{
			case EMTT_CPU:			//CPU
				pClone->AddCPULoad(((CMoniInfoCpuLoad*)m_vecMoniInfo.at(i))->GetCPULoadPer());					
				break;

			case EMTT_MEM:			//�ڴ�
				pClone->AddMEMLoad(((CMoniInfoMemLoad*)m_vecMoniInfo.at(i))->GetMemLoadPer());	
				break;

			case EMTT_PROGRAM:			//����
				pClone->AddProcess(*((CMoniInfoProcess*)m_vecMoniInfo.at(i)));
				break;
			}
		}

		return pClone;
	}
	
	int CCpuAndMemItoAPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ����������
		nSize += sizeof(BYTE);
		
		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;
		
		// �����Ϣ����
		nSize += sizeof(BYTE);
		
		// �����Ϣ
		int nMoniInfoNum = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		for(int i = 0; i < nMoniInfoNum; i++)
		{
			//�����Ϣ����
			nSize += sizeof(WORD);

			//��Ϣ
			nSize += m_vecMoniInfo.at(i)->StreamSize();
		}
		
		return nSize;
	}
	
	int CCpuAndMemItoAPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CCpuAndMemItoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);
		
		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		//�����Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// �����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			WORD wMoniLen = (WORD)MIN(MAX_WORDSIZE, m_vecMoniInfo.at(i)->StreamSize());
			SETSTREAMSHORT(pBuf + nOffset, wMoniLen);
			nOffset += 2;

			//�����Ϣ
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfo.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CCpuAndMemItoAPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt HostName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostName = str0xff;
		nOffset += byLen;
		
		// �����Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE))) 
		{
			LogImportant("FromStreamBuf: Buflen is less than the number len of CCpuAndMemItoAPkt CMoniInfo\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;
		
		CMoniInfoCpuLoad* pMoniInfoCpuLoad;
		CMoniInfoMemLoad* pMoniInfoMemLoad;
		CMoniInfoProcess* pMoniInfoProcess;
		//�����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			if(nBufLen < (nOffset + sizeof(WORD)))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt CMoniInfo len\n");
				return -1;
			}
			WORD wMoniLen = GETSTREAMSHORT(pBuf + nOffset);
			nOffset += 2;

			//�����Ϣ����
			if(nBufLen < (nOffset + wMoniLen))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt CMoniInfo MoniType len\n");
				return -1;
			}
			BYTE byMoniType = pBuf[nOffset];
			//�˴�ƫ���������ӣ���Ϊ����������ڼ����Ϣ����

			switch (byMoniType)
			{
			case EMTT_CPU:			//CPU
				pMoniInfoCpuLoad = new CMoniInfoCpuLoad;
				nRet = pMoniInfoCpuLoad->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoCpuLoad);
				nOffset += nRet;
				break;

			case EMTT_MEM:			//�ڴ�
				pMoniInfoMemLoad = new CMoniInfoMemLoad;
				nRet = pMoniInfoMemLoad->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoMemLoad);
				nOffset += nRet;
				break;

			case EMTT_PROGRAM:			//����
				pMoniInfoProcess = new CMoniInfoProcess;
				nRet = pMoniInfoProcess->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoProcess);
				nOffset += nRet;
				break;

			default:
				break;
			}
		}
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//				 CMoniInfoOS
	//				 ����ϵͳ
	//
	//==============================================================================
	CMoniInfoOS::CMoniInfoOS()
	{
		m_byMoniType = EMTT_OS;
	}

	CMoniInfoOS::~CMoniInfoOS()
	{

	}

	void CMoniInfoOS::SetOSName(string strOSName)
	{
		m_strOSName = strOSName;
	}

	string CMoniInfoOS::GetOSName() const
	{
		return m_strOSName;
	}

	int CMoniInfoOS::StreamSize()
	{
		int nSize = CMoniInfoBase::StreamSize() + 1;
		nSize += (BYTE)MIN(0xff, m_strOSName.size());
		return nSize;
	}

	int CMoniInfoOS::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoOS len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��1��ϵͳ
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		// ����ϵͳ���Ƴ���
		BYTE byLen = (BYTE)MIN(0xff, m_strOSName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ����ϵͳ����
		memcpy(pBuf + nOffset, m_strOSName.c_str(), byLen);
		nOffset += byLen;

		return nOffset;
	}

	int CMoniInfoOS::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��1��ϵͳ
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ����ϵͳ���Ƴ���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostAllItoAPkt OSName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ����ϵͳ����
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostAllItoAPkt OSName len\n");
			return -1;
		}
		char strOSName[0xff+1];
		memcpy(strOSName, pBuf + nOffset, byLen);
		strOSName[byLen] = 0;
		m_strOSName = strOSName;
		nOffset += byLen;

		return nOffset;
	}

	//==============================================================================
	//
	//						CPU��������
	//
	//==============================================================================
	CMoniInfoCpuNum::CMoniInfoCpuNum()
		:m_byCPUNum(0)
	{
		m_byMoniType = EMTT_CPU;
	}

	CMoniInfoCpuNum::~CMoniInfoCpuNum()
	{

	}

	void CMoniInfoCpuNum::SetCPUNum(BYTE byCPUNum)
	{
		m_byCPUNum = byCPUNum;
	}

	BYTE CMoniInfoCpuNum::GetCPUNum() const
	{
		return m_byCPUNum;
	}

	int CMoniInfoCpuNum::StreamSize()
	{
		//CPU����
		return CMoniInfoBase::StreamSize() + 1;
	}

	int CMoniInfoCpuNum::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoCpuNum len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��2��CPU
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//����CPU����
		pBuf[nOffset] =  m_byCPUNum;
		nOffset += 1;		

		return nOffset;
	}

	int CMoniInfoCpuNum::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��2��CPU
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpu m_byCPUNum len\n");
			return -1;
		}

		//ȡCPU����
		m_byCPUNum =  pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//
	//						�ڴ���������
	//
	//==============================================================================
	CMoniInfoMemCap::CMoniInfoMemCap()
		:m_fMemCap(0.0f)
	{
		m_byMoniType = EMTT_MEM;
	}

	CMoniInfoMemCap::~CMoniInfoMemCap()
	{

	}

	void CMoniInfoMemCap::SetMemCap(float fMemCap)
	{
		m_fMemCap = fMemCap;
	}

	float CMoniInfoMemCap::GetMemCap() const
	{
		return m_fMemCap;
	}

	int CMoniInfoMemCap::StreamSize()
	{
		//�ڴ�����
		return CMoniInfoBase::StreamSize() + 4;
	}

	int CMoniInfoMemCap::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoMemCap len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��3��MEM
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);
		
		//�ڴ�����
		SETSTREAMFLOAT(pBuf + nOffset, m_fMemCap);
		nOffset += 4;		

		return nOffset;
	}

	int CMoniInfoMemCap::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��3��MEM
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("ToStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		//�ڴ�����
		if (nBufLen < nOffset + 4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMem CMoniInfoMemCap len\n");
			return -1;
		}

		GETSTREAMFLOAT(m_fMemCap, pBuf + nOffset);
		nOffset += 4;

		return nOffset;
	}


	//==============================================================================
	//
	//                CDisk
	//                ������Ϣ
	//
	//==============================================================================
	CDisk::CDisk()
		:m_fDiskCap(0.0f), m_fDiskLoadPer(0.0f)
	{

	}
	
	CDisk::~CDisk()
	{
	
	}
	
	void CDisk::SetDiskName(string strDiskName)
	{
		m_strDiskName = strDiskName;
	}
	
	string CDisk::GetDiskName() const
	{
		return m_strDiskName;
	}
	
	void CDisk::SetDiskCap(float fDiskCap)
	{
		m_fDiskCap = fDiskCap;
	}
	
	float CDisk::GetDiskCap() const
	{
		return m_fDiskCap;
	}
	
	void CDisk::SetDiskLoadPer(float fDiskLoadPer)
	{
		m_fDiskLoadPer = fDiskLoadPer;
	}
	
	float CDisk::GetDiskLoadPer() const
	{
		return m_fDiskLoadPer;
	}
	
	int CDisk::StreamSize()
	{
		int nSize = 0;
		
		// �������Ƴ���
		nSize += sizeof(BYTE);
		
		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_strDiskName.size());
		nSize += byLen;
		
		// ��������
		nSize += sizeof(float);
		
		// ����ʹ����
		nSize += sizeof(float);
		
		return nSize;
	}
	
	int CDisk::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CDisk len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		int nOffset = 0;
		
		// �������Ƴ���
		BYTE byLen = (BYTE)MIN(0xff, m_strDiskName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������
		memcpy(pBuf + nOffset, m_strDiskName.c_str(), byLen);
		nOffset += byLen;
		
		// ��������
		SETSTREAMFLOAT(pBuf + nOffset, m_fDiskCap);
		nOffset += sizeof(float);
		
		// ����ʹ����
		SETSTREAMFLOAT(pBuf + nOffset, m_fDiskLoadPer);
		nOffset += sizeof(float);
		
		return nOffset;
	}
	
	int CDisk::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// �������Ƴ���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDisk DiskName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDisk DiskName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strDiskName = str0xff;
		nOffset += byLen;
		
		// ��������
		if(nBufLen < (nOffset + sizeof(float)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDisk DiskCap len\n");
			return -1;
		}
		GETSTREAMFLOAT(m_fDiskCap, pBuf + nOffset);
		nOffset += sizeof(float);
		
		// ����ʹ����
		if(nBufLen < (nOffset + sizeof(float)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDisk DiskLoadPer len\n");
			return -1;
		}
		GETSTREAMFLOAT(m_fDiskLoadPer, pBuf + nOffset);
		nOffset += sizeof(float);
		
		return nOffset;
	}

	//==============================================================================
	//
	//             CMoniInfoDisk
	//             �̳м�����Ϣ����
	//
	//==============================================================================
	CMoniInfoDisk::CMoniInfoDisk()
	{
		m_byMoniType = EMTT_DISK;
	}

	CMoniInfoDisk::~CMoniInfoDisk()
	{

	}

	int CMoniInfoDisk::GetDiskCount() const
	{
		return (int)m_vecDisk.size();
	}

	CDisk* CMoniInfoDisk::GetDisk(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecDisk.size())
		{
			return &m_vecDisk[nIndex];
		}
		
		return NULL;
	}

	void CMoniInfoDisk::Add(const CDisk& objDisk)
	{
		if (m_vecDisk.size() < 0xff)
		{
			m_vecDisk.push_back(objDisk);
		}
	}

	void CMoniInfoDisk::Clear()
	{
		m_vecDisk.clear();
	}

	int CMoniInfoDisk::StreamSize()
	{
		//���ͳ���
		int nSize = CMoniInfoBase::StreamSize();

		//��������
		nSize += sizeof(BYTE);

		//����
		BYTE byLen = (BYTE)MIN(0xff, m_vecDisk.size());
		for (int i = 0 ; i < byLen ; i++)
		{
			//ÿ�����̳���
			nSize +=  m_vecDisk.at(i).StreamSize();
		}

		//���ͳ���+��������+������*ÿ�����̳��ȣ�
		return nSize;
	}

	int CMoniInfoDisk::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoDisk len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��4��DISK
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLen);
	
		//������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecDisk.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//������Ϣ
		for (int i=0; i<byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecDisk.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoDisk::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecDisk.clear();
		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��4��DISK
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nOffset < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ������Ϣ����
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess CMoniInfoDisk len\n");
			return -1;
		}
		BYTE  byDiskNum = pBuf[nOffset];
		nOffset += 1;		

		// ����
		for (int i=0; i<byDiskNum; i++)
		{
			CDisk objDisk;
			int nRet = objDisk.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecDisk.push_back(objDisk);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//                CIPInfo
	//                IP��ַ��Ϣ
	//
	//==============================================================================
	CIPInfo::CIPInfo()
	{

	}
	
	CIPInfo::~CIPInfo()
	{
	
	}
	
	void CIPInfo::SetIP(string strIP)
	{
		m_strIP = strIP;
	}
	
	string CIPInfo::GetIP() const
	{
		return m_strIP;
	}
	
	void CIPInfo::SetMask(string strMask)
	{
		m_strMask = strMask;
	}
	
	string CIPInfo::GetMask() const
	{
		return m_strMask;
	}
	
	int CIPInfo::StreamSize()
	{
		int nSize = 0;
		
		// IP��ַ����
		nSize += sizeof(BYTE);
		// IP��ַ
		BYTE byLen = (BYTE)MIN(0xff, m_strIP.size());
		nSize += byLen;

		// �������볤��
		nSize += sizeof(BYTE);
		// ��������
		byLen = (BYTE)MIN(0xff, m_strMask.size());
		nSize += byLen;
		
		return nSize;
	}
	
	int CIPInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CIPInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		int nOffset = 0;
		
		// IP��ַ����
		BYTE byLen = (BYTE)MIN(0xff, m_strIP.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// IP��ַ
		memcpy(pBuf + nOffset, m_strIP.c_str(), byLen);
		nOffset += byLen;
		
		// �������볤��
		byLen = (BYTE)MIN(0xff, m_strMask.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������
		memcpy(pBuf + nOffset, m_strMask.c_str(), byLen);
		nOffset += byLen;
		
		return nOffset;
	}
	
	int CIPInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// IP��ַ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CIPInfo IP_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// IP��ַ
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CIPInfo IP len\n");
			return -1;
		}
		char strTmp[0xff+1];
		memcpy(strTmp, pBuf + nOffset, byLen);
		strTmp[byLen] = 0;
		m_strIP = strTmp;
		nOffset += byLen;
		
		// �������볤��
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CIPInfo Mask_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CIPInfo Mask len\n");
			return -1;
		}
		memcpy(strTmp, pBuf + nOffset, byLen);
		strTmp[byLen] = 0;
		m_strMask = strTmp;
		nOffset += byLen;
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//                CNetCard
	//                ������Ϣ
	//
	//==============================================================================
	CNetCard::CNetCard()
		:m_byCardState(0)
	{

	}
	
	CNetCard::~CNetCard()
	{
	
	}
	
	void CNetCard::SetCardName(string strCardName)
	{
		m_strCardName = strCardName;
	}
	
	string CNetCard::GetCardName() const
	{
		return m_strCardName;
	}
	
	void CNetCard::SetCardState(BYTE byCardState)
	{
		m_byCardState = byCardState;
	}
	
	BYTE CNetCard::GetCardState() const
	{
		return m_byCardState;
	}
	
	int CNetCard::GetIPInfoCount() const
	{
		return (int)m_vecIPInfo.size();
	}
	
	const CIPInfo* CNetCard::GetIPInfo(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_vecIPInfo.size())
		{
			return &m_vecIPInfo[nIndex];
		}
		
		return NULL;
	}
	
	void CNetCard::Add(const CIPInfo& objIPInfo)
	{
		if (m_vecIPInfo.size() < 0xff)
		{
			m_vecIPInfo.push_back(objIPInfo);
		}
	}

	void CNetCard::Clear()
	{
		m_vecIPInfo.clear();
	}

	int CNetCard::StreamSize()
	{
		int nSize = 0;
		
		// �������Ƴ���
		nSize += sizeof(BYTE);
		
		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_strCardName.size());
		nSize += byLen;
		
		// ����״̬
		nSize += sizeof(BYTE);

		// IP��ַ��Ϣ��������
		nSize += sizeof(BYTE);
		
		// IP��ַ��Ϣ
		byLen = (BYTE)MIN(0xff, m_vecIPInfo.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecIPInfo.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CNetCard::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CNetCard len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		int nOffset = 0;
		
		// �������Ƴ���
		BYTE byLen = (BYTE)MIN(0xff, m_strCardName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������
		memcpy(pBuf + nOffset, m_strCardName.c_str(), byLen);
		nOffset += byLen;
		
		// ����״̬
		pBuf[nOffset] = m_byCardState;
		nOffset += 1;
		
		// IP��ַ����
		byLen = (BYTE)MIN(0xff, m_vecIPInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// IP��ַ��Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecIPInfo.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CNetCard::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecIPInfo.clear();

		int nRet, nOffset = 0;
		// �������Ƴ���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CNetCard CardName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CNetCard CardName len\n");
			return -1;
		}
		char strCardName[0xff+1];
		memcpy(strCardName, pBuf + nOffset, byLen);
		strCardName[byLen] = 0;
		m_strCardName = strCardName;
		nOffset += byLen;
		
		// ����״̬
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CNetCard CardState len\n");
			return -1;
		}
		m_byCardState = pBuf[nOffset];
		nOffset += 1;
		
		// IP��ַ��Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CNetCard IPInfo_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CIPInfo objIPInfo;
			nRet = objIPInfo.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			m_vecIPInfo.push_back(objIPInfo);
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//             CMoniInfoNetCard
	//             �̳м�����Ϣ����
	//
	//==============================================================================
	CMoniInfoNetCard::CMoniInfoNetCard()
	{
		m_byMoniType = EMTT_NETWORK;
	}

	CMoniInfoNetCard::~CMoniInfoNetCard()
	{

	}

	int CMoniInfoNetCard::GetMoniInfoNetCardCount() const
	{
		return (int)m_vecMoniInfoNetCard.size();
	}

	const CNetCard* CMoniInfoNetCard::GetMoniInfoNetCard(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_vecMoniInfoNetCard.size())
		{
			return &m_vecMoniInfoNetCard[nIndex];
		}
		
		return NULL;
	}

	void CMoniInfoNetCard::Add(const CNetCard& objNetCard)
	{
		if (m_vecMoniInfoNetCard.size() < 0xff)
		{
			m_vecMoniInfoNetCard.push_back(objNetCard);
		}
	}

	void CMoniInfoNetCard::Clear()
	{
		m_vecMoniInfoNetCard.clear();
	}

	int CMoniInfoNetCard::StreamSize()
	{
		//���ͳ���
		int nSize = CMoniInfoBase::StreamSize();

		//��������
		nSize += sizeof(BYTE);

		//����
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfoNetCard.size());
		for (int i = 0 ; i < byLen ; i++)
		{
			//ÿ��������Ϣ����
			nSize +=  m_vecMoniInfoNetCard.at(i).StreamSize();
		}

		return nSize;
	}

	int CMoniInfoNetCard::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoNetCard len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��5��NETWORK
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfoNetCard.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//������Ϣ
		for (int i=0; i<byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfoNetCard.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoNetCard::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecMoniInfoNetCard.clear();

		int nOffset = 0;
		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��5��NETWORK
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ������Ϣ����
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess CMoniInfoNetCard len\n");
			return -1;
		}
		BYTE byNetCardNum = pBuf[nOffset];
		nOffset += 1;	

		// ������Ϣ
		for (int i=0; i<byNetCardNum; i++)
		{
			CNetCard objNetCard;
			int nRet = objNetCard.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecMoniInfoNetCard.push_back(objNetCard);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//                CAllHostProcItoA
	//                ������Ϣ
	//
	//==============================================================================
	CAllHostProcItoA::CAllHostProcItoA()
		:m_dwPID(0),
		m_fProcFileSize(0.0f),
		m_byProcState(0)
	{

	}
	
	CAllHostProcItoA::~CAllHostProcItoA()
	{
	
	}
	
	void CAllHostProcItoA::SetProcName(const string& strProcName)
	{
		m_strProcName = strProcName;
	}
	
	string CAllHostProcItoA::GetProcName() const
	{
		return m_strProcName;
	}
	
	void CAllHostProcItoA::SetPID(DWORD dwPID)
	{
		m_dwPID = dwPID;
	}
	
	DWORD CAllHostProcItoA::GetPID() const
	{
		return m_dwPID;
	}
	
	void CAllHostProcItoA::SetProcPath(const string& strProcPath)
	{
		m_strProcPath = strProcPath;
	}
	
	string CAllHostProcItoA::GetProcPath() const
	{
		return m_strProcPath;
	}
	
	void CAllHostProcItoA::SetProcVer(const string& strProcVer)
	{
		m_strProcVer = strProcVer;
	}
	
	string CAllHostProcItoA::GetProcVer() const
	{
		return m_strProcVer;
	}
	
	void CAllHostProcItoA::SetProcFileSize(float fProcFileSize)
	{
		m_fProcFileSize = fProcFileSize;
	}
	
	float CAllHostProcItoA::GetProcFileSize() const
	{
		return m_fProcFileSize;
	}
	
	void CAllHostProcItoA::SetCreatTime(time_t& tm, WORD wMs)
	{
		m_CreatTime.Set(tm, wMs);
	}

	CProtocolTime CAllHostProcItoA::GetCreatProtocolTime() const
	{
		return m_CreatTime;
	}

	void CAllHostProcItoA::SetCreatTime(timeval tTime)
	{
		m_CreatTime.Set(tTime);
	}

	timeval CAllHostProcItoA::GetCreatTime() const
	{
		return m_CreatTime.GetTimeval();
	}

	void CAllHostProcItoA::SetModifyTime(time_t& tm, WORD wMs)
	{
		m_ModifyTime.Set(tm, wMs);
	}

	CProtocolTime CAllHostProcItoA::GetModifyProtocolTime() const
	{
		return m_ModifyTime;
	}

	void CAllHostProcItoA::SetModifyTime(timeval tTime)
	{
		m_ModifyTime.Set(tTime);
	}

	timeval CAllHostProcItoA::GetModifyTime() const
	{
		return m_ModifyTime.GetTimeval();
	}

	void CAllHostProcItoA::SetStartTime(time_t& tm, WORD wMs)
	{
		m_StartTime.Set(tm, wMs);
	}

	CProtocolTime CAllHostProcItoA::GetStartProtocolTime() const
	{
		return m_StartTime;
	}

	void CAllHostProcItoA::SetStartTime(timeval tTime)
	{
		m_StartTime.Set(tTime);
	}

	timeval CAllHostProcItoA::GetStartTime() const
	{
		return m_StartTime.GetTimeval();
	}

	void CAllHostProcItoA::SetProcState(BYTE byProcState)
	{
		m_byProcState = byProcState;
	}
	
	BYTE CAllHostProcItoA::GetProcState() const
	{
		return m_byProcState;
	}
	
	int CAllHostProcItoA::StreamSize()
	{
		int nSize = 0;
		
		// �������Ƴ���
		nSize += sizeof(BYTE);
		
		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_strProcName.size());
		nSize += byLen;
		
		// ����ID
		nSize += sizeof(DWORD);
		
		// ����·������
		nSize += sizeof(BYTE);
		
		// ����·��
		byLen = (BYTE)MIN(0xff, m_strProcPath.size());
		nSize += byLen;
		
		// ���̰汾����
		nSize += sizeof(BYTE);
		
		// ���̰汾
		byLen = (BYTE)MIN(0xff, m_strProcVer.size());
		nSize += byLen;
		
		// �����ļ���С
		nSize += sizeof(float);

		//�����ļ�����ʱ��
		nSize += m_CreatTime.StreamSize();

		//�����ļ��޸�ʱ��
		nSize += m_ModifyTime.StreamSize();
		
		// ����״̬
		nSize += sizeof(BYTE);

		//��������ʱ��
		nSize += m_StartTime.StreamSize();
		
		return nSize;
	}
	
	int CAllHostProcItoA::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CAllHostProcItoA len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		int nOffset = 0;
		
		// �������Ƴ���
		BYTE byLen = (BYTE)MIN(0xff, m_strProcName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������
		memcpy(pBuf + nOffset, m_strProcName.c_str(), byLen);
		nOffset += byLen;
		
		// ����ID
		SETSTREAMINT(pBuf + nOffset, m_dwPID);
		nOffset += sizeof(DWORD);
		
		// ����·������
		byLen = (BYTE)MIN(0xff, m_strProcPath.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ����·��
		memcpy(pBuf + nOffset, m_strProcPath.c_str(), byLen);
		nOffset += byLen;
		
		// ���̰汾����
		byLen = (BYTE)MIN(0xff, m_strProcVer.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ���̰汾
		memcpy(pBuf + nOffset, m_strProcVer.c_str(), byLen);
		nOffset += byLen;
		
		// �����ļ���С
		SETSTREAMFLOAT(pBuf + nOffset, m_fProcFileSize);
		nOffset += sizeof(float);

		//�����ļ�����ʱ��
		nBufLeft = nBufLen - nOffset;
		nOffset += m_CreatTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		//�����ļ��޸�ʱ��
		nBufLeft = nBufLen - nOffset;
		nOffset += m_ModifyTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		// ����״̬
		pBuf[nOffset] = m_byProcState;
		nOffset += 1;

		//��������ʱ��
		nBufLeft = nBufLen - nOffset;
		nOffset += m_StartTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		return nOffset;
	}
	
	int CAllHostProcItoA::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// �������Ƴ���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcName len\n");
			return -1;
		}
		char strTmp[0xff+1];
		memcpy(strTmp, pBuf + nOffset, byLen);
		strTmp[byLen] = 0;
		m_strProcName = strTmp;
		nOffset += byLen;
		
		// ����ID
		if(nBufLen < (nOffset + sizeof(DWORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA PID len\n");
			return -1;
		}
		m_dwPID = GETSTREAMINT(pBuf + nOffset);
		nOffset += sizeof(DWORD);
		
		// ����·������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcPath_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ����·��
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcPath len\n");
			return -1;
		}
		memcpy(strTmp, pBuf + nOffset, byLen);
		strTmp[byLen] = 0;
		m_strProcPath = strTmp;
		nOffset += byLen;
		
		// ���̰汾����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcVer_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ���̰汾
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcVer len\n");
			return -1;
		}
		memcpy(strTmp, pBuf + nOffset, byLen);
		strTmp[byLen] = 0;
		m_strProcVer = strTmp;
		nOffset += byLen;
		
		// �����ļ���С
		if(nBufLen < (nOffset + sizeof(float)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcFileSize len\n");
			return -1;
		}
		GETSTREAMFLOAT(m_fProcFileSize,pBuf + nOffset);
		nOffset += sizeof(float);

		// �����ļ�����ʱ��
		int nRet = m_CreatTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// �����ļ��޸�ʱ��
		nRet = m_ModifyTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		// ����״̬
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CAllHostProcItoA ProcState len\n");
			return -1;
		}
		m_byProcState = pBuf[nOffset];
		nOffset += 1;

		// ��������ʱ��
		nRet = m_StartTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}
	
	//==============================================================================
	//
	//             CMoniInfoAllHostProc
	//            �̳м�����Ϣ����
	//
	//==============================================================================
	CMoniInfoAllHostProc::CMoniInfoAllHostProc()
	{
		m_byMoniType = EMTT_PROGRAM;
	}

	CMoniInfoAllHostProc::~CMoniInfoAllHostProc()
	{

	}

	int CMoniInfoAllHostProc::GetAllHostProcItoACount() const
	{
		return (int)m_vecAllHostProcItoA.size();
	}

	CAllHostProcItoA* CMoniInfoAllHostProc::GetAllHostProcItoA(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecAllHostProcItoA.size())
		{
			return &m_vecAllHostProcItoA[nIndex];
		}
		
		return NULL;
	}

	void CMoniInfoAllHostProc::Add(const CAllHostProcItoA& objNetCard)
	{
		if (m_vecAllHostProcItoA.size() < 0xff)
		{
			m_vecAllHostProcItoA.push_back(objNetCard);
		}
	}

	void CMoniInfoAllHostProc::Clear()
	{
		m_vecAllHostProcItoA.clear();
	}

	int CMoniInfoAllHostProc::StreamSize()
	{
		//���ͳ���
		int nSize = CMoniInfoBase::StreamSize();

		//��������
		nSize += sizeof(BYTE);

		//����
		BYTE byLen = (BYTE)MIN(0xff, m_vecAllHostProcItoA.size());
		for (int i = 0 ; i < byLen ; i++)
		{
			//ÿ��������Ϣ����
			nSize +=  m_vecAllHostProcItoA.at(i).StreamSize();
		}

		return nSize;
	}

	int CMoniInfoAllHostProc::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoAllHostProc len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��6��PROGRAM
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecAllHostProcItoA.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//������Ϣ
		for (int i=0; i<byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecAllHostProcItoA.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoAllHostProc::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecAllHostProcItoA.clear();

		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��6��PROGRAM
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ������Ϣ����
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess CMoniInfoAllHostProc len\n");
			return -1;
		}
		BYTE  byProcessNum = pBuf[nOffset];
		nOffset += 1;		

		// ������Ϣ
		for (int i=0; i<byProcessNum; i++)
		{
			CAllHostProcItoA objAllHostProcItoA;
			nRet = objAllHostProcItoA.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecAllHostProcItoA.push_back(objAllHostProcItoA);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//                CHostAllItoAPkt
	//                ȫ������״̬��Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	CHostAllItoAPkt::CHostAllItoAPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_HOSTALL_INTERTOAPP)
	{
	
	}
	
	CHostAllItoAPkt::CHostAllItoAPkt(const CHostAllItoAPkt& objHostAllItoAPkt)
	{
		*this = objHostAllItoAPkt;
	}

	CHostAllItoAPkt::~CHostAllItoAPkt()
	{
		Clear();
	}
	
	CHostAllItoAPkt& CHostAllItoAPkt::operator=(const CHostAllItoAPkt& objHostAllItoAPkt)
	{
		m_strHostName = objHostAllItoAPkt.GetHostName();

		const CMoniInfoBase* pMoniInfoBase;
		int nCount = objHostAllItoAPkt.GetMoniInfoBaseCount();
		for (int i = 0; i < nCount; i++)
		{
			pMoniInfoBase = objHostAllItoAPkt.m_vecMoniInfo.at(i);
			Add(pMoniInfoBase);
		}
		return *this;
	}

	void CHostAllItoAPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}
	
	string CHostAllItoAPkt::GetHostName() const
	{
		return m_strHostName;
	}
	
	int CHostAllItoAPkt::GetMoniInfoBaseCount() const
	{
		return (int)m_vecMoniInfo.size();
	}

	CMoniInfoBase* CHostAllItoAPkt::GetMoniInfoBase(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_vecMoniInfo.size())
		{
			return m_vecMoniInfo[nIndex];
		}
		
		return NULL;
	}

	void CHostAllItoAPkt::Add(const CMoniInfoBase* pMoniInfoBase)
	{
		assert(pMoniInfoBase);

		CMoniInfoOS* pMoniInfoOS;
		CMoniInfoCpuNum* pMoniInfoCpuNum;
		CMoniInfoMemCap* pMoniInfoMemCap;
		CMoniInfoDisk* pMoniInfoDisk;
		CMoniInfoNetCard* pMoniInfoNetCard;
		CMoniInfoAllHostProc* pMoniInfoAllHostProc;

		switch (pMoniInfoBase->GetMoniType())
		{
		case EMTT_OS:			//����ϵͳ
			pMoniInfoOS = (CMoniInfoOS*)pMoniInfoBase;
			AddOS(pMoniInfoOS->GetOSName());
			break;

		case EMTT_CPU:			//CPU����
			pMoniInfoCpuNum = (CMoniInfoCpuNum*)pMoniInfoBase;
			AddCPUNum(pMoniInfoCpuNum->GetCPUNum());
			break;

		case EMTT_MEM:			//�ڴ�����
			pMoniInfoMemCap = (CMoniInfoMemCap*)pMoniInfoBase;
			AddMEMCap(pMoniInfoMemCap->GetMemCap());
			break;

		case EMTT_DISK:			//����
			pMoniInfoDisk = (CMoniInfoDisk*)pMoniInfoBase;
			AddDisk(pMoniInfoDisk);
			break;

		case EMTT_NETWORK:		//����
			pMoniInfoNetCard = (CMoniInfoNetCard*)pMoniInfoBase;
			AddNetCard(pMoniInfoNetCard);
			break;

		case EMTT_PROGRAM:			//����
			pMoniInfoAllHostProc = (CMoniInfoAllHostProc*)pMoniInfoBase;
			AddProcess(pMoniInfoAllHostProc);
			break;

		default:
			break;
		}
	}

	//os
	void CHostAllItoAPkt::AddOS(string sOSName)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoOS* pInfo = new CMoniInfoOS;
		pInfo->SetOSName(sOSName);
		m_vecMoniInfo.push_back(pInfo);
	}

	//cpu
	void CHostAllItoAPkt::AddCPUNum(BYTE byCPUNum)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoCpuNum* pInfo = new CMoniInfoCpuNum;
		pInfo->SetCPUNum(byCPUNum);
		m_vecMoniInfo.push_back(pInfo);
	}

	//mem
	void CHostAllItoAPkt::AddMEMCap(float fMEMCap)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoMemCap* pInfo = new CMoniInfoMemCap;
		pInfo->SetMemCap(fMEMCap);
		m_vecMoniInfo.push_back(pInfo);
	}

	//disk
	void CHostAllItoAPkt::AddDisk(CMoniInfoDisk* pMoniInfoDisk)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoDisk* pInfo = new CMoniInfoDisk;
		*pInfo  = *pMoniInfoDisk;
		m_vecMoniInfo.push_back(pInfo);
	}

	//netcard
	void CHostAllItoAPkt::AddNetCard(CMoniInfoNetCard* pMoniInfoNetCard)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoNetCard* pInfo = new CMoniInfoNetCard;
		*pInfo  = *pMoniInfoNetCard;
		m_vecMoniInfo.push_back(pInfo);
	}

	//process
	void CHostAllItoAPkt::AddProcess(CMoniInfoAllHostProc* pMoniInfoAllHostProc)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoAllHostProc* pInfo = new CMoniInfoAllHostProc;
		*pInfo  = *pMoniInfoAllHostProc;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CHostAllItoAPkt::Clear()
	{
		CMoniInfoBase* pItem;
		vector<CMoniInfoBase*>::iterator it;
		for (it = m_vecMoniInfo.begin(); it != m_vecMoniInfo.end(); it++)
		{
			pItem = *it;
			assert(pItem);
			delete pItem;
			pItem = NULL;
		}
		m_vecMoniInfo.clear();
	}

	CProtocolHead* CHostAllItoAPkt::Clone()
	{
		CHostAllItoAPkt* pClone = new CHostAllItoAPkt();
		for (int i = 0; i < m_vecMoniInfo.size(); i++)
		{
			BYTE byType = m_vecMoniInfo.at(i)->GetMoniType();
			switch (byType)
			{
			case EMTT_OS:			//OS
				pClone->AddOS(((CMoniInfoOS*)m_vecMoniInfo.at(i))->GetOSName());
				break;

			case EMTT_CPU:			//CPU
				pClone->AddCPUNum(((CMoniInfoCpuNum*)m_vecMoniInfo.at(i))->GetCPUNum());					
				break;

			case EMTT_MEM:			//�ڴ�
				pClone->AddMEMCap(((CMoniInfoMemCap*)m_vecMoniInfo.at(i))->GetMemCap());	
				break;

			case EMTT_DISK:			//����
				pClone->AddDisk((CMoniInfoDisk*)m_vecMoniInfo.at(i));	
				break;

			case EMTT_NETWORK:			//����
				pClone->AddNetCard((CMoniInfoNetCard*)m_vecMoniInfo.at(i));	
				break;

			case EMTT_PROGRAM:			//����
				pClone->AddProcess((CMoniInfoAllHostProc*)m_vecMoniInfo.at(i));
				break;
			}
		}

		return pClone;
	}
	
	int CHostAllItoAPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ����������
		nSize += sizeof(BYTE);
		
		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;

		// �����Ϣ����
		nSize += sizeof(BYTE);

		// �����Ϣ
		int nMoniInfoNum = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		for(int i = 0; i < nMoniInfoNum; i++)
		{
			//�����Ϣ����
			nSize += sizeof(WORD);

			//��Ϣ
			nSize += m_vecMoniInfo.at(i)->StreamSize();
		}	
		return nSize;
	}
	
	int CHostAllItoAPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostAllItoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		//�����Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// �����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			WORD wMoniLen = (WORD)MIN(MAX_WORDSIZE, m_vecMoniInfo.at(i)->StreamSize());
			SETSTREAMSHORT(pBuf + nOffset, wMoniLen);
			nOffset += 2;

			//�����Ϣ
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfo.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}
	
	int CHostAllItoAPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt HostName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostName = str0xff;
		nOffset += byLen;

		// �����Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the number len of CCpuAndMemItoAPkt CMoniInfo\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		CMoniInfoOS*	  pMoniInfoOS;	
		CMoniInfoCpuNum* pMoniInfoCpuNum;
		CMoniInfoMemCap* pMoniInfoMemCap;
		CMoniInfoDisk*    pMoniInfoDisk;
		CMoniInfoNetCard* pMoniInfoNetCard;
		CMoniInfoAllHostProc* pMoniInfoAllHostProc;
		//�����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			if(nBufLen < (nOffset + sizeof(WORD)))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt CMoniInfo len\n");
				return -1;
			}
			WORD wMoniLen = GETSTREAMSHORT(pBuf + nOffset);
			nOffset += 2;

			//�����Ϣ����
			if(nBufLen < (nOffset + wMoniLen))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt CMoniInfo MoniType len\n");
				return -1;
			}
			BYTE byMoniType = pBuf[nOffset];
			//�˴�ƫ���������ӣ���Ϊ����������ڼ����Ϣ����

			switch (byMoniType)
			{
			case EMTT_OS:		//OS
				pMoniInfoOS = new CMoniInfoOS;
				nRet = pMoniInfoOS->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoOS);
				nOffset += nRet;
				break;
			case EMTT_CPU:			//CPU
				pMoniInfoCpuNum = new CMoniInfoCpuNum;
				nRet = pMoniInfoCpuNum->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoCpuNum);
				nOffset += nRet;
				break;

			case EMTT_MEM:			//�ڴ�
				pMoniInfoMemCap = new CMoniInfoMemCap;
				nRet = pMoniInfoMemCap->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoMemCap);
				nOffset += nRet;
				break;

			case EMTT_DISK:		//����
				pMoniInfoDisk = new CMoniInfoDisk;
				nRet = pMoniInfoDisk->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoDisk);
				nOffset += nRet;
				break;
			case EMTT_NETWORK:		//����
				pMoniInfoNetCard = new CMoniInfoNetCard;
				nRet = pMoniInfoNetCard->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoNetCard);
				nOffset += nRet;
				break;
			case EMTT_PROGRAM:			//����
				pMoniInfoAllHostProc = new CMoniInfoAllHostProc;
				nRet = pMoniInfoAllHostProc->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoAllHostProc);
				nOffset += nRet;
				break;

			default:
				break;
			}
		}
		return nOffset;
	}
	
	//==============================================================================
	//
	//                CHostCHGItoAPkt
	//                �仯����״̬��Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	CHostCHGItoAPkt::CHostCHGItoAPkt()
	{
		CProtocolHead::SetType(TYPE_MAINTEN);
		CProtocolHead::SetFunc(FUNC_HOSTCHG_INTERTOAPP);
	}
	
	CHostCHGItoAPkt::~CHostCHGItoAPkt()
	{
	
	}

	//==============================================================================
	//
	//                CReqGrpAndHostAllCtoAPkt
	//                ����ȫ���鼰����״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	CReqGrpAndHostAllCtoAPkt::CReqGrpAndHostAllCtoAPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_GROUPANDHOSTALL_REQ_CTRLTOAPP)
	{
	}
	
	CReqGrpAndHostAllCtoAPkt::~CReqGrpAndHostAllCtoAPkt()
	{
	
	}

	//==============================================================================
	//
	//                CHostAllAtoC
	//                ����״̬
	//
	//==============================================================================
	CHostAllAtoC::CHostAllAtoC()
		:m_byHostState(HOST_GROUP_STATE_UNKNOW)
	{

	}

	CHostAllAtoC::~CHostAllAtoC()
	{

	}

	void CHostAllAtoC::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CHostAllAtoC::GetHostName() const
	{
		return m_strHostName;
	}

	void CHostAllAtoC::SetHostState(BYTE byHostState)
	{
		m_byHostState = byHostState;
	}

	BYTE CHostAllAtoC::GetHostState() const
	{
		return m_byHostState;
	}

	int CHostAllAtoC::StreamSize()
	{
		int nSize = 0;

		// ����������
		nSize += sizeof(BYTE);

		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;

		// ����״̬
		nSize += sizeof(BYTE);

		return nSize;
	}

	int CHostAllAtoC::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostAllAtoC len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// ����״̬
		pBuf[nOffset] = m_byHostState;
		nOffset += 1;

		return nOffset;
	}

	int CHostAllAtoC::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostAllAtoC HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostAllAtoC HostName len\n");
			return -1;
		}
		char strHostName[0xff+1];
		memcpy(strHostName, pBuf + nOffset, byLen);
		strHostName[byLen] = 0;
		m_strHostName = strHostName;
		nOffset += byLen;

		// ����״̬
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostAllAtoC HostState len\n");
			return -1;
		}
		m_byHostState = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//
	//                CGroupAndHostsAtoC
	//                �����鼰��������״̬
	//
	//==============================================================================
	CGroupAndHostsAtoC::CGroupAndHostsAtoC()
		:m_byGrpState(HOST_GROUP_STATE_UNKNOW)
	{

	}
	
	CGroupAndHostsAtoC::~CGroupAndHostsAtoC()
	{
	
	}
	
	void CGroupAndHostsAtoC::SetGroupName(string strGroupName)
	{
		m_strGroupName = strGroupName;
	}
	
	string CGroupAndHostsAtoC::GetGroupName() const
	{
		return m_strGroupName;
	}
	
	void CGroupAndHostsAtoC::SetGrpState(BYTE byGrpState)
	{
		m_byGrpState = byGrpState;
	}
	
	BYTE CGroupAndHostsAtoC::GetGrpState() const
	{
		return m_byGrpState;
	}
	
	int CGroupAndHostsAtoC::GetHostAllAtoCCount() const
	{
		return (int)m_vecHostAllAtoC.size();
	}

	CHostAllAtoC* CGroupAndHostsAtoC::GetHostAllAtoC(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecHostAllAtoC.size());
		return &m_vecHostAllAtoC[nIndex];
	}

	void CGroupAndHostsAtoC::Add(const CHostAllAtoC& objHostAllAtoC)
	{
		if (m_vecHostAllAtoC.size() < 0xff)
		{
			m_vecHostAllAtoC.push_back(objHostAllAtoC);
		}
	}
	void CGroupAndHostsAtoC::Clear()
	{
		m_vecHostAllAtoC.clear();
	}

	int CGroupAndHostsAtoC::StreamSize()
	{
		int nSize = 0;
		
		// ������������
		nSize += sizeof(BYTE);
		
		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_strGroupName.size());
		nSize += byLen;
		
		// ��״̬
		nSize += sizeof(BYTE);

		// ��������
		nSize += sizeof(BYTE);

		// ������Ϣ
		byLen = (BYTE)MIN(0xff, m_vecHostAllAtoC.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecHostAllAtoC.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CGroupAndHostsAtoC::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CGroupAtoC len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		int nOffset = 0;
		
		// ������������
		BYTE byLen = (BYTE)MIN(0xff, m_strGroupName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������
		memcpy(pBuf + nOffset, m_strGroupName.c_str(), byLen);
		nOffset += byLen;
		
		// ��״̬
		pBuf[nOffset] = m_byGrpState;
		nOffset += 1;

		// ������Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecHostAllAtoC.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecHostAllAtoC.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}
	
	int CGroupAndHostsAtoC::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// ������������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAtoC GroupName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAtoC GroupName len\n");
			return -1;
		}
		char strGroupName[0xff+1];
		memcpy(strGroupName, pBuf + nOffset, byLen);
		strGroupName[byLen] = 0;
		m_strGroupName = strGroupName;
		nOffset += byLen;
		
		// ��״̬
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAtoC GrpState len\n");
			return -1;
		}
		m_byGrpState = pBuf[nOffset];
		nOffset += 1;

		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGrpAndHostAllAtoCPkt GroupAtoC_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CHostAllAtoC objHostAtoC;
			int nRet = objHostAtoC.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}

			m_vecHostAllAtoC.push_back(objHostAtoC);
			nOffset += nRet;
		}

		return nOffset;
	}

	int CGroupAndHostsAtoC::GetHostIndexByName(string strName)
	{
		for (int i = 0; i < m_vecHostAllAtoC.size(); i++)
		{
			if (m_vecHostAllAtoC.at(i).GetHostName() == strName)
			{
				return i;
			}
		}

		return -1;
	}

	//==============================================================================
	//
	//                CGrpAndHostAllAtoCPkt
	//                ȫ����״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	CGrpAndHostAllAtoCPkt::CGrpAndHostAllAtoCPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_GROUPHOSTALL_APPTOCTRL)
	{
		m_vecGroupAndHostsAtoC.clear();
	}
	
	CGrpAndHostAllAtoCPkt::~CGrpAndHostAllAtoCPkt()
	{
	
	}
	
	int CGrpAndHostAllAtoCPkt::GetGroupAndHostsAtoCCount() const
	{
		return (int)m_vecGroupAndHostsAtoC.size();
	}
	
	CGroupAndHostsAtoC* CGrpAndHostAllAtoCPkt::GetGroupAndHostsAtoC(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecGroupAndHostsAtoC.size());
		return &m_vecGroupAndHostsAtoC[nIndex];
	}
	
	void CGrpAndHostAllAtoCPkt::Add(const CGroupAndHostsAtoC& objGroupAndHostsAtoC)
	{
		if (m_vecGroupAndHostsAtoC.size() < 0xff)
		{
			m_vecGroupAndHostsAtoC.push_back(objGroupAndHostsAtoC);
		}
	}
	void CGrpAndHostAllAtoCPkt::Clear()
	{
		m_vecGroupAndHostsAtoC.clear();
	}
	
	CProtocolHead* CGrpAndHostAllAtoCPkt::Clone()
	{
		CGrpAndHostAllAtoCPkt* pClone = new CGrpAndHostAllAtoCPkt();
		*pClone = *this;
		return pClone;
	}
	
	int CGrpAndHostAllAtoCPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ��������Ϣ����
		nSize += sizeof(BYTE);
		
		// ��������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecGroupAndHostsAtoC.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecGroupAndHostsAtoC.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CGrpAndHostAllAtoCPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CGrpAndHostAllAtoCPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);
		
		// ��������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecGroupAndHostsAtoC.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecGroupAndHostsAtoC.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CGrpAndHostAllAtoCPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ��������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGrpAndHostAllAtoCPkt GroupAtoC_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CGroupAndHostsAtoC GroupAndHostsAtoC;
			nRet = GroupAndHostsAtoC.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			
			m_vecGroupAndHostsAtoC.push_back(GroupAndHostsAtoC);
			nOffset += nRet;
		}
		
		return nOffset;
	}

	int CGrpAndHostAllAtoCPkt::GetHostGrpIndexByName(string strName)
	{
		for (int i = 0; i < m_vecGroupAndHostsAtoC.size(); i++)
		{
			if (m_vecGroupAndHostsAtoC.at(i).GetGroupName() == strName)
			{
				return i;
			}
		}

		return -1;
	}

	//==============================================================================
	//
	//             CGroupAtoC
	//             ������״̬
	//
	//==============================================================================
	CGroupAtoC::CGroupAtoC()
	{

	}

	CGroupAtoC::~CGroupAtoC()
	{

	}

	void CGroupAtoC::SetGroupName(string strGroupName)
	{
		m_strGroupName = strGroupName;
	}

	string CGroupAtoC::GetGroupName() const
	{
		return m_strGroupName;
	}

	void CGroupAtoC::SetGrpState(BYTE byGrpState)
	{
		m_byGrpState = byGrpState;
	}

	BYTE CGroupAtoC::GetGrpState() const
	{
		return m_byGrpState;
	}

	int CGroupAtoC::StreamSize()
	{
		int nSize = 0;

		// ������������
		nSize += sizeof(BYTE);

		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_strGroupName.size());
		nSize += byLen;

		// ��״̬
		nSize += sizeof(BYTE);

		return nSize;
	}

	int CGroupAtoC::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CGroupAtoC len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		// ������������
		BYTE byLen = (BYTE)MIN(0xff, m_strGroupName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ��������
		memcpy(pBuf + nOffset, m_strGroupName.c_str(), byLen);
		nOffset += byLen;

		// ��״̬
		pBuf[nOffset] = m_byGrpState;
		nOffset += 1;

		return nOffset;
	}

	int CGroupAtoC::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// ������������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAtoC GroupName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAtoC GroupName len\n");
			return -1;
		}
		char strGroupName[0xff+1];
		memcpy(strGroupName, pBuf + nOffset, byLen);
		strGroupName[byLen] = 0;
		m_strGroupName = strGroupName;
		nOffset += byLen;

		// ��״̬
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAtoC GrpState len\n");
			return -1;
		}
		m_byGrpState = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	//==============================================================================
	//
	//                CGrpCHGAtoCPkt
	//                �仯��״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	CGrpCHGAtoCPkt::CGrpCHGAtoCPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_GROUPCHG_APPTOCTRL)
	{
		
	}
	
	CGrpCHGAtoCPkt::~CGrpCHGAtoCPkt()
	{
	
	}

	int CGrpCHGAtoCPkt::GetGroupAtoCCount() const
	{
		return (int)m_vecGroupAtoC.size();
	}

	CGroupAtoC* CGrpCHGAtoCPkt::GetGroupAtoC(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecGroupAtoC.size());
		return &m_vecGroupAtoC[nIndex];
	}

	void CGrpCHGAtoCPkt::Add(const CGroupAtoC& objGroupAtoC)
	{
		if (m_vecGroupAtoC.size() < 0xff)
		{
			m_vecGroupAtoC.push_back(objGroupAtoC);
		}
	}
	void CGrpCHGAtoCPkt::Clear()
	{
		m_vecGroupAtoC.clear();
	}

	CProtocolHead* CGrpCHGAtoCPkt::Clone()
	{
		CGrpCHGAtoCPkt* pClone = new CGrpCHGAtoCPkt();
		*pClone = *this;
		return pClone;
	}

	int CGrpCHGAtoCPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();

		// ��������Ϣ����
		nSize += sizeof(BYTE);

		// ��������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecGroupAtoC.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecGroupAtoC.at(i).StreamSize();
		}

		return nSize;
	}

	int CGrpCHGAtoCPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CGrpAndHostAllAtoCPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// ��������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecGroupAtoC.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ��������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecGroupAtoC.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CGrpCHGAtoCPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// ��������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGrpAndHostAllAtoCPkt GroupAtoC_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CGroupAtoC objGroupAtoC;
			nRet = objGroupAtoC.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}

			m_vecGroupAtoC.push_back(objGroupAtoC);
			nOffset += nRet;
		}

		return nOffset;
	}
	
	//==============================================================================
	//
	//                CGroupAllAtoC
	//                ��������Ϣ
	//
	//==============================================================================
	CGroupAllAtoC::CGroupAllAtoC()
	{
		m_vecHostAllAtoC.clear();
	}
	
	CGroupAllAtoC::~CGroupAllAtoC()
	{
	
	}
	
	void CGroupAllAtoC::SetGroupName(string strGroupName)
	{
		m_strGroupName = strGroupName;
	}
	
	string CGroupAllAtoC::GetGroupName() const
	{
		return m_strGroupName;
	}
	
	int CGroupAllAtoC::GetHostAllAtoCCount() const
	{
		return (int)m_vecHostAllAtoC.size();
	}
	
	CHostAllAtoC* CGroupAllAtoC::GetHostAllAtoC(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecHostAllAtoC.size());
		return &m_vecHostAllAtoC[nIndex];
	}
	
	void CGroupAllAtoC::Add(const CHostAllAtoC& objHostAllAtoC)
	{
		if (m_vecHostAllAtoC.size() < 0xff)
		{
			m_vecHostAllAtoC.push_back(objHostAllAtoC);
		}
	}

	void CGroupAllAtoC::Clear()
	{
		m_vecHostAllAtoC.clear();
	}

	int CGroupAllAtoC::StreamSize()
	{
		int nSize = 0;
		
		// ������������
		nSize += sizeof(BYTE);
		
		// ��������
		BYTE byLen = (BYTE)MIN(0xff, m_strGroupName.size());
		nSize += byLen;
		
		// ������Ϣ����
		nSize += sizeof(BYTE);
		
		// ������Ϣ
		byLen = (BYTE)MIN(0xff, m_vecHostAllAtoC.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecHostAllAtoC.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CGroupAllAtoC::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CGroupAllAtoC len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		int nOffset = 0;
		
		// ������������
		BYTE byLen = (BYTE)MIN(0xff, m_strGroupName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������
		memcpy(pBuf + nOffset, m_strGroupName.c_str(), byLen);
		nOffset += byLen;
		
		// ������Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecHostAllAtoC.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecHostAllAtoC.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CGroupAllAtoC::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// ������������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAllAtoC GroupName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAllAtoC GroupName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strGroupName = str0xff;
		nOffset += byLen;
		
		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CGroupAllAtoC HostAllAtoC_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CHostAllAtoC objHostAllAtoC;
			nRet = objHostAllAtoC.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			
			m_vecHostAllAtoC.push_back(objHostAllAtoC);
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//                CHostInGrpCHGAtoCPkt
	//                �仯��������״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	CHostInGrpCHGAtoCPkt::CHostInGrpCHGAtoCPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_HOSTINGROUPCHG_APPTOCTRL)
	{
		m_vecGroupAllAtoC.clear();
	}
	
	CHostInGrpCHGAtoCPkt::~CHostInGrpCHGAtoCPkt()
	{
	
	}
	
	int CHostInGrpCHGAtoCPkt::GetGroupAllAtoCCount() const
	{
		return (int)m_vecGroupAllAtoC.size();
	}
	
	CGroupAllAtoC* CHostInGrpCHGAtoCPkt::GetGroupAllAtoC(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecGroupAllAtoC.size());
		return &m_vecGroupAllAtoC[nIndex];
	}
	
	void CHostInGrpCHGAtoCPkt::Add(const CGroupAllAtoC& objGroupAllAtoC)
	{
		if (m_vecGroupAllAtoC.size() < 0xff)
		{
			m_vecGroupAllAtoC.push_back(objGroupAllAtoC);
		}
	}

	void CHostInGrpCHGAtoCPkt::Clear()
	{
		m_vecGroupAllAtoC.clear();
	}

	CProtocolHead* CHostInGrpCHGAtoCPkt::Clone()
	{
		CHostInGrpCHGAtoCPkt* pClone = new CHostInGrpCHGAtoCPkt();
		*pClone = *this;
		return pClone;
	}
	
	int CHostInGrpCHGAtoCPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
				
		// ��������Ϣ����
		nSize += sizeof(BYTE);
		
		// ��������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecGroupAllAtoC.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecGroupAllAtoC.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CHostInGrpCHGAtoCPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostInGrpAllAtoCPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);
		
		// ��������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecGroupAllAtoC.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ��������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecGroupAllAtoC.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CHostInGrpCHGAtoCPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ��������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostInGrpAllAtoCPkt GroupAllAtoC_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CGroupAllAtoC objGroupAllAtoC;
			nRet = objGroupAllAtoC.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			
			m_vecGroupAllAtoC.push_back(objGroupAllAtoC);
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//                CSglHost
	//                ������Ϣ
	//
	//==============================================================================
	CSglHost::CSglHost()
	{

	}
	
	CSglHost::~CSglHost()
	{
	
	}
	
	void CSglHost::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}
	
	string CSglHost::GetHostName() const
	{
		return m_strHostName;
	}
	
	int CSglHost::StreamSize()
	{
		int nSize = 0;
		
		// ����������
		nSize += sizeof(BYTE);
		
		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;
		
		return nSize;
	}
	
	int CSglHost::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSglHost len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		int nOffset = 0;
		
		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;
		
		return nOffset;
	}
	
	int CSglHost::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglHost HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglHost HostName len\n");
			return -1;
		}
		char strHostName[0xff+1];
		memcpy(strHostName, pBuf + nOffset, byLen);
		strHostName[byLen] = 0;
		m_strHostName = strHostName;
		nOffset += byLen;
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//                CReqSglHostAllCtoAPkt
	//                ����ȫ�嵥������״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	CReqSglHostAllCtoAPkt::CReqSglHostAllCtoAPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_SIGLEHOSTANDSERVICEALL_REQ_CTRLTOAPP)
	{
		m_vecSglHost.clear();
	}
	
	CReqSglHostAllCtoAPkt::~CReqSglHostAllCtoAPkt()
	{
	
	}
	
	int CReqSglHostAllCtoAPkt::GetSglHostCount() const
	{
		return (int)m_vecSglHost.size();
	}
	
	CSglHost* CReqSglHostAllCtoAPkt::GetSglHost(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecSglHost.size());
		return &m_vecSglHost[nIndex];
	}
	
	void CReqSglHostAllCtoAPkt::Add(const CSglHost& objSglHost)
	{
		if (m_vecSglHost.size() < 0xff)
		{
			m_vecSglHost.push_back(objSglHost);
		}
	}
	void CReqSglHostAllCtoAPkt::Clear()
	{
		m_vecSglHost.clear();
	}
	
	CProtocolHead* CReqSglHostAllCtoAPkt::Clone()
	{
		CReqSglHostAllCtoAPkt* pClone = new CReqSglHostAllCtoAPkt();
		*pClone = *this;
		return pClone;
	}
	
	int CReqSglHostAllCtoAPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ������Ϣ����
		nSize += sizeof(BYTE);
		
		// ������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecSglHost.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecSglHost.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CReqSglHostAllCtoAPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqSglHostAllCtoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);
		
		// ������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecSglHost.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecSglHost.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CReqSglHostAllCtoAPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqSglHostAllCtoAPkt SglHost_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CSglHost objSglHost;
			nRet = objSglHost.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			
			m_vecSglHost.push_back(objSglHost);
			nOffset += nRet;
		}
		
		return nOffset;
	}

	//==============================================================================
	//
	//                CSglCpuAndMem
	//                ������������������̵�CPU���ڴ�ʵʱ״̬��Ϣ
	//
	//==============================================================================
	CSglCpuAndMem::CSglCpuAndMem()
		: m_byValidity(VALIDITY_YES)
	{
	
	}

	CSglCpuAndMem::CSglCpuAndMem(const CSglCpuAndMem& objSglCpuAndMem)
	{
		*this = objSglCpuAndMem;
	}

	CSglCpuAndMem::~CSglCpuAndMem()
	{
		Clear();
	}

	CSglCpuAndMem& CSglCpuAndMem::operator=(const CSglCpuAndMem& objSglCpuAndMem)
	{
		m_strHostName = objSglCpuAndMem.m_strHostName;
		m_byValidity = objSglCpuAndMem.m_byValidity;

		const CMoniInfoBase* pMoniInfoBase;
		int i;
		for (i = 0; i < objSglCpuAndMem.GetMoniInfoBaseCount(); i++)
		{
			pMoniInfoBase = objSglCpuAndMem.GetMoniInfoBase(i);
			Add(pMoniInfoBase);
		}

		return *this;
	}

	void CSglCpuAndMem::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CSglCpuAndMem::GetHostName() const
	{
		return m_strHostName;
	}

	void CSglCpuAndMem::SetValidity(BYTE byValidity)
	{
		m_byValidity = byValidity;
	}

	BYTE CSglCpuAndMem::GetValidity() const
	{
		return m_byValidity;
	}

	int CSglCpuAndMem::GetMoniInfoBaseCount() const
	{
		return (int)m_vecMoniInfo.size();
	}

	CMoniInfoBase* CSglCpuAndMem::GetMoniInfoBase(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_vecMoniInfo.size())
		{
			return m_vecMoniInfo[nIndex];
		}

		return NULL;
	}

	void CSglCpuAndMem::Add(const CMoniInfoBase* pMoniInfoBase)
	{
		assert(pMoniInfoBase);

		CMoniInfoCpuLoad* pMoniInfoCpuLoad;
		CMoniInfoMemLoad* pMoniInfoMemLoad;
		CMoniInfoProcess* pMoniInfoProcess;

		switch (pMoniInfoBase->GetMoniType())
		{
		case EMTT_CPU:			//CPU����
			pMoniInfoCpuLoad = (CMoniInfoCpuLoad*)pMoniInfoBase;
			AddCPULoad(pMoniInfoCpuLoad->GetCPULoadPer());
			break;

		case EMTT_MEM:			//�ڴ�ռ��
			pMoniInfoMemLoad = (CMoniInfoMemLoad*)pMoniInfoBase;
			AddMEMLoad(pMoniInfoMemLoad->GetMemLoadPer());
			break;

		case EMTT_PROGRAM:			//����
			pMoniInfoProcess = (CMoniInfoProcess*)pMoniInfoBase;
			AddProcess(*pMoniInfoProcess);
			break;

		default:
			break;
		}
	}

	void CSglCpuAndMem::AddCPULoad(const float fCPULoad)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoCpuLoad* pInfo = new CMoniInfoCpuLoad;
		pInfo->SetCPULoadPer(fCPULoad);
		m_vecMoniInfo.push_back(pInfo);
	}

	void CSglCpuAndMem::AddMEMLoad(const float fMEMLoad)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoMemLoad* pInfo = new CMoniInfoMemLoad;
		pInfo->SetMemLoadPer(fMEMLoad);
		m_vecMoniInfo.push_back(pInfo);
	}

	void CSglCpuAndMem::AddProcess(const CMoniInfoProcess& objProcess)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoProcess* pInfo = new CMoniInfoProcess;
		*pInfo  = objProcess;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CSglCpuAndMem::Clear()
	{
		CMoniInfoBase* pItem;
		vector<CMoniInfoBase*>::iterator it;
		for (it = m_vecMoniInfo.begin(); it != m_vecMoniInfo.end(); it++)
		{
			pItem = *it;
			assert(pItem);
			delete pItem;
			pItem = NULL;
		}
		m_vecMoniInfo.clear();
	}

	int CSglCpuAndMem::StreamSize()
	{
		int nSize = 0;

		// ����������
		nSize += sizeof(BYTE);

		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;

		// ����״̬
		nSize += 1;

		// �����Ϣ����
		nSize += sizeof(BYTE);

		// �����Ϣ
		int nMoniInfoNum = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		for(int i = 0; i < nMoniInfoNum; i++)
		{
			//�����Ϣ����
			nSize += sizeof(BYTE);

			//��Ϣ
			nSize += m_vecMoniInfo.at(i)->StreamSize();
		}

		return nSize;
	}

	int CSglCpuAndMem::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CCpuAndMemItoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// ��Ϣ��Ч��
		pBuf[nOffset] = m_byValidity;
		nOffset += 1;

		//�����Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// �����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			WORD wMoniLen = (WORD)MIN(MAX_WORDSIZE, m_vecMoniInfo.at(i)->StreamSize());
			SETSTREAMSHORT(pBuf + nOffset, wMoniLen);
			nOffset += 2;

			//�����Ϣ
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfo.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSglCpuAndMem::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;

		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt HostName len\n");
			return -1;
		}
		char strHostName[0xff+1];
		memcpy(strHostName, pBuf + nOffset, byLen);
		strHostName[byLen] = 0;
		m_strHostName = strHostName;
		nOffset += byLen;

		// ��Ϣ��Ч��
		m_byValidity = pBuf[nOffset];
		nOffset += 1;

		// �����Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the number len of CCpuAndMemItoAPkt CMoniInfo\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		CMoniInfoCpuLoad* pMoniInfoCpuLoad;
		CMoniInfoMemLoad* pMoniInfoMemLoad;
		CMoniInfoProcess* pMoniInfoProcess;
		//�����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			if(nBufLen < (nOffset + sizeof(WORD)))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt CMoniInfo len\n");
				return -1;
			}
			WORD wMoniLen = GETSTREAMSHORT(pBuf + nOffset);
			nOffset += 2;

			//�����Ϣ����
			if(nBufLen < (nOffset + wMoniLen))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemItoAPkt CMoniInfo MoniType len\n");
				return -1;
			}
			BYTE byMoniType = pBuf[nOffset];
			//�˴�ƫ���������ӣ���Ϊ����������ڼ����Ϣ����

			switch (byMoniType)
			{
			case EMTT_CPU:			//CPU
				pMoniInfoCpuLoad = new CMoniInfoCpuLoad;
				nRet = pMoniInfoCpuLoad->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoCpuLoad);
				nOffset += nRet;
				break;

			case EMTT_MEM:			//�ڴ�
				pMoniInfoMemLoad = new CMoniInfoMemLoad;
				nRet = pMoniInfoMemLoad->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoMemLoad);
				nOffset += nRet;
				break;

			case EMTT_PROGRAM:			//����
				pMoniInfoProcess = new CMoniInfoProcess;
				nRet = pMoniInfoProcess->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoProcess);
				nOffset += nRet;
				break;

			default:
				break;
			}
		}

		return nOffset;
	}
	
	//==============================================================================
	//
	//                CCpuAndMemAtoCPkt
	//                ����������̵�CPU���ڴ�ʵʱ״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	CCpuAndMemAtoCPkt::CCpuAndMemAtoCPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_CPUANDMEMREALTIME_APPTOCTRL)
	{
		m_vecHostAtoC.clear();
	}
	
	CCpuAndMemAtoCPkt::~CCpuAndMemAtoCPkt()
	{
		
	}
	
	int CCpuAndMemAtoCPkt::GetHostAtoCCount() const
	{
		return (int)m_vecHostAtoC.size();
	}
	
	CSglCpuAndMem* CCpuAndMemAtoCPkt::GetHostAtoC(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecHostAtoC.size());
		return &m_vecHostAtoC[nIndex];
	}
	
	void CCpuAndMemAtoCPkt::Add(const CSglCpuAndMem& objCpuAndMemItoAPkt)
	{
		if (m_vecHostAtoC.size() < 0xff)
		{
			m_vecHostAtoC.push_back(objCpuAndMemItoAPkt);
		}
	}
	void CCpuAndMemAtoCPkt::Clear()
	{
		m_vecHostAtoC.clear();
	}
	
	CProtocolHead* CCpuAndMemAtoCPkt::Clone()
	{
		CCpuAndMemAtoCPkt* pClone = new CCpuAndMemAtoCPkt();
		*pClone = *this;
		return pClone;
	}
	
	int CCpuAndMemAtoCPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ������Ϣ����
		nSize += sizeof(BYTE);
		
		// ������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecHostAtoC.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecHostAtoC.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CCpuAndMemAtoCPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CCpuAndMemAtoCPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);		
		
		// ������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecHostAtoC.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecHostAtoC.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CCpuAndMemAtoCPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CCpuAndMemAtoCPkt HostAtoC_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CSglCpuAndMem objHostAtoC;
			nRet = objHostAtoC.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			
			m_vecHostAtoC.push_back(objHostAtoC);
			nOffset += nRet;
		}
		
		return nOffset;
	}
	
	//==============================================================================
	//
	//                        CSglHostState
	//       ����״̬��Ϣ������״̬��Ϣ����������ϵͳ��CPU���ڴ桢���̡����硢������Ϣ��
	//
	//==============================================================================
	CSglHostState::CSglHostState()
		: m_byValidity(VALIDITY_YES)
	{

	}
	
	CSglHostState::CSglHostState(const CSglHostState& objSglHostState)
	{
		*this = objSglHostState;
	}

	CSglHostState::~CSglHostState()
	{
		Clear();
	}
	
	CSglHostState& CSglHostState::operator=(const CSglHostState& objSglHostAndService)
	{
		m_strHostName = objSglHostAndService.m_strHostName;
		m_strHostDesc = objSglHostAndService.m_strHostDesc;
		m_byValidity = objSglHostAndService.m_byValidity;

		const CMoniInfoBase* pMoniInfoBase;
		int i;
		for (i = 0; i < objSglHostAndService.GetMoniInfoBaseCount(); i++)
		{
			pMoniInfoBase = objSglHostAndService.GetMoniInfoBase(i);
			Add(pMoniInfoBase);
		}

		return *this;
	}

	void CSglHostState::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}
	
	string CSglHostState::GetHostName() const
	{
		return m_strHostName;
	}
	
	void CSglHostState::SetHostDesc(string strHostDesc)
	{
		m_strHostDesc = strHostDesc;
	}

	std::string CSglHostState::GetHostDesc() const
	{
		return m_strHostDesc;
	}

	void CSglHostState::SetValidity(BYTE byValidity)
	{
		m_byValidity = byValidity;
	}

	BYTE CSglHostState::GetValidity() const
	{
		return m_byValidity;
	}

	int CSglHostState::GetMoniInfoBaseCount() const
	{
		return (int)m_vecMoniInfo.size();
	}

	CMoniInfoBase* CSglHostState::GetMoniInfoBase(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_vecMoniInfo.size());
		return m_vecMoniInfo[nIndex];
	}

	void CSglHostState::Add(const CMoniInfoBase* pMoniInfoBase)
	{
		assert(pMoniInfoBase);

		CMoniInfoOS* pMoniInfoOS = NULL;
		CMoniInfoCpuNum* pMoniInfoCpuNum = NULL;
		CMoniInfoMemCap* pMoniInfoMemCap = NULL;
		CMoniInfoDisk* pMoniInfoDisk = NULL;
		CMoniInfoNetCard* pMoniInfoNetCard = NULL;
		CMoniInfoAllHostProc* pMoniInfoAllHostProc = NULL;
		switch (pMoniInfoBase->GetMoniType())
		{
		case EMTT_OS:			// ����ϵͳ
			pMoniInfoOS = (CMoniInfoOS*)pMoniInfoBase;
			AddOS(pMoniInfoOS->GetOSName());
			break;

		case EMTT_CPU:			// CPU����
			pMoniInfoCpuNum = (CMoniInfoCpuNum*)pMoniInfoBase;
			AddCPUNum(pMoniInfoCpuNum->GetCPUNum());
			break;

		case EMTT_MEM:			// �ڴ�����
			pMoniInfoMemCap = (CMoniInfoMemCap*)pMoniInfoBase;
			AddMEMCap(pMoniInfoMemCap->GetMemCap());
			break;

		case EMTT_DISK:			// ����
			pMoniInfoDisk = (CMoniInfoDisk*)pMoniInfoBase;
			AddDisk(pMoniInfoDisk);
			break;

		case EMTT_NETWORK:		// ����
			pMoniInfoNetCard = (CMoniInfoNetCard*)pMoniInfoBase;
			AddNetCard(pMoniInfoNetCard);
			break;

		case EMTT_PROGRAM:			// ����
			pMoniInfoAllHostProc = (CMoniInfoAllHostProc*)pMoniInfoBase;
			AddProcess(pMoniInfoAllHostProc);
			break;

		default:
			break;
		}
	}

	//os
	void CSglHostState::AddOS(const string sOSName)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoOS* pInfo = new CMoniInfoOS;
		pInfo->SetOSName(sOSName);
		m_vecMoniInfo.push_back(pInfo);
	}

	//cpu
	void CSglHostState::AddCPUNum(const BYTE byCPUNum)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoCpuNum* pInfo = new CMoniInfoCpuNum;
		pInfo->SetCPUNum(byCPUNum);
		m_vecMoniInfo.push_back(pInfo);
	}

	//mem
	void CSglHostState::AddMEMCap(const float fMEMCap)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoMemCap* pInfo = new CMoniInfoMemCap;
		pInfo->SetMemCap(fMEMCap);
		m_vecMoniInfo.push_back(pInfo);
	}

	//disk
	void CSglHostState::AddDisk(const CMoniInfoDisk* pMoniInfoDisk)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoDisk* pInfo = new CMoniInfoDisk;
		*pInfo  = *pMoniInfoDisk;
		m_vecMoniInfo.push_back(pInfo);
	}

	//netcard
	void CSglHostState::AddNetCard(const CMoniInfoNetCard* pMoniInfoNetCard)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoNetCard* pInfo = new CMoniInfoNetCard;
		*pInfo  = *pMoniInfoNetCard;
		m_vecMoniInfo.push_back(pInfo);
	}

	//process
	void CSglHostState::AddProcess(const CMoniInfoAllHostProc* pMoniInfoAllHostProc)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoAllHostProc* pInfo = new CMoniInfoAllHostProc;
		*pInfo  = *pMoniInfoAllHostProc;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CSglHostState::Clear()
	{
		vector<CMoniInfoBase*>::iterator it;
		for (it = m_vecMoniInfo.begin(); it != m_vecMoniInfo.end(); it++)
		{
			CMoniInfoBase* pItem = *it;
			assert(pItem);
			delete pItem;
			pItem = NULL;
		}

		m_vecMoniInfo.clear();
	}

	int CSglHostState::StreamSize()
	{
		// CProtocolHead
		int nSize = 0;

		// ����������
		nSize += sizeof(BYTE);

		// ������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;

		// ������������
		nSize += sizeof(BYTE);

		// ��������
		byLen = (BYTE)MIN(0xff, m_strHostDesc.size());
		nSize += byLen;

		// ����״̬
		nSize += 1;

		// �����Ϣ����
		nSize += sizeof(BYTE);

		// �����Ϣ
		int nMoniInfoNum = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		for(int i = 0; i < nMoniInfoNum; i++)
		{
			//�����Ϣ����
			nSize += sizeof(WORD);

			//��Ϣ
			nSize += m_vecMoniInfo.at(i)->StreamSize();
		}	

		return nSize;
	}

	int CSglHostState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSglHostState len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = 0;

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// ������������
		byLen = (BYTE)MIN(0xff, m_strHostDesc.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ��������
		memcpy(pBuf + nOffset, m_strHostDesc.c_str(), byLen);
		nOffset += byLen;

		// ��Ϣ��Ч��
		pBuf[nOffset] = m_byValidity;
		nOffset += 1;

		//�����Ϣ����
		byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// �����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			WORD wMoniLen = m_vecMoniInfo.at(i)->StreamSize();
			SETSTREAMSHORT(pBuf + nOffset, wMoniLen);
			nOffset += 2;

			//�����Ϣ
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfo.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSglHostState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;

		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglHostState HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglHostState HostName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostName = str0xff;
		nOffset += byLen;

		// ������������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglHostState HostDesc_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglHostState HostDesc len\n");
			return -1;
		}
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostDesc = str0xff;
		nOffset += byLen;

		// ��Ϣ��Ч��
		m_byValidity = pBuf[nOffset];
		nOffset += 1;

		// �����Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the number len of CSglHostState CMoniInfo\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		CMoniInfoOS*	  pMoniInfoOS;	
		CMoniInfoCpuNum* pMoniInfoCpuNum;
		CMoniInfoMemCap* pMoniInfoMemCap;
		CMoniInfoDisk*    pMoniInfoDisk;
		CMoniInfoNetCard* pMoniInfoNetCard;
		CMoniInfoAllHostProc* pMoniInfoAllHostProc;
		//�����Ϣ
		for(int i = 0; i < byLen; i++)
		{
			//�����Ϣ����
			if(nBufLen < (nOffset + sizeof(WORD)))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CSglHostState CMoniInfo len\n");
				return -1;
			}
			WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
			nOffset += 2;

			//�����Ϣ����
			if(nBufLen < (nOffset + wLen))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CSglHostState CMoniInfo MoniType len\n");
				return -1;
			}
			BYTE byMoniType = pBuf[nOffset];
			//�˴�ƫ���������ӣ���Ϊ����������ڼ����Ϣ����

			switch (byMoniType)
			{
			case EMTT_OS:		//OS
				pMoniInfoOS = new CMoniInfoOS;
				nRet = pMoniInfoOS->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoOS);
				nOffset += nRet;
				break;
			case EMTT_CPU:			//CPU
				pMoniInfoCpuNum = new CMoniInfoCpuNum;
				nRet = pMoniInfoCpuNum->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoCpuNum);
				nOffset += nRet;
				break;

			case EMTT_MEM:			//�ڴ�
				pMoniInfoMemCap = new CMoniInfoMemCap;
				nRet = pMoniInfoMemCap->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoMemCap);
				nOffset += nRet;
				break;

			case EMTT_DISK:		//����
				pMoniInfoDisk = new CMoniInfoDisk;
				nRet = pMoniInfoDisk->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoDisk);
				nOffset += nRet;
				break;
			case EMTT_NETWORK:		//����
				pMoniInfoNetCard = new CMoniInfoNetCard;
				nRet = pMoniInfoNetCard->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoNetCard);
				nOffset += nRet;
				break;
			case EMTT_PROGRAM:			//����
				pMoniInfoAllHostProc = new CMoniInfoAllHostProc;
				nRet = pMoniInfoAllHostProc->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
				if(nRet == -1)
				{
					return -1;
				}
				m_vecMoniInfo.push_back(pMoniInfoAllHostProc);
				nOffset += nRet;
				break;

			default:
				break;
			}
		}

		return nOffset;
	}

	//==============================================================================
	//
	//                CSglHostAllCtoAPkt
	//                ȫ�嵥������״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	CSglHostAllCtoAPkt::CSglHostAllCtoAPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_SIGLEHOSTANDSERVICEALL_APPTOCTRL)
	{
		m_vecSglHostState.clear();
	}
	
	CSglHostAllCtoAPkt::~CSglHostAllCtoAPkt()
	{
	
	}
	
	int CSglHostAllCtoAPkt::GetSglHostStateCount() const
	{
		return (int)m_vecSglHostState.size();
	}
	
	CSglHostState* CSglHostAllCtoAPkt::GetSglHostState(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecSglHostState.size());
		return &m_vecSglHostState[nIndex];
	}
	
	void CSglHostAllCtoAPkt::Add(const CSglHostState& objSglHostState)
	{
		if (m_vecSglHostState.size() < 0xff)
		{
			m_vecSglHostState.push_back(objSglHostState);
		}
	}

	void CSglHostAllCtoAPkt::Clear()
	{
		m_vecSglHostState.clear();
	}

	CProtocolHead* CSglHostAllCtoAPkt::Clone()
	{
		CSglHostAllCtoAPkt* pClone = new CSglHostAllCtoAPkt();

		for (int i = 0; i < m_vecSglHostState.size(); i++)
		{
			CSglHostState* pSglHostState = GetSglHostState(i);
			CSglHostState objCloneSglHostState = *pSglHostState;
			pClone->Add(objCloneSglHostState);
		}

		return pClone;
	}
	
	int CSglHostAllCtoAPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();
		
		// ��������
		nSize += sizeof(BYTE);
		
		// ������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecSglHostState.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecSglHostState.at(i).StreamSize();
		}
		
		return nSize;
	}
	
	int CSglHostAllCtoAPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqSglHostAllCtoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}
		
		int nBufLeft = nBufLen;
		
		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// ������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecSglHostState.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecSglHostState.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}
		
		return nOffset;
	}
	
	int CSglHostAllCtoAPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqSglHostAllCtoAPkt SglHostAndService_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		for(int i = 0; i < byLen; i++)
		{
			CSglHostState objSglHostAndService;
			nRet = objSglHostAndService.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}
			
			m_vecSglHostState.push_back(objSglHostAndService);
			nOffset += nRet;
		}
		
		return nOffset;
	}

	//==============================================================================
	//
	//             CSglHostCHGCtoAPkt
	//             �仯��������������״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	CSglHostCHGCtoAPkt::CSglHostCHGCtoAPkt()
	{
		m_byFunc = FUNC_SIGLEHOSTANDSERVICECHG_APPTOCTRL;
	}

	CSglHostCHGCtoAPkt::~CSglHostCHGCtoAPkt()
	{

	}

	CProtocolHead* CSglHostCHGCtoAPkt::Clone()
	{
		CProtocolHead* pClone = CSglHostAllCtoAPkt::Clone();

		pClone->SetFunc(FUNC_SIGLEHOSTANDSERVICECHG_APPTOCTRL);

		return pClone;
	}

	//==============================================================================
	//
	//             CDeviceInfoEx
	//             ���豸�������豸��Ϣ
	//
	//==============================================================================
	CDeviceInfoEx::CDeviceInfoEx()
	{

	}

	CDeviceInfoEx::~CDeviceInfoEx()
	{

	}

	bool CDeviceInfoEx::operator == (CDeviceInfoEx& other )
	{
		bool bEqual = false;
		if ((CDeviceInfo)*this == (CDeviceInfo)other)
		{
			bEqual = true;
		}

		return bEqual;
	}

	void CDeviceInfoEx::SetDevName(const string& strName)
	{
		m_strDevName = strName;
	}

	string CDeviceInfoEx::GetDevName() const
	{
		return m_strDevName;
	}

	int CDeviceInfoEx::StreamSize()
	{
		int nSize = CDeviceInfo::StreamSize();
		nSize += 1 + (int)m_strDevName.size();

		return nSize;
	}

	int CDeviceInfoEx::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CDeviceInfoEx len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// DeviceInfo
		int nOffset = CDeviceInfo::ToStreamBuf(pBuf, nBufLen);

		// �豸����
		BYTE byLen = (BYTE)MIN(0xff, m_strDevName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strDevName.c_str(), byLen);
		nOffset += byLen;

		return nOffset;
	}

	int CDeviceInfoEx::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < CDeviceInfo::StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfoEx Deviceinfo len\n");
			return -1;
		}
		int nOffset = CDeviceInfo::FromStreamBuf(pBuf, nBufLen);

		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfoEx Device Desc len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfoEx Device Desc len\n");
			return -1;
		}
		char strTemp[MAX_BYTESIZE + 1];
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strDevName = strTemp;
		nOffset += byLen;

		return nOffset;
	}

	//==============================================================================
	//
	//             CProgramClient
	//             �ͻ�����Ϣ��
	//
	//==============================================================================
	CProgramClient::CProgramClient()
	{

	}

	CProgramClient::~CProgramClient()
	{

	}

	void CProgramClient::SetCltName(const string& strName)
	{
		m_strCltName = strName;
	}

	string CProgramClient::GetCltName() const
	{
		return m_strCltName;
	}

	int CProgramClient::StreamSize()
	{
		int nSize = CForwardAddr::Size() + 1 + (int)m_strCltName.size();

		return nSize;
	}

	int CProgramClient::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CProgramClient len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = CForwardAddr::ToStreamBuf(pBuf);

		BYTE byLen = (BYTE)MIN(0xff, m_strCltName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strCltName.c_str(), byLen);
		nOffset += byLen;

		return nOffset;
	}

	int CProgramClient::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < CForwardAddr::Size())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProgramClient Device Info len\n");
			return -1;
		}
		int nOffset = CForwardAddr::FromStreamBuf(pBuf);

		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProgramClient Device Desc len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProgramClient Device Desc len\n");
			return -1;
		}
		char strTemp[MAX_BYTESIZE + 1];
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strCltName = strTemp;
		nOffset += byLen;

		return nOffset;
	}

	//==============================================================================
	//
	//             CClientConnMainCommtoSysEx
	//             �ͻ�����Ϣ��
	//
	//==============================================================================
	CClientConnMainCommtoSysEx::CClientConnMainCommtoSysEx()
	{

	}

	CClientConnMainCommtoSysEx::~CClientConnMainCommtoSysEx()
	{

	}

	void CClientConnMainCommtoSysEx::SetProgramClient(CProgramClient& objClt)
	{
		m_objClt = objClt;
	}

	CProgramClient* CClientConnMainCommtoSysEx::GetProgramClient()
	{
		return &m_objClt;
	}

	void CClientConnMainCommtoSysEx::SetConnState(CConnState& objConnState)
	{
		m_objConnState = objConnState;
	}

	CConnState* CClientConnMainCommtoSysEx::GetConnState()
	{
		return &m_objConnState;
	}

	int CClientConnMainCommtoSysEx::StreamSize()
	{
		int nSize = m_objClt.StreamSize() + m_objConnState.StreamSize();

		return nSize;
	}

	int CClientConnMainCommtoSysEx::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CClientConnMainCommtoSysEx len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;
		int nOffset = m_objClt.ToStreamBuf(pBuf, nBufLeft);

		nBufLeft = nBufLen - nOffset;
		nOffset += m_objConnState.ToStreamBuf(pBuf + nOffset, nBufLeft);

		return nOffset;
	}

	int CClientConnMainCommtoSysEx::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet = m_objClt.FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		int nOffset = nRet;

		nRet = m_objConnState.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	//==============================================================================
	//
	//             CProgramServerEx
	//             ���������Ϣ��
	//
	//==============================================================================
	CProgramServerEx::CProgramServerEx()
	{

	}

	CProgramServerEx::~CProgramServerEx()
	{

	}

	string CProgramServerEx::GetSvcName() const
	{
		return m_strSvcName;
	}

	void CProgramServerEx::SetSvcName(const string& strName)
	{
		m_strSvcName = strName;
	}

	int CProgramServerEx::StreamSize()
	{
		int nSize = CProgramServer::StreamSize();

		nSize += 1 + (int)m_strSvcName.size();

		return nSize;
	}

	int CProgramServerEx::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CProgramServerEx len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;
		int nOffset = CProgramServer::ToStreamBuf(pBuf, nBufLeft);

		BYTE byLen = (BYTE)MIN(0xff, m_strSvcName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strSvcName.c_str(), byLen);
		nOffset += byLen;

		return nOffset;
	}

	int CProgramServerEx::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet = CProgramServer::FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		int nOffset = nRet;

		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProgramServerEx Desc len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProgramServerEx Desc len\n");
			return -1;
		}
		char strTemp[MAX_BYTESIZE + 1];
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strSvcName = strTemp;
		nOffset += byLen;

		return nOffset;
	}

	//==============================================================================
	//
	//			CSvrConnStateEx
	//			������̵�����״̬
	//
	//==============================================================================
	CSvrConnStateEx::CSvrConnStateEx()
	{

	}

	CSvrConnStateEx::~CSvrConnStateEx()
	{

	}

	void CSvrConnStateEx::SetProgramServer(CProgramServerEx& objSvc)
	{
		m_objSvc = objSvc;
	}

	CProgramServerEx* CSvrConnStateEx::GetProgramServer()
	{
		return &m_objSvc;
	}

	void CSvrConnStateEx::SetConnInfo(CConnInfo& objConnInfo)
	{
		m_objConnInfo = objConnInfo;
	}

	CConnInfo* CSvrConnStateEx::GetConnInfo()
	{
		return &m_objConnInfo;
	}

	void CSvrConnStateEx::Clear()
	{
		m_vecClientConnMainCommtoSys.clear();
	}

	void CSvrConnStateEx::AddClient(const CClientConnMainCommtoSysEx& objClient)
	{
		m_vecClientConnMainCommtoSys.push_back(objClient);
	}

	WORD CSvrConnStateEx::GetClientCount() const
	{
		return (WORD)m_vecClientConnMainCommtoSys.size();
	}

	CClientConnMainCommtoSysEx* CSvrConnStateEx::GetClient(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecClientConnMainCommtoSys.size())
		{
			return &(m_vecClientConnMainCommtoSys.at(nIndex));
		}

		return NULL;
	}

	CClientConnMainCommtoSysEx* CSvrConnStateEx::GetClientByAddr(CForwardAddr& objAddr)
	{
		size_t i = 0;
		for (i = 0; i < m_vecClientConnMainCommtoSys.size(); i++)
		{
			if (objAddr == (CForwardAddr)(*(m_vecClientConnMainCommtoSys[i].GetProgramClient())))
			{
				return &m_vecClientConnMainCommtoSys[i];
			}
		}

		return NULL;
	}

	int CSvrConnStateEx::StreamSize()
	{
		int nSize = m_objSvc.StreamSize();

		nSize += m_objConnInfo.StreamSize();

		nSize += 2;
		for (int i = 0; i < m_vecClientConnMainCommtoSys.size(); i++)
		{
			nSize += m_vecClientConnMainCommtoSys.at(i).StreamSize();
		}

		return nSize;
	}

	int CSvrConnStateEx::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSvrConnStateEx len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;
		int nOffset = m_objSvc.ToStreamBuf(pBuf, nBufLeft);

		nBufLeft = nBufLen - nOffset;
		nOffset += m_objConnInfo.ToStreamBuf(pBuf + nOffset, nBufLeft);

		WORD wLen = (WORD)MIN(MAX_WORDSIZE, m_vecClientConnMainCommtoSys.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		for (int i = 0; i < wLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecClientConnMainCommtoSys.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSvrConnStateEx::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecClientConnMainCommtoSys.clear();

		int nRet = m_objSvc.FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		int nOffset = nRet;

		nRet = m_objConnInfo.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		if (nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSvrConnStateEx Clt count len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		for (int i = 0; i < wLen; i++)
		{
			CClientConnMainCommtoSysEx objClt;
			nRet = objClt.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (nRet == -1)
			{
				return -1;
			}
			m_vecClientConnMainCommtoSys.push_back(objClt);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//			CReqSglSysAllCtoAPkt
	//			����ȫ�嵥��������ϵͳ״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	CReqSglSysAllCtoAPkt::CReqSglSysAllCtoAPkt()
	{
		SetType(TYPE_MAINTEN);
		SetFunc(FUNC_SYSINFO_REQ_CTRLTOAPP);
	}

	CReqSglSysAllCtoAPkt::~CReqSglSysAllCtoAPkt()
	{

	}

	//==============================================================================
	//
	//			CMoniInfoStandbyState
	//			��������״̬
	//
	//==============================================================================
	CMoniInfoStandbyState::CMoniInfoStandbyState()
		:m_byState(0)
	{
		m_byMoniType = EMTT_STANDBY;
	}

	CMoniInfoStandbyState::~CMoniInfoStandbyState()
	{

	}

	void CMoniInfoStandbyState::SetState(BYTE byActStdbySta)
	{
		m_byState = byActStdbySta;
	}

	BYTE CMoniInfoStandbyState::GetState() const
	{
		return m_byState;
	}

	void CMoniInfoStandbyState::SetChangeTime(time_t tm, WORD wMs)
	{
		m_ChangeTime.Set(tm, wMs);
	}

	CProtocolTime* CMoniInfoStandbyState::GetChangeProtocolTime()
	{
		return &m_ChangeTime;
	}

	void CMoniInfoStandbyState::SetChangeProtocolTime(CProtocolTime* tTime)
	{
		m_ChangeTime = *tTime;
	}

	int CMoniInfoStandbyState::StreamSize()
	{
		int nSize = CMoniInfoBase::StreamSize();
		// ����״̬����
		nSize += sizeof(BYTE);
		// ����״̬�ı�ʱ��
		nSize += m_ChangeTime.StreamSize();

		return nSize;
	}

	int CMoniInfoStandbyState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSvrConnStateEx len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//����״̬
		pBuf[nOffset] = m_byState;
		nOffset += 1;

		//����״̬�ı�ʱ��
		nBufLeft = nBufLen - nOffset;
		nOffset += m_ChangeTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		return nOffset;
	}

	int CMoniInfoStandbyState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��7������״̬
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		//����״̬
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buffer of CMoniInfoStandbyState is less than Standby length\n");
			return -1;
		}
		m_byState = pBuf[nOffset];
		nOffset += 1;

		// ����״̬�ı�ʱ��
		nRet = m_ChangeTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	//==============================================================================
	//
	//			CMoniInfoSvrConnState
	//			��������״̬
	//
	//==============================================================================
	CMoniInfoSvrConnState::CMoniInfoSvrConnState()
	{
		m_byMoniType = EMTT_SERVICECONN;
	}

	CMoniInfoSvrConnState::~CMoniInfoSvrConnState()
	{
		m_vecSvrConnStateEx.clear();
	}

	int CMoniInfoSvrConnState::GetSvrConnStateExCount() const
	{
		return (int)m_vecSvrConnStateEx.size();
	}

	CSvrConnStateEx* CMoniInfoSvrConnState::GetSvrConnStateEx(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecSvrConnStateEx.size())
		{
			return &m_vecSvrConnStateEx[nIndex];
		}

		return NULL;
	}

	void CMoniInfoSvrConnState::Add(const CSvrConnStateEx& objSvrConnStateEx)
	{
		if (m_vecSvrConnStateEx.size() < 0xff)
		{
			m_vecSvrConnStateEx.push_back(objSvrConnStateEx);
		}
	}

	CSvrConnStateEx* CMoniInfoSvrConnState::GetSvrConnStateBySvrId(WORD wSvrId)
	{
		size_t i = 0;
		for (i = 0; i < m_vecSvrConnStateEx.size(); i++)
		{
			if (wSvrId == m_vecSvrConnStateEx[i].GetProgramServer()->GetSvcID())
			{
				return &m_vecSvrConnStateEx[i];
			}
		}

		return NULL;
	}

	void CMoniInfoSvrConnState::Clear()
	{
		m_vecSvrConnStateEx.clear();
	}

	int CMoniInfoSvrConnState::StreamSize()
	{
		//���ͳ���
		int nSize = CMoniInfoBase::StreamSize();

		//��������
		nSize += sizeof(BYTE);

		//����
		BYTE byLen = (BYTE)MIN(0xff, m_vecSvrConnStateEx.size());
		for (int i = 0; i < byLen; i++)
		{
			//ÿ��������Ϣ����
			nSize +=  m_vecSvrConnStateEx.at(i).StreamSize();
		}

		return nSize;
	}

	int CMoniInfoSvrConnState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniSvcConnSta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��8����������״̬
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//����˸���
		BYTE byLen = (BYTE)MIN(0xff, m_vecSvrConnStateEx.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//�������Ϣ
		for (int i=0; i<byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecSvrConnStateEx.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoSvrConnState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecSvrConnStateEx.clear();

		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��8������״̬
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ����˸���
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess CSvrConnStateEx len\n");
			return -1;
		}
		BYTE  byProcessNum = pBuf[nOffset];
		nOffset += 1;		

		// ����������Ϣ
		for (int i=0; i<byProcessNum; i++)
		{
			CSvrConnStateEx objSvrConnStateEx;
			nRet = objSvrConnStateEx.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecSvrConnStateEx.push_back(objSvrConnStateEx);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//             CMoniInfoStaChannelState
	//                  վͨ��״̬
	//
	//==============================================================================
	CMoniInfoStaChannelState::CMoniInfoStaChannelState()
	{
		m_byMoniType = EMTT_CHAN;
	}

	CMoniInfoStaChannelState::~CMoniInfoStaChannelState()
	{
		Clear();
	}

	CMoniInfoStaChannelState::CMoniInfoStaChannelState(const CMoniInfoStaChannelState& objMoniStaChannel)
	{
		*this = objMoniStaChannel;
	}

	CMoniInfoStaChannelState& CMoniInfoStaChannelState::operator=(const CMoniInfoStaChannelState& objMoniStaChannel)
	{
		Clear();

		for (int i = 0; i < objMoniStaChannel.GetStaCommDevCount(); i++)
		{
			Add((*objMoniStaChannel.m_vecStaCommDev[i]));
		}

		return *this;
	}	

	int CMoniInfoStaChannelState::GetStaCommDevCount() const
	{
		return (int)m_vecStaCommDev.size();
	}

	CStaCommDev* CMoniInfoStaChannelState::GetStaCommDev(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecStaCommDev.size())
		{
			return m_vecStaCommDev[nIndex];
		}

		return NULL;
	}

	void CMoniInfoStaChannelState::Add(const CStaCommDev& objStaCommDev)
	{
		if (m_vecStaCommDev.size() < MAX_WORDSIZE)
		{
			CStaCommDev* pNew = new CStaCommDev();
			*pNew = objStaCommDev;
			m_vecStaCommDev.push_back(pNew);
		}
	}

	void CMoniInfoStaChannelState::Clear()
	{
		for (int i = 0; i < m_vecStaCommDev.size(); i++)
		{
			delete m_vecStaCommDev.at(i);
			m_vecStaCommDev.at(i) = NULL;
		}

		m_vecStaCommDev.clear();
	}

	int CMoniInfoStaChannelState::StreamSize()
	{
		//���ͳ���
		int nSize = CMoniInfoBase::StreamSize();

		//��������
		nSize += sizeof(WORD);

		//����
		WORD wLen = (BYTE)MIN(MAX_WORDSIZE, m_vecStaCommDev.size());
		for (int i = 0 ; i < wLen ; i++)
		{
			//ÿ��վͨ������
			nSize +=  m_vecStaCommDev.at(i)->StreamSize();
		}

		return nSize;
	}

	int CMoniInfoStaChannelState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniChanSta len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//���ü�����ͣ�ENUM_MAINTAIN_TYPE��9��ͨ��״̬
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//��վ����
		WORD wLen = (WORD)MIN(MAX_WORDSIZE, m_vecStaCommDev.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		//ͨ����Ϣ
		for (int i=0; i<wLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecStaCommDev.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoStaChannelState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		Clear();

		int nOffset = 0;

		//ȡ������ͣ�ENUM_MAINTAIN_TYPE��9��ͨ��״̬
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// ��վ����
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess CStaCommDev len\n");
			return -1;
		}
		WORD  wChannelNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;		

		// ͨ����Ϣ
		for (int i=0; i<wChannelNum; i++)
		{
			CStaCommDev objStaCommDev;
			nRet = objStaCommDev.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			Add(objStaCommDev);
			nOffset += nRet;
		}

		return nOffset;
	}

	vector<CStaCommDev*>& CMoniInfoStaChannelState::GetAllStaCommDev()
	{
		return m_vecStaCommDev;
	}

	//==============================================================================
	//
	//								CDeviceSysState
	//             �豸��ϵͳ״̬��Ϣ������״̬����������״̬��վͨ��״̬��
	//
	//==============================================================================
	CDeviceSysState::CDeviceSysState()
		: m_byValidity(VALIDITY_YES)
	{

	}

	CDeviceSysState::~CDeviceSysState()
	{
		Clear();
	}

	CDeviceSysState::CDeviceSysState(const CDeviceSysState& objDeviceSysState)
	{
		*this = objDeviceSysState;
	}

	CDeviceSysState& CDeviceSysState::operator=(const CDeviceSysState& objDeviceSysState)
	{
		m_objDeviceInfoEx = objDeviceSysState.m_objDeviceInfoEx;
		m_byValidity = objDeviceSysState.m_byValidity;

		CMoniInfoBase* pMoniInfoBase;
		for (int i = 0; i < objDeviceSysState.GetMoniInfoCount(); i++)
		{
			pMoniInfoBase = objDeviceSysState.m_vecMoniInfo.at(i);
			// ���
			AddMoniInfo(pMoniInfoBase);
		}

		return *this;
	}

	void CDeviceSysState::SetDeviceInfo(CDeviceInfoEx& objDevInfo)
	{
		m_objDeviceInfoEx = objDevInfo;
	}

	CDeviceInfoEx* CDeviceSysState::GetDeviceInfo()
	{
		return &m_objDeviceInfoEx;
	}

	CDeviceInfoEx const* CDeviceSysState::GetDeviceInfo() const
	{
		return &m_objDeviceInfoEx;
	}

	void CDeviceSysState::SetValidity(BYTE byValidity)
	{
		m_byValidity = byValidity;
	}

	BYTE CDeviceSysState::GetValidity() const
	{
		return m_byValidity;
	}

	int CDeviceSysState::GetMoniInfoCount() const
	{
		return (int)m_vecMoniInfo.size();
	}

	CMoniInfoBase* CDeviceSysState::GetMoniInfo(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecMoniInfo.size())
		{
			return m_vecMoniInfo[nIndex];
		}

		return NULL;
	}

	void CDeviceSysState::AddMoniInfo(const CMoniInfoBase* pMoniInfoBase)
	{
		assert(pMoniInfoBase);
		CMoniInfoStandbyState* pMoniSvcActStdbySta;
		CMoniInfoSvrConnState* pMoniSvcConnSta;
		CMoniInfoStaChannelState* pMoniChanSta;

		switch (pMoniInfoBase->GetMoniType())
		{
		case EMTT_STANDBY:			//��������״̬
			pMoniSvcActStdbySta = (CMoniInfoStandbyState*)pMoniInfoBase;
			// ���
			AddSvrActStbyState(pMoniSvcActStdbySta);
			break;

		case EMTT_SERVICECONN:		//��������״̬
			pMoniSvcConnSta = (CMoniInfoSvrConnState*)pMoniInfoBase;
			// ���
			AddSvcConnState(pMoniSvcConnSta);
			break;

		case EMTT_CHAN:			//ͨ��״̬
			pMoniChanSta = (CMoniInfoStaChannelState*)pMoniInfoBase;
			// ���
			AddChanState(pMoniChanSta);
			break;

		default:
			break;
		}
	}

	int CDeviceSysState::StreamSize()
	{
		int nSize = m_objDeviceInfoEx.StreamSize();

		// ����״̬
		nSize += 1;

		// �����Ϣ����
		nSize += 1;

		//����
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		for (int i = 0 ; i < byLen ; i++)
		{
			//ÿ����ַ��Ϣ����
			nSize +=  m_vecMoniInfo.at(i)->StreamSize();
		}

		return nSize;
	}

	int CDeviceSysState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CDeviceSysState len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		nOffset = m_objDeviceInfoEx.ToStreamBuf(pBuf, nBufLeft);

		// ��Ϣ��Ч��
		pBuf[nOffset] = m_byValidity;
		nOffset += 1;

		//�������͸���
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//��������
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfo.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CDeviceSysState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		Clear();

		int nRet, nOffset = 0;

		nRet = m_objDeviceInfoEx.FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset = nRet;

		// ��Ϣ��Ч��
		m_byValidity = pBuf[nOffset];
		nOffset += 1;

		// �������͸���
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceSysState MoniTypeInfo_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		//����������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			// ����
			BYTE byType = pBuf[nOffset];
			CMoniInfoBase* pMoniTypeInfo = NULL;

			switch (byType)
			{
			case EMTT_STANDBY:
				pMoniTypeInfo = new CMoniInfoStandbyState();
				break;
			case EMTT_SERVICECONN:
				pMoniTypeInfo = new CMoniInfoSvrConnState();
				break;
			case EMTT_CHAN:
				pMoniTypeInfo = new CMoniInfoStaChannelState();
				break;
			default:
				break;
			}

			if (pMoniTypeInfo == NULL)
			{
				continue;
			}

			nRet = pMoniTypeInfo->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				delete pMoniTypeInfo;
				pMoniTypeInfo = NULL;
				return -1;
			}

			m_vecMoniInfo.push_back(pMoniTypeInfo);
			nOffset += nRet;
		}

		return nOffset;
	}

	//��������״̬
	void CDeviceSysState::AddSvrActStbyState(const CMoniInfoStandbyState* pMoniSvcActStdbySta)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoStandbyState* pInfo = new CMoniInfoStandbyState;
		*pInfo = *pMoniSvcActStdbySta;
		m_vecMoniInfo.push_back(pInfo);
	}

	//��������״̬
	void CDeviceSysState::AddSvcConnState(const CMoniInfoSvrConnState* pSvrConnState)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoSvrConnState* pInfo = new CMoniInfoSvrConnState;
		*pInfo  = *pSvrConnState;
		m_vecMoniInfo.push_back(pInfo);
	}

	//ͨ��״̬
	void CDeviceSysState::AddChanState(const CMoniInfoStaChannelState* pStaCommDev)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoStaChannelState* pInfo = new CMoniInfoStaChannelState;
		*pInfo  = *pStaCommDev;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CDeviceSysState::Clear()
	{
		vector<CMoniInfoBase*>::iterator it;
		for (it = m_vecMoniInfo.begin(); it != m_vecMoniInfo.end(); it++)
		{
			CMoniInfoBase* pItem = *it;
			assert(pItem);
			delete pItem;
			pItem = NULL;
		}
		m_vecMoniInfo.clear();
	}

	//==============================================================================
	//
	//             CSglSysAll
	//             ����������Ϣ
	//
	//==============================================================================
	CSglSysAll::CSglSysAll()
	{

	}

	CSglSysAll::~CSglSysAll()
	{
		
	}

	void CSglSysAll::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CSglSysAll::GetHostName() const
	{
		return m_strHostName;
	}

	void CSglSysAll::SetHostDesc(string strHostDesc)
	{
		m_strHostDesc = strHostDesc;
	}
	string CSglSysAll::GetHostDesc() const
	{
		return m_strHostDesc;
	}

	int CSglSysAll::GetDeviceSysStateCount() const
	{
		return (int)m_vecDeviceSysState.size();
	}

	CDeviceSysState* CSglSysAll::GetDeviceSysState(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecDeviceSysState.size());
		return &m_vecDeviceSysState[nIndex];
	}

	void CSglSysAll::Add(const CDeviceSysState& objDeviceSysState)
	{
		if (m_vecDeviceSysState.size() < 0xff)
		{
			m_vecDeviceSysState.push_back(objDeviceSysState);
		}
	}

	void CSglSysAll::Clear()
	{
		m_vecDeviceSysState.clear();
	}

	int CSglSysAll::StreamSize()
	{
		int nSize = 0;
		nSize = 1 + (int)m_strHostName.size() + 1 + (int)m_strHostDesc.size() + 1;

		BYTE byLen = (BYTE)MIN(0xff, m_vecDeviceSysState.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecDeviceSysState.at(i).StreamSize();
		}
		return nSize;
	}

	int CSglSysAll::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSglSysAll len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		// ����������
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// ������
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// ������������
		byLen = (BYTE)MIN(0xff, m_strHostDesc.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		// ��������
		memcpy(pBuf + nOffset, m_strHostDesc.c_str(), byLen);
		nOffset += byLen;

		//�豸��ַ����
		byLen = (BYTE)MIN(0xff, m_vecDeviceSysState.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// �豸��ַ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecDeviceSysState.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSglSysAll::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;

		// ����������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglSysAll HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// ������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglSysAll HostName len\n");
			return -1;
		}
		char str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostName = str0xff;
		nOffset += byLen;

		// ������������
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglSysAll m_strHostDesc len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		// ��������
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglSysAll m_strHostDesc len\n");
			return -1;
		}
		str0xff[0xff+1];
		memcpy(str0xff, pBuf + nOffset, byLen);
		str0xff[byLen] = 0;
		m_strHostDesc = str0xff;
		nOffset += byLen;

		// ��ַ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglSysAll MoniTypeInfo_len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CDeviceSysState objDeviceSysState;
			nRet = objDeviceSysState.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				objDeviceSysState.Clear();
				return -1;
			}

			m_vecDeviceSysState.push_back(objDeviceSysState);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//             CSglSysAllAtoCPkt
	//             ȫ�嵥��������ϵͳ״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	CSglSysAllAtoCPkt::CSglSysAllAtoCPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_SYSINFO_ALL_APPTOCTRL)
	{

	}

	CSglSysAllAtoCPkt::~CSglSysAllAtoCPkt()
	{
		m_vecSglSysAll.clear();
	}

	int CSglSysAllAtoCPkt::GetSglSysAllCount() const
	{
		return (int)m_vecSglSysAll.size();
	}

	CSglSysAll* CSglSysAllAtoCPkt::GetSglSysAll(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vecSglSysAll.size());
		return &m_vecSglSysAll[nIndex];
	}

	void CSglSysAllAtoCPkt::Add(CSglSysAll& objSglSysAll)
	{
		if (m_vecSglSysAll.size() < 0xff)
		{
			m_vecSglSysAll.push_back(objSglSysAll);
		}
	}

	void CSglSysAllAtoCPkt::Clear()
	{
		m_vecSglSysAll.clear();
	}

	//�ѿ�¡������֮ǰ��·ͳһ��������ŵ������������
	CProtocolHead* CSglSysAllAtoCPkt::Clone()
	{
		CSglSysAllAtoCPkt* pClone = new CSglSysAllAtoCPkt();
		*pClone = *this;
		return pClone;
	}

	int CSglSysAllAtoCPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();


		// ������Ϣ����
		nSize += sizeof(BYTE);

		// ������Ϣ
		BYTE byLen = (BYTE)MIN(0xff, m_vecSglSysAll.size());
		for(int i = 0; i < byLen; i++)
		{
			nSize += m_vecSglSysAll.at(i).StreamSize();
		}

		return nSize;
	}

	int CSglSysAllAtoCPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqSglHostAllCtoAPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// ������Ϣ����
		BYTE byLen = (BYTE)MIN(0xff, m_vecSglSysAll.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// ������Ϣ
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecSglSysAll.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSglSysAllAtoCPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// ������Ϣ����
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSglSysAllAtoCPkt SglHostAndService_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		for(int i = 0; i < byLen; i++)
		{
			CSglSysAll objSglSysAll;
			nRet = objSglSysAll.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if(nRet == -1)
			{
				return -1;
			}

			m_vecSglSysAll.push_back(objSglSysAll);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//             CSglSysCHGAtoCPkt
	//            �仯����������ϵͳ״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	CSglSysCHGAtoCPkt::CSglSysCHGAtoCPkt()
	{
		SetType(TYPE_MAINTEN);
		SetFunc(FUNC_SYSINFO_CHG_APPTOCTRL);
	}

	CSglSysCHGAtoCPkt::~CSglSysCHGAtoCPkt()
	{

	}
}
