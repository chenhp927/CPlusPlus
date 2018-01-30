//=========================================================================================
//
//							 ProtocolSysUpdate.cpp
//
// begin    : 2016-12-27
// describe : 系统升级协议
//
//=========================================================================================
#include "ProtocolSysUpdate.h"
#include "ProtocolMainten.h"


namespace ProtocolSysUpdate
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//             CReqSysCurVerPkt
	//             请求主机当前软件数据版本【RmControlMonitor→RmInterHost】
	//
	//==============================================================================
	CReqSysCurVerPkt::CReqSysCurVerPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_HOSTCURVER_REQ_CTRLTOAPP)
	{
	}

	CReqSysCurVerPkt::~CReqSysCurVerPkt()
	{

	}

	CProtocolHead* CReqSysCurVerPkt::Clone()
	{
		CReqSysCurVerPkt* pClone = new CReqSysCurVerPkt();
		*pClone = *this;
		return pClone;
	}

	void CReqSysCurVerPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CReqSysCurVerPkt::GetHostName() const
	{
		return m_strHostName;
	}

	void CReqSysCurVerPkt::AddRoute(string strRoute)
	{
		m_vecRoute.push_back(strRoute);
	}

	string CReqSysCurVerPkt::GetRoute(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_vecRoute.size())
		{
			return NULL;
		}

		return m_vecRoute.at(nIndex);
	}

	int CReqSysCurVerPkt::GetRouteCount() const
	{
		return (int)m_vecRoute.size();
	}

	int CReqSysCurVerPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();

		// 主机名长度
		nSize += 1;

		// 主机名
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		nSize += byLen;

		// 路径数量
		WORD wNum = (WORD)MIN(0xffff, m_vecRoute.size());
		nSize += 2;

		// 路径
		for (int i = 0; i < wNum; i++)
		{
			// 路径长度
			nSize += 2;
			// 路径
			WORD wLen = (WORD)MIN(0xffff, m_vecRoute[i].size());
			nSize += wLen;
		}

		return nSize;
	}

	int CReqSysCurVerPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqSysCurVerPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// 主机名长度
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 主机名
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 路径数量
		WORD wNum = (WORD)MIN(0xffff, m_vecRoute.size());
		SETSTREAMSHORT(pBuf + nOffset, wNum);
		nOffset += 2;

		// 路径
		for (int i = 0; i < wNum; i++)
		{
			// 路径长度
			WORD wLen = (WORD)MIN(0xffff, m_vecRoute[i].size());
			SETSTREAMSHORT(pBuf + nOffset, wLen);
			nOffset += 2;

			// 路径
			memcpy(pBuf + nOffset, m_vecRoute[i].c_str(), wLen);
			nOffset += wLen;
		}

		return nOffset;
	}

	int CReqSysCurVerPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 主机名长度
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqSysCurVerPkt HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 主机名
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqSysCurVerPkt HostName len\n");
			return -1;
		}
		char cHostName[0xff+1];
		memcpy(cHostName, pBuf + nOffset, byLen);
		cHostName[byLen] = '\0';
		m_strHostName = cHostName;
		nOffset += byLen;

		// 路径数量
		WORD wRouteNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		for (int i = 0; i < wRouteNum; i++)
		{
			// 路径长度
			if(nBufLen < (nOffset + 2))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CReqSysCurVerPkt RouteName len\n");
				return -1;
			}
			WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
			nOffset += 2;

			// 路径
			if(nBufLen < (nOffset + wLen))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CReqSysCurVerPkt RouteName\n");
				return -1;
			}
			string strRoute;
			char cRouteName[0xffff+1];
			memcpy(cRouteName, pBuf + nOffset, wLen);
			cRouteName[wLen] = '\0';
			strRoute = cRouteName;
			m_vecRoute.push_back(strRoute);
			nOffset += wLen;
		}

		return nOffset;
	}

	//==============================================================================
	//
	//								CRouteVerInfo
	//             主机软件数据当前版本信息【RmInterHost →RmControlMonitor】路径信息
	//
	//==============================================================================
	CRouteVerInfo::CRouteVerInfo()
		: m_strRouteName("")
		, m_dwVerID(0)
		, m_strLogInfo("")
	{
	}

	CRouteVerInfo::~CRouteVerInfo()
	{

	}

	void CRouteVerInfo::SetRouteName(string strRouteName)
	{
		m_strRouteName = strRouteName;
	}

	string CRouteVerInfo::GetRouteName() const
	{
		return m_strRouteName;
	}

	void CRouteVerInfo::SetVerID(DWORD dwVerID)
	{
		m_dwVerID = dwVerID;
	}

	DWORD CRouteVerInfo::GetVerID() const
	{
		return m_dwVerID;
	}

	void CRouteVerInfo::SetLogInfo(string strLog)
	{
		m_strLogInfo = strLog;
	}

	string CRouteVerInfo::GetLogInfo() const
	{
		return m_strLogInfo;
	}

	void CRouteVerInfo::SetTime(time_t tm, WORD wMs)
	{
		m_Time.Set(tm, wMs);
	}

	void CRouteVerInfo::SetTime(timeval t)
	{
		m_Time.Set(t);
	}

	CProtocolTime CRouteVerInfo::GetProCommitTime() const
	{
		return m_Time;
	}

	timeval CRouteVerInfo::GetCommitTime() const
	{
		return m_Time.GetTimeval();
	}

	int CRouteVerInfo::StreamSize()
	{
		int nSize = 0;

		// 路径名称长度
		nSize += 2;
		// 路径名称
		WORD wLen = (WORD)MIN(0xffff, m_strRouteName.size());
		nSize += wLen;
		// 版本号
		nSize += 4;
		// 日志信息长度
		nSize += 2;
		// 日志信息
		wLen = (WORD)MIN(0xffff, m_strLogInfo.size());
		nSize += wLen;
		// 更新、提交时间
		nSize += CProtocolTime::StreamSize();

		return nSize;
	}

	int CRouteVerInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CRouteVerInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		int nOffset = 0;

		// 路径名称长度
		WORD wLen = (WORD)MIN(0xffff, m_strRouteName.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径
		memcpy(pBuf + nOffset, m_strRouteName.c_str(), wLen);
		nOffset += wLen;

		// 版本号
		SETSTREAMINT(pBuf + nOffset, m_dwVerID);
		nOffset += 4;

		// 日志长度
		wLen = (WORD)MIN(0xffff, m_strLogInfo.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 日志信息
		memcpy(pBuf + nOffset, m_strLogInfo.c_str(), wLen);
		nOffset += wLen;

		// 提交或更新时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_Time.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}

		nOffset += nRet;

		return nOffset;
	}

	int CRouteVerInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int  nOffset = 0;
		// 路径长度
		if(nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteVerInfo RouteName len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 路径
		if(nBufLen < (nOffset + wLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteVerInfo RouteName\n");
			return -1;
		}
		string strRoute;
		char cRouteName[0xffff+1];
		memcpy(cRouteName, pBuf + nOffset, wLen);
		cRouteName[wLen] = '\0';
		m_strRouteName = cRouteName;
		nOffset += wLen;

		// 版本号
		if (nBufLen < nOffset+4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteVerInfo VerId\n");
			return -1;
		}
		m_dwVerID = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 日志信息长度
		if(nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteVerInfo Log len\n");
			return -1;
		}
		wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 日志信息
		if(nBufLen < (nOffset + wLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteVerInfo Log\n");
			return -1;
		}
		char cLogInfo[0xffff+1];
		memcpy(cLogInfo, pBuf + nOffset, wLen);
		cLogInfo[wLen] = '\0';
		m_strLogInfo = cLogInfo;
		nOffset += wLen;

		// 提交或更新时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_Time.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	//==============================================================================
	//
	//								CSysCurVerInfoPkt
	//             主机软件数据当前版本信息【RmInterHost →RmControlMonitor】
	//
	//==============================================================================
	CSysCurVerInfoPkt::CSysCurVerInfoPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_HOSTCURVER_REPLY_APPTOCTRL)
		, m_strHostName("")
	{

	}

	CSysCurVerInfoPkt::~CSysCurVerInfoPkt()
	{

	}

	void CSysCurVerInfoPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CSysCurVerInfoPkt::GetHostName() const
	{
		return m_strHostName;
	}

	void CSysCurVerInfoPkt::AddRoute(CRouteVerInfo& objRouteVerInfo)
	{
		m_vecRouteInfo.push_back(objRouteVerInfo);
	}

	CRouteVerInfo* CSysCurVerInfoPkt::GetRoute(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecRouteInfo.size())
		{
			return NULL;
		}

		return &m_vecRouteInfo.at(nIndex);
	}

	int CSysCurVerInfoPkt::GetRouteCount()
	{
		return (int)m_vecRouteInfo.size();
	}

	CProtocolHead* CSysCurVerInfoPkt::Clone()
	{
		CSysCurVerInfoPkt* pClone = new CSysCurVerInfoPkt();
		*pClone = *this;
		return pClone;
	}

	int CSysCurVerInfoPkt::StreamSize()
	{
		int nSize = 0;
		nSize = (int)(CProtocolHead::StreamSize() + 1 + m_strHostName.size());
		for (int i = 0; i < m_vecRouteInfo.size(); i++)
		{
			nSize += m_vecRouteInfo[i].StreamSize();
		}	
		return nSize;
	}

	int CSysCurVerInfoPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSysCurVerInfoPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;
		// 包头
		int nRet = CProtocolHead::ToStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;


		// 主机名长度
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 主机名
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 路径数量
		WORD wNum = (WORD)MIN(0xffff, m_vecRouteInfo.size());
		SETSTREAMSHORT(pBuf + nOffset, wNum);
		nOffset += 2;
		// 路径信息
		for (int i = 0; i < wNum; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			nRet = m_vecRouteInfo[i].ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}
		return nOffset;
	}

	int CSysCurVerInfoPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// 包头
		int nRet = CProtocolHead::FromStreamBuf(pBuf + nOffset, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 主机名长度
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSysCurVerInfoPkt HostName_Len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 主机名
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSysCurVerInfoPkt HostName len\n");
			return -1;
		}
		char cHostName[0xff+1];
		memcpy(cHostName, pBuf + nOffset, byLen);
		cHostName[byLen] = '\0';
		m_strHostName = cHostName;
		nOffset += byLen;

		// 路径数量
		if(nBufLen < (nOffset + sizeof(WORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSysCurVerInfoPkt RouteNum Len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 路径信息
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			CRouteVerInfo objRouteVerInfo;
			nRet = objRouteVerInfo.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
			AddRoute(objRouteVerInfo);
		}

		return nOffset;
	}

	//==============================================================================
	//
	//						CRouteUpdateInfo
	//             更新主机软件数据版本【RmControlMonitor→RmInterHost】
	//
	//==============================================================================
	CRouteUpdateInfo::CRouteUpdateInfo()
		: m_strRouteName("")
		, m_dwVerID(0)
	{

	}

	CRouteUpdateInfo::~CRouteUpdateInfo()
	{

	}

	void CRouteUpdateInfo::SetRouteName(string strRouteName)
	{
		m_strRouteName = strRouteName;
	}

	string CRouteUpdateInfo::GetRouteName() const
	{
		return m_strRouteName;
	}

	void CRouteUpdateInfo::SetVerID(DWORD dwVeID)
	{
		m_dwVerID = dwVeID;
	}

	DWORD CRouteUpdateInfo::GetVerID() const
	{
		return m_dwVerID;
	}

	int CRouteUpdateInfo::StreamSize()
	{
		return 2 + (WORD)MIN(0xffff, m_strRouteName.size()) + 4;
	}

	int CRouteUpdateInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSingleHostCfg len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 路径名长度
		WORD wLen = (WORD)MIN(0xffff, m_strRouteName.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径名
		memcpy(pBuf + nOffset, m_strRouteName.c_str(), wLen);
		nOffset += wLen;

		// 版本号
		SETSTREAMINT(pBuf + nOffset, m_dwVerID);
		nOffset += 4;
		return nOffset;
	}

	int CRouteUpdateInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		// 路径长度
		if(nBufLen < (nOffset + sizeof(WORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateInfo RouteName_len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 路径
		if(nBufLen < (nOffset + wLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateInfo RouteName len\n");
			return -1;
		}
		char cRouteName[0xffff+1];
		memcpy(cRouteName, pBuf + nOffset, wLen);
		cRouteName[wLen] = '\0';
		m_strRouteName = cRouteName;
		nOffset += wLen;

		// 版本号
		if(nBufLen < (nOffset + sizeof(DWORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateInfo VERID len\n");
			return -1;
		}
		m_dwVerID = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;		

		return nOffset;
	}

	//==============================================================================
	//
	//						CHostUpdateInfo
	//             更新主机软件数据版本【RmControlMonitor→RmInterHost】主机信息
	//
	//==============================================================================
	CHostUpdateInfo::CHostUpdateInfo()
		: m_strHostName("")
	{
		m_vecRouteUpdateInfo.clear();
	}

	CHostUpdateInfo::~CHostUpdateInfo()
	{

	}

	void CHostUpdateInfo::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CHostUpdateInfo::GetHostName() const
	{
		return m_strHostName;
	}

	void CHostUpdateInfo::AddRouteUpdateInfo(CRouteUpdateInfo objRouteUpdateInfo)
	{
		m_vecRouteUpdateInfo.push_back(objRouteUpdateInfo);
	}

	CRouteUpdateInfo*  CHostUpdateInfo::GetRouteUpdateInfo(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecRouteUpdateInfo.size())
		{
			return NULL;
		}

		return &m_vecRouteUpdateInfo.at(nIndex);
	}

	int CHostUpdateInfo::GetRouteCount() const
	{
		return (int)m_vecRouteUpdateInfo.size();
	}

	int CHostUpdateInfo::StreamSize()
	{
		int nSize = 0;
		nSize = 1 + (BYTE)MIN(0xff, m_strHostName.size());

		for (int i = 0; i < m_vecRouteUpdateInfo.size(); i++)
		{
			nSize += m_vecRouteUpdateInfo.at(i).StreamSize();
		}

		return nSize;
	}

	int CHostUpdateInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSingleHostCfg len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 主机名长度
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 主机名
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 路径数量
		WORD wLen = (WORD)MIN(0xffff, m_vecRouteUpdateInfo.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecRouteUpdateInfo[i].ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CHostUpdateInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		// 主机名称长度
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostUpdateInfo HostName_len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;
		
		// 主机名称
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostUpdateInfo HostName len\n");
			return -1;
		}
		char cHostName[0xff+1];
		memcpy(cHostName, pBuf + nOffset, byLen);
		cHostName[byLen] = '\0';
		m_strHostName = cHostName;
		nOffset += byLen;

		// 路径数量
		if(nBufLen < (nOffset + sizeof(WORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostUpdateInfo RouteNum len\n");
			return -1;
		}
		WORD dwNum = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;		

		// 路径
		for (int i = 0; i < dwNum; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			CRouteUpdateInfo objRouteInfo;
			int nRet = objRouteInfo.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
			AddRouteUpdateInfo(objRouteInfo);
		}

		return nOffset;
	}


	//==============================================================================
	//
	//						CSysUpdatePkt
	//             更新主机软件数据版本【RmControlMonitor→RmInterHost】
	//
	//==============================================================================
	CSysUpdatePkt::CSysUpdatePkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_HOSTVERUPDATE_REQ_CTRLTOAPP)
		, m_byUpdatePara(0)
	{
		m_vecHostUpdateInfo.clear();
	}

	CSysUpdatePkt::~CSysUpdatePkt()
	{

	}

	void CSysUpdatePkt::SetUpdatePara(BYTE byUpdatePara)
	{
		m_byUpdatePara = byUpdatePara;
	}

	BYTE CSysUpdatePkt::GetUpdatePara() const
	{
		return m_byUpdatePara;
	}

	void CSysUpdatePkt::AddHostUpdateInfo(CHostUpdateInfo& objHostUpdateInfo)
	{		
		m_vecHostUpdateInfo.push_back(objHostUpdateInfo);	
	}

	CHostUpdateInfo* CSysUpdatePkt::GetHostUpdateInfo(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecHostUpdateInfo.size())
		{
			return NULL;
		}
		return &m_vecHostUpdateInfo[nIndex];
	}

	int CSysUpdatePkt::GetHostCount() const
	{
		return (int)m_vecHostUpdateInfo.size();
	}

	CProtocolHead* CSysUpdatePkt::Clone()
	{
		CSysUpdatePkt* pClone = new CSysUpdatePkt();
		*pClone = *this;
		return pClone;
	}

	int CSysUpdatePkt::StreamSize()
	{
		// CProtocolHead
		int nSize = CProtocolHead::StreamSize();

		// 更新参数
		nSize += 1;

		// 主机个数
		nSize += sizeof(WORD);

		// 主机
		WORD wLen = (WORD)MIN(0xffff, m_vecHostUpdateInfo.size());
		for(int i = 0; i < wLen; i++)
		{
			nSize += m_vecHostUpdateInfo.at(i).StreamSize();
		}

		return nSize;
	}

	int CSysUpdatePkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSysUpdatePkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLen);

		pBuf[nOffset] = m_byUpdatePara;
		nOffset += 1;

		// 主机个数
		WORD wLen = (WORD)MIN(0xffff, m_vecHostUpdateInfo.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 主机信息
		for(int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecHostUpdateInfo.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CSysUpdatePkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 更新参数
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSysUpdatePkt updatepara len\n");
			return -1;
		}
		m_byUpdatePara = pBuf[nOffset];
		nOffset += 1;

		// 主机个数
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSysUpdatePkt Host-len len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;
		LogImportant("主机数量%d\n", wLen);
		// 主机
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			CHostUpdateInfo objHostUpdateInfo;
			nRet = objHostUpdateInfo.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
			AddHostUpdateInfo(objHostUpdateInfo);
		}

		return nOffset;
	}

	//==============================================================================
	//
	//						CRouteUpdateRep
	//             更新主机软件数据版本回执【RmInterHost→RmControlMonitor】信息
	//
	//==============================================================================
	CRouteUpdateRep::CRouteUpdateRep()
		: m_strRouteName("")
		, m_byRslt(0)
		, m_dwVerID(0)
		, m_strRslt("")
	{
		
	}

	CRouteUpdateRep::~CRouteUpdateRep()
	{

	}

	void CRouteUpdateRep::SetRouteName(string strRouteName)
	{
		m_strRouteName = strRouteName;
	}

	string CRouteUpdateRep::GetRouteName() const
	{
		return m_strRouteName;
	}

	void CRouteUpdateRep::SetRsltValue(BYTE byRsltValue)
	{
		m_byRslt = byRsltValue;
	}

	BYTE CRouteUpdateRep::GetRsltValue() const
	{
		return m_byRslt;
	}

	void CRouteUpdateRep::SetVerID(DWORD dwVerID)
	{
		m_dwVerID = dwVerID;
	}

	DWORD CRouteUpdateRep::GetVerID() const
	{
		return m_dwVerID;
	}

	void CRouteUpdateRep::SetRsltString(string strRslt)
	{
		m_strRslt = strRslt;
	}

	string  CRouteUpdateRep::GetRsltString() const
	{
		return m_strRslt;
	}

	int CRouteUpdateRep::StreamSize()
	{
		return (int)(2 + m_strRouteName.size() + 1 + 4 + 2 + m_strRslt.size());
	}

	int CRouteUpdateRep::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CRouteUpdateRep len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 路径名称长度
		WORD wLen = (WORD)MIN(0xffff, m_strRouteName.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);		
		nOffset += 2;

		// 路径		
		memcpy(pBuf + nOffset, m_strRouteName.c_str(), wLen);
		nOffset += wLen;		

		// 更新结果
		pBuf[nOffset] = m_byRslt;
		nOffset += 1;

		// 当前版本号
		SETSTREAMINT(pBuf + nOffset, m_dwVerID);
		nOffset += 4;

		// 回执长度
		wLen = (WORD)MIN(0xffff, m_strRslt.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);		
		nOffset += 2;

		// 回执内容
		memcpy(pBuf + nOffset, m_strRslt.c_str(), wLen);
		nOffset += wLen;

		return nOffset;
	}

	int CRouteUpdateRep::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		// 路径长度
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateRep RouteName—len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf+nOffset);
		nOffset += 2;

		// 路径
		if(nBufLen < (nOffset + wLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateRep RouteName len\n");
			return -1;
		}
		char cRouteName[0xffff+1];
		memcpy(cRouteName, pBuf + nOffset, wLen);
		cRouteName[wLen] = '\0';
		m_strRouteName = cRouteName;
		nOffset += wLen;

		// 更新结果
		m_byRslt = pBuf[nOffset];
		nOffset += 1;

		// 当前版本号
		m_dwVerID = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 回执内容长度
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateRep Rslt—len\n");
			return -1;
		}
		wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 回执内容
		if(nBufLen < (nOffset + wLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteUpdateRep strrslt len\n");
			return -1;
		}
		char cRsltName[0xffff+1];
		memcpy(cRsltName, pBuf + nOffset, wLen);
		cRsltName[wLen] = '\0';
		m_strRslt = cRsltName;
		nOffset += wLen;

		return nOffset;
	}

	//==============================================================================
	//
	//							CHostUpdateRepPkt
	//             更新主机软件数据版本回执【RmInterHost→RmControlMonitor】
	//
	//==============================================================================
	CHostUpdateRepPkt::CHostUpdateRepPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_HOSTVERUPDATE_REPLY_APPCTRL)
		, m_strHostName("")
	{
		m_vecRouteUpdateRep.clear();
	}
	
	CHostUpdateRepPkt::~CHostUpdateRepPkt()
	{

	}

	void CHostUpdateRepPkt::SetHostName(string strHostName) 
	{
		m_strHostName = strHostName;
	}

	string CHostUpdateRepPkt::GetHostName() const	
	{
		return m_strHostName;
	}

	CRouteUpdateRep* CHostUpdateRepPkt::GetRouteUpdateReq(int nIndex)			
	{
		if (nIndex < 0 || nIndex >= m_vecRouteUpdateRep.size())
		{
			return NULL;
		}
		return &m_vecRouteUpdateRep[nIndex];
	}

	void CHostUpdateRepPkt::AddRouteUpdateRep(const CRouteUpdateRep& objRouteUdateRep)
	{
		m_vecRouteUpdateRep.push_back(objRouteUdateRep);
	}

	int CHostUpdateRepPkt::GetRouteCount()			
	{
		return (int)m_vecRouteUpdateRep.size();
	}

	CProtocolHead* CHostUpdateRepPkt::Clone()
	{
		CHostUpdateRepPkt* pClone = new CHostUpdateRepPkt();
		*pClone = *this;
		return pClone;
	}

	int CHostUpdateRepPkt::StreamSize()
	{
		int nSize = (int)(CProtocolHead::StreamSize() + 1 + m_strHostName.size());
		for (int i = 0; i < m_vecRouteUpdateRep.size(); i++)
		{
			nSize += m_vecRouteUpdateRep[i].StreamSize();
		}
		
		return nSize;
	}

	int CHostUpdateRepPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostUpdateRepPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLen);

		// 主机名长度
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;
		
		// 主机名称
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 路径数量
		WORD wLen = (WORD)MIN(0xffff, m_vecRouteUpdateRep.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径信息
		for(int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecRouteUpdateRep.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CHostUpdateRepPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 主机名称长度
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostUpdateRepPkt HostName_len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 主机名称
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostUpdateRepPkt HostName len\n");
			return -1;
		}
		char cHostName[0xff+1];
		memcpy(cHostName, pBuf + nOffset, byLen);
		cHostName[byLen] = '\0';
		m_strHostName = cHostName;
		nOffset += byLen;
		
		// 路径个数
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CHostUpdateRepPkt Route—len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 路径
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			CRouteUpdateRep objRouteRep;
			nRet = objRouteRep.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
			AddRouteUpdateRep(objRouteRep);
		}

		return nOffset;
	}

	//==============================================================================
	//
	//							CVerInfo
	//          存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	CVerInfo::CVerInfo()
		: m_dwVerID(0)
	{
		
	}

	CVerInfo::~CVerInfo()
	{

	}

	void CVerInfo::SetVerID(DWORD dwVerId)
	{
		m_dwVerID = dwVerId;
	}

	DWORD CVerInfo::GetVerID() const
	{
		return m_dwVerID;
	}

	void CVerInfo::SetUpdateTime(time_t tm, WORD wMs)
	{
		m_tmUpdate.Set(tm, wMs);
	}

	void CVerInfo::SetUpdateTime(timeval t)
	{
		m_tmUpdate.Set(t);
	}

	CProtocolTime CVerInfo::GetUpateTime() const
	{
		return m_tmUpdate;
	}

	timeval CVerInfo::GetUpdateTimeVal() const
	{
		return m_tmUpdate.GetTimeval();
	}
	
	int CVerInfo::StreamSize()
	{
		return CProtocolTime::StreamSize() + 4;
	}

	int CVerInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CVerInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;
		
		// 版本号
		SETSTREAMSHORT(pBuf + nOffset, m_dwVerID);
		nOffset += 4;		

		// 更新时间
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_tmUpdate.ToStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	int CVerInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CVerInfo len\n");
			return -1;
		}

		// 版本号
		if (nBufLen < nOffset + 4)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CVerInfo verid len\n");
			return -1;
		}
		m_dwVerID = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 更新时间
		if (nBufLen < nOffset + CProtocolTime::StreamSize())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CVerInfo updatetime len\n");
			return -1;
		}
		int nLenTmp = nBufLen - nOffset;
		int nRet = m_tmUpdate.FromStreamBuf(pBuf + nOffset, nLenTmp);
		if (-1 == nRet)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}


	//==============================================================================
	//
	//							CRouteHisVerInfo
	//          存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	CRouteHisVerInfo::CRouteHisVerInfo()
		: m_strRouteName("")
	{
		
	}

	CRouteHisVerInfo::~CRouteHisVerInfo()
	{

	}

	void CRouteHisVerInfo::SetRouteName(string strRouteName)
	{
		m_strRouteName = strRouteName;
	}

	string CRouteHisVerInfo::GetRouteName() const
	{
		return m_strRouteName;
	}

	void CRouteHisVerInfo::AddVerInfo(CVerInfo objVerInfo)
	{
		m_vecHisVerInfo.push_back(objVerInfo);
	}

	CVerInfo* CRouteHisVerInfo::GetVerInfo(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecHisVerInfo.size())
		{
			return NULL;
		}
		return &m_vecHisVerInfo[nIndex];
	}

	int CRouteHisVerInfo::StreamSize()
	{
		int nSize = 0; 
		nSize = (int)(2 + m_strRouteName.size() + 2 + 4 * m_vecHisVerInfo.size());

		return nSize;
	}

	int CRouteHisVerInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CRouteHisVerInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}
		int nOffset = 0;

		// 路径名称长度
		WORD wLen = (WORD)MIN(0xffff, m_strRouteName.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径名称
		memcpy(pBuf + nOffset, m_strRouteName.c_str(), wLen);
		nOffset += wLen;

		// 版本号数量
		wLen = (WORD)MIN(0xffff, m_vecHisVerInfo.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 版本号
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecHisVerInfo[i].ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CRouteHisVerInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		
		if (nBufLen < StreamSize())
		{
			LogImportant("FromStreamBuf:  Buflen is less than the CRouteHisVerInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// 路径名称长度
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteHisVerInfo Route-len\n");
			nBufLen = StreamSize();
			return -1;
		}
		WORD wLen = (WORD)MIN(0xffff, m_strRouteName.size());
		nOffset += 2;

		// 路径名称
		if (nBufLen < nOffset + wLen)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteHisVerInfo m_strRouteName len\n");
			return -1;
		}
		char cRouteName[0xffff+1];
		memcpy(cRouteName, pBuf + nOffset, wLen);
		cRouteName[wLen] = '\0';
		m_strRouteName = cRouteName;
		nOffset += wLen;

		// 版本号数量
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CRouteHisVerInfo VERID len\n");
			return -1;
		}
		wLen = (WORD)MIN(0xffff, m_vecHisVerInfo.size());
		nOffset += 2;

		// 版本号
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			CVerInfo objVerInfo;
			int nRet = objVerInfo.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}

			nOffset += nRet;
			AddVerInfo(objVerInfo);
		}

		return nOffset;
	}

	int CRouteHisVerInfo::GetVerCount()
	{
		return (int)m_vecHisVerInfo.size();
	}


	//==============================================================================
	//
	//							CSaveHostVerPkt
	//				存储主机软件数据版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	CSaveHostVerPkt::CSaveHostVerPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_HOSTSAVEVER_REQ_CTRLTOOSM)
		, m_strHostName("")
	{
		m_vecRouteHisVer.clear();
	}

	CSaveHostVerPkt::~CSaveHostVerPkt()
	{

	}
	
	void CSaveHostVerPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CSaveHostVerPkt::GetHostName() const
	{
		return m_strHostName;
	}

	void CSaveHostVerPkt::AddRouteInfo(const CRouteHisVerInfo objRouteHisVerInfo)
	{
		m_vecRouteHisVer.push_back(objRouteHisVerInfo);
	}
	
	CRouteHisVerInfo* CSaveHostVerPkt::GetRouteInfo(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_vecRouteHisVer.size())
		{
			return NULL;
		}		

		return &(m_vecRouteHisVer[nIndex]);
	}

	int CSaveHostVerPkt::GetRouteCount()
	{
		return (int)m_vecRouteHisVer.size();
	}
	
	CProtocolHead* CSaveHostVerPkt::Clone()
	{
		CSaveHostVerPkt* pClone = new CSaveHostVerPkt();
		*pClone = * this;
		return pClone;
	}

	int CSaveHostVerPkt::StreamSize()
	{
		int nSize = (int)(CProtocolHead::StreamSize() + m_strHostName.size() + 2);
		for (int i = 0; i < m_vecRouteHisVer.size(); i++)
		{
			nSize += m_vecRouteHisVer[i].StreamSize();
		}

		return nSize;
	}

	int CSaveHostVerPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostUpdateRepPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLen);

		// 主机名长度
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 主机名称
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 路径数量
		WORD wLen = (WORD)MIN(0xffff, m_vecRouteHisVer.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径信息
		for(int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			int nRet = m_vecRouteHisVer.at(i).ToStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
		}

		return nOffset;
	}

	int CSaveHostVerPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 主机名称长度
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSaveHostVerPkt HostName_len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 主机名称
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSaveHostVerPkt HostName len\n");
			return -1;
		}
		char cHostName[0xff+1];
		memcpy(cHostName, pBuf + nOffset, byLen);
		cHostName[byLen] = '\0';
		m_strHostName = cHostName;
		nOffset += byLen;

		// 路径个数
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSaveHostVerPkt Route—len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 路径
		for (int i = 0; i < wLen; i++)
		{
			int nLenTmp = nBufLen - nOffset;
			CRouteHisVerInfo objRouteHisInfo;
			nRet = objRouteHisInfo.FromStreamBuf(pBuf + nOffset, nLenTmp);
			if (-1 == nRet)
			{
				return -1;
			}
			nOffset += nRet;
			AddRouteInfo(objRouteHisInfo);
		}

		return nOffset;
	}

	//==============================================================================
	//
	//							CSaveHostVerRepPkt
	//				主机软件数据版本存储回执【工程数据存取服务→RmControlMonitor】
	//
	//==============================================================================
	CSaveHostVerRepPkt::CSaveHostVerRepPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_HOSTSAVEVER_REPLY_OSMTOCTRL)
		, m_dwSaveRslt(0)
		, m_strRsn("")
	{
	}

	CSaveHostVerRepPkt::~CSaveHostVerRepPkt()
	{

	}

	void CSaveHostVerRepPkt::SetRslt(DWORD dwRslt)
	{
		m_dwSaveRslt = dwRslt;
	}

	DWORD CSaveHostVerRepPkt::GetRslt() const
	{
		return m_dwSaveRslt;
	}

	void CSaveHostVerRepPkt::SetRsn(string strRsn)
	{
		m_strRsn = strRsn;
	}

	string CSaveHostVerRepPkt::GetRsn() const
	{
		return m_strRsn;
	}

	CProtocolHead* CSaveHostVerRepPkt::Clone()
	{
		CSaveHostVerRepPkt* pClone = new CSaveHostVerRepPkt();
		*pClone = *this;
		return pClone;
	}

	int CSaveHostVerRepPkt::StreamSize()
	{
		return (int)(CProtocolHead::StreamSize() + 4 + 2 + m_strRsn.size());
	}

	int CSaveHostVerRepPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CHostUpdateRepPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLen);

		// 结果
		SETSTREAMINT(pBuf + nOffset, m_dwSaveRslt);
		nOffset += 4;

		// 失败原因长度
		WORD wLen = (WORD)MIN(0xffff, m_strRsn.size());
		nOffset += 2;
		
		// 失败原因
		memcpy(pBuf + nOffset, m_strRsn.c_str(), wLen);
		nOffset += wLen;
		
		return nOffset;
	}

	int CSaveHostVerRepPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 返回结果
		m_dwSaveRslt = GETSTREAMINT(pBuf + nOffset);
		nOffset += 4;

		// 失败原因长度
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSaveHostVerRepPkt Rsn-len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 失败原因
		if(nBufLen < (nOffset + wLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSaveHostVerRepPkt Rsn len\n");
			return -1;
		}
		char cRsn[0xffff+1];
		memcpy(cRsn, pBuf + nOffset, wLen);
		cRsn[wLen] = '\0';
		m_strRsn = cRsn;
		nOffset += wLen;		

		return nOffset;
	}

	//==============================================================================
	//
	//				            CReqHostHisVerPkt
	//            请求主机软件数据历史版本信息【RmControlMonitor→工程数据存取服务】
	//
	//==============================================================================
	CReqHostHisVerPkt::CReqHostHisVerPkt()
		:CProtocolHead(TYPE_MAINTEN, FUNC_HOSTHISVER_REQ_CTRLTOAPP)
		, m_strHostName("")
	{
		m_vecRouteName.clear();
	}

	CReqHostHisVerPkt::~CReqHostHisVerPkt()
	{
		
	}

	void CReqHostHisVerPkt::SetHostName(string strHostName)
	{
		m_strHostName = strHostName;
	}

	string CReqHostHisVerPkt::GetHostName() const
	{
		return m_strHostName;
	}	

	string CReqHostHisVerPkt::GetRouteName(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_vecRouteName.size())
		{
			return NULL;
		}
		return m_vecRouteName[nIndex];
	}

	void CReqHostHisVerPkt::AddRouteName(string& strRouteName)
	{
		m_vecRouteName.push_back(strRouteName);
	}	

	CProtocolHead* CReqHostHisVerPkt::Clone()
	{
		CReqHostHisVerPkt* pClone = new CReqHostHisVerPkt();
		*pClone = *this;
		return pClone;
	}

	int CReqHostHisVerPkt::StreamSize()
	{
		// CProtocolHead
		int nSize = (int)(CProtocolHead::StreamSize() + 1 + m_strHostName.size() + 2);

		int nLen = (WORD)MIN(0xffff, m_vecRouteName.size());
		for(int i = 0; i < nLen; i++)
		{
			nSize += 2;
			nSize += (int)(m_vecRouteName.at(i).size());
		}

		return nSize;
	}

	int CReqHostHisVerPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CReqHostHisVerPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// CProtocolHead
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// 主机名长度
		BYTE byLen = (BYTE)MIN(0xff, m_strHostName.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		// 主机名
		memcpy(pBuf + nOffset, m_strHostName.c_str(), byLen);
		nOffset += byLen;

		// 路径个数
		WORD wLen = (WORD)MIN(0xffff, m_vecRouteName.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 路径信息
		for(int i = 0; i < wLen; i++)
		{
			// 路径信息长度
			WORD wRouteLen = (WORD)MIN(0xffff, m_vecRouteName.at(i).size());
			SETSTREAMSHORT(pBuf + nOffset, wRouteLen);
			nOffset += 2;

			//路径信息
			memcpy(pBuf + nOffset, m_vecRouteName[i].c_str(), wRouteLen);
			nOffset += wRouteLen;
		}

		return nOffset;
	}

	int CReqHostHisVerPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;
		// CProtocolHead
		nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if(nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 主机名长度
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqHostHisVerPkt HostName_len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		// 主机名
		if(nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CReqHostHisVerPkt HostName len\n");
			return -1;
		}
		char cHost[0xff+1];
		memcpy(cHost, pBuf + nOffset, byLen);
		cHost[byLen] = '\0';
		m_strHostName = cHost;
		nOffset += byLen;

		// 路径信息个数
		if(nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the number len of CReqHostHisVerPkt Route-Num\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 路径信息
		for(int i = 0; i < wLen; i++)
		{
			// 路径长度
			if(nBufLen < (nOffset + sizeof(WORD)))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CReqHostHisVerPkt ROUTE-len\n");
				return -1;
			}
			WORD wRouteLen = GETSTREAMSHORT(pBuf + nOffset);
			nOffset += 2;

			// 路径
			if(nBufLen < (nOffset + wRouteLen))
			{
				LogImportant("FromStreamBuf: Buflen is less than the CReqHostHisVerPkt Route len\n");
				return -1;
			}
			char cRoute[0xffff+1];
			memcpy(cRoute, pBuf + nOffset, wRouteLen);
			cRoute[wRouteLen] = '\0';
			string strRouteName = cRoute;
			m_vecRouteName.push_back(strRouteName);
			nOffset += wRouteLen;			
		}

		return nOffset;
	}

	//==============================================================================
	//
	//							CHostHisVerInfoPkt
	//          主机软件数据历史版本信息【工程数据存取服务→RmControlMonitor】
	//
	//==============================================================================
	CHostHisVerInfoPkt::CHostHisVerInfoPkt()
	{
		m_byType = TYPE_MAINTEN;
		m_byFunc = FUNC_HOSTHISVER_REPLY_APPTOCTRL;
	}

	CHostHisVerInfoPkt::~CHostHisVerInfoPkt()
	{

	}
}