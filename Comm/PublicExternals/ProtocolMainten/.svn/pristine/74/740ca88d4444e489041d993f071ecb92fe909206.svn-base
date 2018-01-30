//=========================================================================================
//
//							 ProtocolMaintenSysInfo.cpp
//
// begin    : 2016-12-16
// describe : 系统状态信息协议集
//
//=========================================================================================
#include "ProtocolMainten.h"
#include "ProtocolMaintenSysInfo.h"

namespace ProtocolMaintenContent
{
	//==============================================================================
	//
	//			CMoniInfoSvrConnStateItoA
	//				 服务连接状态
	//
	//==============================================================================
	CMoniInfoSvrConnStateItoA::CMoniInfoSvrConnStateItoA()
	{
		m_byMoniType = EMTT_SERVICECONN;
	}

	CMoniInfoSvrConnStateItoA::~CMoniInfoSvrConnStateItoA()
	{
		m_vecSvrConnState.clear();
	}

	int CMoniInfoSvrConnStateItoA::GetSvrConnStateCount() const
	{
		return (int)m_vecSvrConnState.size();
	}

	CSvrConnState* CMoniInfoSvrConnStateItoA::GetSvrConnState(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecSvrConnState.size())
		{
			return &m_vecSvrConnState[nIndex];
		}

		return NULL;
	}

	void CMoniInfoSvrConnStateItoA::Add(const CSvrConnState& objSvrConnState)
	{
		if (m_vecSvrConnState.size() < 0xff)
		{
			m_vecSvrConnState.push_back(objSvrConnState);
		}
	}

	CSvrConnState* CMoniInfoSvrConnStateItoA::GetSvrConnStateBySvrId(WORD wSvrId)
	{
		size_t i = 0;
		for (i = 0; i < m_vecSvrConnState.size(); i++)
		{
			if (wSvrId == m_vecSvrConnState[i].GetProgramServer()->GetSvcID())
			{
				return &m_vecSvrConnState[i];
			}
		}

		return NULL;
	}

	void CMoniInfoSvrConnStateItoA::Clear()
	{
		m_vecSvrConnState.clear();
	}

	int CMoniInfoSvrConnStateItoA::StreamSize()
	{
		//类型长度
		int nSize = CMoniInfoBase::StreamSize();

		//个数长度
		nSize += sizeof(BYTE);

		//个数
		BYTE byLen = (BYTE)MIN(0xff, m_vecSvrConnState.size());
		for (int i = 0; i < byLen; i++)
		{
			//每个进程信息长度
			nSize +=  m_vecSvrConnState.at(i).StreamSize();
		}

		return nSize;
	}

	int CMoniInfoSvrConnStateItoA::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMoniInfoSvrConnStateItoA len\n");
			nBufLen = StreamSize();
			return -1;
		}

		//设置监控类型：ENUM_MAINTAIN_TYPE，8：服务连接状态
		int nBufLeft = nBufLen;
		int nOffset = CMoniInfoBase::ToStreamBuf(pBuf, nBufLeft);

		//服务端个数
		BYTE byLen = (BYTE)MIN(0xff, m_vecSvrConnState.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		//服务端信息
		for (int i=0; i<byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecSvrConnState.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMoniInfoSvrConnStateItoA::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		Clear();

		int nOffset = 0;

		//取监控类型：ENUM_MAINTAIN_TYPE，8：连接状态
		int nRet = CMoniInfoBase::FromStreamBuf(pBuf, nBufLen);
		if (nRet < 0)
		{
			LogImportant("FromStreamBuf: nOffset of CMoniInfoBase is less than 0\n");
			return -1;
		}
		nOffset += nRet;

		// 服务端个数
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProcess CSvrConnState len\n");
			return -1;
		}
		BYTE  byProcessNum = pBuf[nOffset];
		nOffset += 1;		

		// 服务连接信息
		for (int i=0; i<byProcessNum; i++)
		{
			CSvrConnState objSvrConnState;
			nRet = objSvrConnState.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (-1 == nRet)
			{
				return -1;
			}
			m_vecSvrConnState.push_back(objSvrConnState);
			nOffset += nRet;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//				CSvrAllInfo
	//				服务器信息类
	//
	//==============================================================================
	CSvrAllInfo::CSvrAllInfo()
		: m_byValidity(VALIDITY_YES)
	{

	}

	CSvrAllInfo::CSvrAllInfo(const CSvrAllInfo& objSvrAllInfo)
	{
		*this = objSvrAllInfo;
	}

	CSvrAllInfo::~CSvrAllInfo()
	{
		Clear();
	}

	CSvrAllInfo& CSvrAllInfo::operator=(const CSvrAllInfo& objSvrAllInfo)
	{
		m_objDevice = objSvrAllInfo.m_objDevice;
		m_byValidity = objSvrAllInfo.m_byValidity;

		const CMoniInfoBase* pMoniInfoBase;
		for (int i = 0; i < objSvrAllInfo.GetMoniInfoCount(); i++)
		{
			pMoniInfoBase = objSvrAllInfo.GetMoniInfo(i);
			// 深拷贝
			AddMoniInfo(pMoniInfoBase);
		}

		return *this;
	}

	void CSvrAllInfo::SetDevice(CDeviceInfo& objDevice)
	{
		m_objDevice = objDevice;
	}

	CDeviceInfo* CSvrAllInfo::GetDevice()
	{
		return &m_objDevice;
	}

	void CSvrAllInfo::SetValidity(BYTE byValidity)
	{
		m_byValidity = byValidity;
	}

	BYTE CSvrAllInfo::GetValidity() const
	{
		return m_byValidity;
	}

	int CSvrAllInfo::GetMoniInfoCount() const
	{
		return (int)m_vecMoniInfo.size();
	}

	CMoniInfoBase* CSvrAllInfo::GetMoniInfo(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < m_vecMoniInfo.size())
		{
			return m_vecMoniInfo[nIndex];
		}

		return NULL;
	}

	void CSvrAllInfo::AddMoniInfo(const CMoniInfoBase* pMoniInfoBase)
	{
		assert(pMoniInfoBase);
		CMoniInfoStandbyState* pMoniSvcActStdbySta;
		CMoniInfoSvrConnStateItoA* pMoniSvcConnSta;
		CMoniInfoStaChannelState* pMoniChanSta;

		switch (pMoniInfoBase->GetMoniType())
		{
		case EMTT_STANDBY:			//主备服务状态
			pMoniSvcActStdbySta = (CMoniInfoStandbyState*)pMoniInfoBase;
			// 深拷贝
			AddSvrActStbyState(pMoniSvcActStdbySta);
			break;

		case EMTT_SERVICECONN:		//服务连接状态
			pMoniSvcConnSta = (CMoniInfoSvrConnStateItoA*)pMoniInfoBase;
			// 深拷贝
			AddSvcConnState(pMoniSvcConnSta);
			break;

		case EMTT_CHAN:			//通道状态
			pMoniChanSta = (CMoniInfoStaChannelState*)pMoniInfoBase;
			// 深拷贝
			AddChanState(pMoniChanSta);
			break;

		default:
			break;
		}
	}

	int CSvrAllInfo::StreamSize()
	{
		int nSize = m_objDevice.StreamSize();

		// 故障状态
		nSize += 1;

		// 监控信息个数
		nSize += 1;

		//个数
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		for (int i = 0; i < byLen; i++)
		{
			//每个地址信息长度
			nSize +=  m_vecMoniInfo.at(i)->StreamSize();
		}

		return nSize;
	}

	int CSvrAllInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSvrAllInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		nOffset = m_objDevice.ToStreamBuf(pBuf, nBufLeft);

		// 信息有效性
		pBuf[nOffset] = m_byValidity;
		nOffset += 1;

		// 监视类型个数
		BYTE byLen = (BYTE)MIN(0xff, m_vecMoniInfo.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 监视类型
		for(int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecMoniInfo.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSvrAllInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		Clear();

		int nRet, nOffset = 0;

		nRet = m_objDevice.FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset = nRet;

		// 信息有效性
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSvrAllInfo Validity len");
			return -1;
		}
		m_byValidity = pBuf[nOffset];
		nOffset += 1;

		// 监视类型个数
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSvrAllInfo MoniTypeInfo len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 监视类型信息
		for(int i = 0; i < byLen; i++)
		{
			// 类型
			BYTE byType = pBuf[nOffset];
			CMoniInfoBase* pMoniTypeInfo = NULL;

			switch (byType)
			{
			case EMTT_STANDBY:
				pMoniTypeInfo = new CMoniInfoStandbyState();
				break;
			case EMTT_SERVICECONN:
				pMoniTypeInfo = new CMoniInfoSvrConnStateItoA();
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

	void CSvrAllInfo::AddSvrActStbyState(const CMoniInfoStandbyState* pMoniSvcActStdbySta)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoStandbyState* pInfo = new CMoniInfoStandbyState;
		*pInfo = *pMoniSvcActStdbySta;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CSvrAllInfo::AddSvcConnState(const CMoniInfoSvrConnStateItoA* pSvrConnState)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoSvrConnStateItoA* pInfo = new CMoniInfoSvrConnStateItoA;
		*pInfo  = *pSvrConnState;
		m_vecMoniInfo.push_back(pInfo);
	}

	void CSvrAllInfo::AddChanState(CMoniInfoStaChannelState* pStaCommDev)
	{
		if (m_vecMoniInfo.size() >= 0xff)
		{
			return;
		}

		CMoniInfoStaChannelState* pInfo = new CMoniInfoStaChannelState;
		for (int i = 0; i < pStaCommDev->GetStaCommDevCount(); i++)
		{
			pInfo->Add(*(pStaCommDev->GetStaCommDev(i)));
		}

		m_vecMoniInfo.push_back(pInfo);
	}

	void CSvrAllInfo::Clear()
	{
		vector<CMoniInfoBase*>::iterator it;
		for (it = m_vecMoniInfo.begin(); it != m_vecMoniInfo.end(); it++)
		{
			if (*it)
			{
				delete *it;
				*it = NULL;
			}
		}
		m_vecMoniInfo.clear();
	}

	//==============================================================================
	//
	//				CMainSysAllInfoPkt
	//				全体主系统状态信息【RmInterSys→RmAppHostMgr】
	//
	//==============================================================================
	CMainSysAllInfoPkt::CMainSysAllInfoPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_SYSALL_INTERTOAPP)
	{

	}

	CMainSysAllInfoPkt::~CMainSysAllInfoPkt()
	{
		ClearAllSvr();
	}

	void CMainSysAllInfoPkt::ClearAllSvr()
	{
		for (int i = 0; i < m_vecSvr.size(); i++)
		{
			delete m_vecSvr.at(i);
			m_vecSvr.at(i) = NULL;
		}
		m_vecSvr.clear();
	}

	void CMainSysAllInfoPkt::AddSvr(const CSvrAllInfo& objSvr)
	{
		CSvrAllInfo* pNew = new CSvrAllInfo();
		*pNew = objSvr;
		m_vecSvr.push_back(pNew);
	}

	BYTE CMainSysAllInfoPkt::GetSvrCount() const
	{
		return (BYTE)m_vecSvr.size();
	}

	CSvrAllInfo* CMainSysAllInfoPkt::GetSvr(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecSvr.size())
		{
			return m_vecSvr.at(nIndex);
		}

		return NULL;
	}

	CProtocolHead* CMainSysAllInfoPkt::Clone()
	{
		CMainSysAllInfoPkt* pClone = new CMainSysAllInfoPkt();

		for (int i = 0; i < m_vecSvr.size(); i++)
		{
			pClone->AddSvr(*m_vecSvr.at(i));
		}
		return pClone;
	}

	int CMainSysAllInfoPkt::StreamSize()
	{
		// 报文首部
		int nSize = CProtocolHead::StreamSize();

		// 服务器个数
		nSize += 1;

		// 服务器信息
		for (int i = 0; i < m_vecSvr.size(); i++)
		{
			nSize += m_vecSvr.at(i)->StreamSize();
		}

		return nSize;
	}

	int CMainSysAllInfoPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		// 判断buffer的大小
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMainSysAllInfoPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nBufLeft = nBufLen;
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);
		
		// 服务器数量
		BYTE byLen = (BYTE)MIN(MAX_BYTESIZE, m_vecSvr.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// 服务器信息
		for (int i = 0; i < byLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecSvr.at(i)->ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMainSysAllInfoPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		ClearAllSvr();

		// CProtocolHead
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		int nOffset = nRet;
		
		// 服务器数量
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAllInfoPkt Server len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 服务器信息
		for (int i = 0; i < byLen; i++)
		{
			CSvrAllInfo* pSvr = new CSvrAllInfo;
			nRet = pSvr->FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (nRet == -1)
			{
				delete pSvr;
				return -1;
			}
			nOffset += nRet;
			m_vecSvr.push_back(pSvr);
		}

		return nOffset;
	}

	//==============================================================================
	//
	//				CMainSysChangedInfoPkt
	//				变化主系统状态信息【RmInterSys→RmAppHostMgr】
	//
	//==============================================================================
	CMainSysChangedInfoPkt::CMainSysChangedInfoPkt()
	{
		m_byFunc = FUNC_SYSCHG_INTERTOAPP;
	}

	CMainSysChangedInfoPkt::~CMainSysChangedInfoPkt()
	{

	}

	//==============================================================================
	//
	//				CMainSysAlarmPkt
	//				//主系统报警信息【RmInterSys→RmAppHostMgr】
	//
	//==============================================================================
	CMainSysAlarmPkt::CMainSysAlarmPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_SYSALM_INTERTOAPP)
	{

	}

	CMainSysAlarmPkt::~CMainSysAlarmPkt()
	{

	}

	CDeviceInfo* CMainSysAlarmPkt::GetDevice()
	{
		return &m_objDecive;
	}

	void CMainSysAlarmPkt::SetName(const string& strName)
	{
		m_strHostName = strName;
	}

	string CMainSysAlarmPkt::GetName() const
	{
		return m_strHostName;
	}

	int CMainSysAlarmPkt::GetAlarmCount() const
	{
		return (int)m_vecAlarm.size();
	}

	CAlarmAllEvt* CMainSysAlarmPkt::GetAlarm(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecAlarm.size())
		{
			return &(m_vecAlarm.at(nIndex));
		}

		return NULL;
	}

	void CMainSysAlarmPkt::Add(const CAlarmAllEvt& objAlarm)
	{
		m_vecAlarm.push_back(objAlarm);
	}

	void CMainSysAlarmPkt::Clear()
	{
		m_vecAlarm.clear();
	}

	CProtocolHead* CMainSysAlarmPkt::Clone()
	{
		CMainSysAlarmPkt* pClone = new CMainSysAlarmPkt();
		*pClone = *this;
		return pClone;
	}

	int CMainSysAlarmPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();

		// 主机名长度 + 主机名
		nSize += 1 + (int)m_strHostName.size();

		// 局码 + 单位类型 + 单位Id + 设备类型 + 设备Id
		nSize += 1 + 1 + 2 + 1 + 2;

		// 报警数量
		nSize += 4;

		// 报警信息
		for (int i = 0; i < m_vecAlarm.size(); i++)
		{
			nSize += m_vecAlarm.at(i).StreamSize();
		}

		return nSize;
	}

	int CMainSysAlarmPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		// 判断buffer的大小
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMainSysAlarmPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nBufLeft = nBufLen;
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);


		// 主机名长度
		BYTE byLen = (BYTE)MIN(MAX_BYTESIZE, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 主机名
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 局码
		pBuf[nOffset] = m_objDecive.GetBureauId();
		nOffset += 1;
		
		// 单位类型
		pBuf[nOffset] = m_objDecive.GetUnitType();
		nOffset += 1;

		// 单位Id
		SETSTREAMSHORT(pBuf + nOffset, m_objDecive.GetUnitId());
		nOffset += 2;
		
		// 设备类型
		pBuf[nOffset] = m_objDecive.GetDeviceType();
		nOffset += 1;
		
		// 设备Id
		SETSTREAMSHORT(pBuf + nOffset, m_objDecive.GetDeviceId());
		nOffset += 2;

		// 报警数量
		DWORD dwLen = (DWORD)MIN(0xffffffff, m_vecAlarm.size());
		SETSTREAMINT(pBuf + nOffset, dwLen);
		nOffset += 4;

		// 报警信息
		for (int i = 0; i < dwLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecAlarm.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CMainSysAlarmPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// CProtocolHead
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		int nOffset = nRet;

		// 主机名长度
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt Hostname Length len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 主机名
		if (nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt Hostname len\n");
			return -1;
		}
		char strTemp[MAX_BYTESIZE];
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strHostName = strTemp;
		nOffset += byLen;

		// 局码
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt BureauId len\n");
			return -1;
		}
		m_objDecive.SetBureauId(pBuf[nOffset]);
		nOffset += 1;

		// 单位类型
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt UnitType len\n");
			return -1;
		}
		m_objDecive.SetUnitType(pBuf[nOffset]);
		nOffset += 1;

		// 单位Id
		if (nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt UnitId len\n");
			return -1;
		}
		m_objDecive.SetUnitId(GETSTREAMSHORT(pBuf + nOffset));
		nOffset += 2;

		// 设备类型
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt DevType len\n");
			return -1;
		}
		m_objDecive.SetDeviceType(pBuf[nOffset]);
		nOffset += 1;

		// 设备Id
		if (nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt DevId len\n");
			return -1;
		}
		m_objDecive.SetDeviceId(GETSTREAMSHORT(pBuf + nOffset));
		nOffset += 2;

		// 报警数量
		if (nBufLen < (nOffset + 4))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CMainSysAlarmPkt Alarm Num len\n");
			return -1;
		}
		DWORD dwLen = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 报警信息
		m_vecAlarm.clear();
		for (int i = 0; i < dwLen; i++)
		{
			CAlarmAllEvt objEvt;
			nRet = objEvt.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (nRet == -1)
			{
				return -1;
			}
			nOffset += nRet;
			m_vecAlarm.push_back(objEvt);
		}

		return nOffset;
	}
}
