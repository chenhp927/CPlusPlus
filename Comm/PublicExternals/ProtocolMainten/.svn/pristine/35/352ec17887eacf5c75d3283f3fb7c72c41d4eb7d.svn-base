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
	//                CDeviceInfo
	//                设备信息
	//
	//==============================================================================
	CDeviceInfo::CDeviceInfo()
		:m_byBureauId(0),
		m_byUnitType(EST_STATION_TYPE_CENTER),
		m_wUnitId(0),
		m_byDeviceType(EDT_DEVICE_DISPATCHER),
		m_wDeviceId(0)
	{

	}

	CDeviceInfo::~CDeviceInfo()
	{

	}

	bool CDeviceInfo::operator==( const CDeviceInfo& other )
	{
		return (m_byBureauId == other.m_byBureauId
			&& m_byUnitType == other.m_byUnitType
			&& m_wUnitId == other.m_wUnitId
			&& m_byDeviceType == other.m_byDeviceType
			&& m_wDeviceId == other.m_wDeviceId
			&& m_byAbFlag == other.m_byAbFlag);
	}

	void CDeviceInfo::SetBureauId(BYTE byBureauId)
	{
		m_byBureauId = byBureauId;
	}

	BYTE CDeviceInfo::GetBureauId() const
	{
		return m_byBureauId;
	}

	void CDeviceInfo::SetUnitType(BYTE byUnitType)
	{
		m_byUnitType = byUnitType;
	}

	BYTE CDeviceInfo::GetUnitType() const
	{
		return m_byUnitType;
	}

	void CDeviceInfo::SetUnitId(WORD wUnitId)
	{
		m_wUnitId = wUnitId;
	}

	WORD CDeviceInfo::GetUnitId() const
	{
		return m_wUnitId;
	}

	void CDeviceInfo::SetDeviceType(BYTE byDeviceType)
	{
		m_byDeviceType = byDeviceType;
	}

	BYTE CDeviceInfo::GetDeviceType() const
	{
		return m_byDeviceType;
	}

	void CDeviceInfo::SetDeviceId(WORD wDeviceId)
	{
		m_wDeviceId = wDeviceId;
	}

	WORD CDeviceInfo::GetDeviceId() const
	{
		return m_wDeviceId;
	}

	void CDeviceInfo::SetAbFlag(BYTE byAb)
	{
		m_byAbFlag = byAb;
	}

	BYTE CDeviceInfo::GetAbFlag() const
	{
		return m_byAbFlag;
	}

	int CDeviceInfo::StreamSize()
	{
		int nSize = 0;

		// 路局Id
		nSize += sizeof(BYTE);

		// 单位类型
		nSize += sizeof(BYTE);

		// 单位Id
		nSize += sizeof(WORD);

		// 设备类型
		nSize += sizeof(BYTE);

		// 设备Id
		nSize += sizeof(WORD);

		// Ab机标识
		nSize += 1;

		return nSize;
	}

	int CDeviceInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CDeviceInfo len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// 路局Id
		pBuf[nOffset] = m_byBureauId;
		nOffset += 1;

		// 单位类型
		pBuf[nOffset] = m_byUnitType;
		nOffset += 1;

		// 单位Id
		SETSTREAMSHORT(pBuf + nOffset, m_wUnitId);
		nOffset += sizeof(WORD);

		// 设备类型
		pBuf[nOffset] = m_byDeviceType;
		nOffset += 1;

		// 设备Id
		SETSTREAMSHORT(pBuf + nOffset, m_wDeviceId);
		nOffset += sizeof(WORD);

		// Ab机标识
		pBuf[nOffset] = m_byAbFlag;
		nOffset += 1;

		return nOffset;
	}

	int CDeviceInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;
		// 路局Id
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfo BureauId len\n");
			return -1;
		}
		m_byBureauId = pBuf[nOffset];
		nOffset += 1;

		// 单位类型
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfo UnitType len\n");
			return -1;
		}
		m_byUnitType = pBuf[nOffset];
		nOffset += 1;

		// 单位Id
		if(nBufLen < (nOffset + sizeof(WORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfo UnitId len\n");
			return -1;
		}
		m_wUnitId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += sizeof(WORD);

		// 设备类型
		if(nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfo DeviceType len\n");
			return -1;
		}
		m_byDeviceType = pBuf[nOffset];
		nOffset += 1;

		// 设备Id
		if(nBufLen < (nOffset + sizeof(WORD)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfo DeviceId len\n");
			return -1;
		}
		m_wDeviceId = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += sizeof(WORD);

		// Ab机标识
		if(nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CDeviceInfo AbFlag len\n");
			return -1;
		}
		m_byAbFlag = pBuf[nOffset];
		nOffset += 1;

		return nOffset;
	}

	string CDeviceInfo::GetDumpStr()
	{
		char szBuf[256];
		memset(szBuf, 0, sizeof(szBuf));

#ifdef WIN32
		_snprintf_s(szBuf, sizeof(szBuf), "BureauId=%d, UnitType=%d, UnitId=%d, DevType=%d, DevId=%d, AbFlag=%d",
			m_byBureauId, m_byUnitType, m_wUnitId, m_byDeviceType, m_wDeviceId, m_byAbFlag);
#else
		snprintf(szBuf, sizeof(szBuf), "BureauId=%d, UnitType=%d, UnitId=%d, DevType=%d, DevId=%d, AbFlag=%d",
			m_byBureauId, m_byUnitType, m_wUnitId, m_byDeviceType, m_wDeviceId, m_byAbFlag);
#endif

		return szBuf;
	}

	//==============================================================================
	//
	//				CConnState
	//				客户端连接状态
	//
	//==============================================================================
	CConnState::CConnState()
		:m_byNet1Conn(0)
		,m_byNet2Conn(0)
		,m_byAF_UNIXState(0)
		,m_wNet1Port(0)
		,m_wNet2Port(0)
	{

	}

	CConnState::~CConnState()
	{

	}

	bool CConnState::operator==(const CConnState& objOther)
	{
		return(m_strIP1.compare(objOther.m_strIP1) == 0
			&& m_wNet1Port == objOther.m_wNet1Port
			&& m_byNet1Conn == objOther.m_byNet1Conn
			&& m_Net1ConnStaChgTime == objOther.m_Net1ConnStaChgTime
			&& m_strIP2.compare(objOther.m_strIP2) == 0
			&& m_wNet2Port == objOther.m_wNet2Port
			&& m_byNet2Conn == objOther.m_byNet2Conn
			&& m_Net2ConnStaChgTime == objOther.m_Net2ConnStaChgTime
			&& m_byAF_UNIXState == objOther.m_byAF_UNIXState
			&& m_AF_UNIXChgTime == objOther.m_AF_UNIXChgTime);
	}

	void CConnState::SetIP1(const string& strIP1)
	{
		m_strIP1 = strIP1;
	}

	string CConnState::GetIP1() const 
	{
		return m_strIP1;
	}

	void CConnState::SetNet1Port(WORD wNet1Port)
	{
		m_wNet1Port = wNet1Port;
	}

	WORD CConnState::GetNet1Port() const
	{
		return m_wNet1Port;
	}

	void CConnState::SetNet1Conn(BYTE byNet1Conn)
	{
		m_byNet1Conn = byNet1Conn;
	}

	BYTE CConnState::GetNet1Conn() const
	{
		return m_byNet1Conn;
	}

	void CConnState::SetNet1ConnStaChgTime(time_t& tm, WORD wMs)
	{
		m_Net1ConnStaChgTime.Set(tm, wMs);
	}

	void CConnState::SetNet1ConnStaChgTime(const CProtocolTime& objChgTime)
	{
		m_Net1ConnStaChgTime = objChgTime;
	}

	CProtocolTime CConnState::GetNet1ConnStaChgTime() const
	{
		return m_Net1ConnStaChgTime;
	}

	void CConnState::SetIP2(const string& strIP2)
	{
		m_strIP2 = strIP2;
	}

	string CConnState::GetIP2() const 
	{
		return m_strIP2;
	}

	void CConnState::SetNet2Port(WORD wNet2Port)
	{
		m_wNet2Port = wNet2Port;
	}

	WORD CConnState::GetNet2Port() const
	{
		return m_wNet2Port;
	}

	void CConnState::SetNet2Conn(BYTE byNet2Conn)
	{
		m_byNet2Conn = byNet2Conn;
	}

	BYTE CConnState::GetNet2Conn() const
	{
		return m_byNet2Conn;
	}

	void CConnState::SetNet2ConnStaChgTime(time_t& tm, WORD wMs)
	{
		m_Net2ConnStaChgTime.Set(tm, wMs);
	}

	void CConnState::SetNet2ConnStaChgTime(const CProtocolTime& objChgTime)
	{
		m_Net2ConnStaChgTime = objChgTime;
	}

	CProtocolTime CConnState::GetNet2ConnStaChgTime() const
	{
		return m_Net2ConnStaChgTime;
	}

	void CConnState::SetAF_UNIXState(BYTE byAF_UNIXState)
	{
		m_byAF_UNIXState = byAF_UNIXState;
	}

	BYTE CConnState::GetAF_UNIXState() const
	{
		return m_byAF_UNIXState;
	}

	void CConnState::SetAF_UNIXChgTime(time_t& tm, WORD wMs)
	{
		m_AF_UNIXChgTime.Set(tm, wMs);
	}

	void CConnState::SetAF_UNIXChgTime(const CProtocolTime& objChgTime)
	{
		m_AF_UNIXChgTime = objChgTime;
	}

	CProtocolTime CConnState::GetAF_UNIXChgTime() const
	{
		return m_AF_UNIXChgTime;
	}

	int CConnState::StreamSize()
	{
		int nSize = 0;
		// Ip1地址 + 网络1端口 + 网络1连接状态 + 网络1状态改变时间
		nSize += 1 + (int)m_strIP1.size() + 2 + 1
			+ m_Net1ConnStaChgTime.StreamSize();
		// Ip2地址 + 网络2端口 + 网络2连接状态 + 网络2状态改变时间
		nSize += 1 + (int)m_strIP2.size() + 2 + 1
			+ m_Net2ConnStaChgTime.StreamSize();
		//AF_UNIX连接状态+改变时间
		nSize += 1 + m_AF_UNIXChgTime.StreamSize();

		return nSize;
	}

	int CConnState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		// 判断buffer的大小
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CConnState len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;

		// Ip1地址
		BYTE byLen = (BYTE)MIN(MAX_BYTESIZE, m_strIP1.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strIP1.c_str(), byLen);
		nOffset += byLen;

		// 网络1端口
		SETSTREAMSHORT(pBuf + nOffset, m_wNet1Port);
		nOffset += 2;

		// 网络1连接状态
		pBuf[nOffset] = m_byNet1Conn;
		nOffset += 1;

		// 网络1状态改变时间
		int nBufLeft = nBufLen - nOffset;
		nOffset += m_Net1ConnStaChgTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		// Ip2地址
		byLen = (BYTE)MIN(MAX_BYTESIZE, m_strIP2.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strIP2.c_str(), byLen);
		nOffset += byLen;

		// 网络2端口
		SETSTREAMSHORT(pBuf + nOffset, m_wNet2Port);
		nOffset += 2;

		// 网络2连接状态
		pBuf[nOffset] = m_byNet2Conn;
		nOffset += 1;

		// 网络2状态改变时间
		nBufLeft = nBufLen - nOffset;
		nOffset += m_Net2ConnStaChgTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		//AF_UNIX连接状态
		pBuf[nOffset] = m_byAF_UNIXState;
		nOffset += 1;

		//AF_UNIX连接状态改变时间
		nBufLeft = nBufLen - nOffset;
		nOffset += m_AF_UNIXChgTime.ToStreamBuf(pBuf + nOffset, nBufLeft);

		return nOffset;
	}

	int CConnState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nRet, nOffset = 0;

		// Ip1地址
		if (nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState IP1 Address length len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState IP1 Address len\n");
			return -1;
		}
		char strTemp[MAX_BYTESIZE];
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strIP1 = strTemp;
		nOffset += byLen;

		// 网络1端口
		if (nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState Net1 Port len\n");
			return -1;
		}
		m_wNet1Port = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 网络1连接状态
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState Net1Conn len\n");
			return -1;
		}
		m_byNet1Conn = pBuf[nOffset];
		nOffset += 1;

		// 网络1状态改变时间
		nRet = m_Net1ConnStaChgTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// Ip2地址
		if (nBufLen < (nOffset + sizeof(BYTE)))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState IP2 Address length len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < (nOffset + byLen))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState IP2 Address len\n");
			return -1;
		}
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strIP2 = strTemp;
		nOffset += byLen;

		// 网络2端口
		if (nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState Net2 Port len\n");
			return -1;
		}
		m_wNet2Port = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 网络2连接状态
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState Net2Conn len\n");
			return -1;
		}
		m_byNet2Conn = pBuf[nOffset];
		nOffset += 1;

		// 网络2状态改变时间
		nRet = m_Net2ConnStaChgTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// AF_UNIX连接状态
		if (nBufLen < (nOffset + 1))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnState m_byAF_UNIXState len\n");
			return -1;
		}
		m_byAF_UNIXState = pBuf[nOffset];
		nOffset += 1;

		// AF_UNIX连接状态变时间
		nRet = m_AF_UNIXChgTime.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	//==============================================================================
	//
	//				CClientConnMainCommtoSys
	//				客户端连接【主系统通信平台→RmInterSys】
	//
	//==============================================================================
	CClientConnMainCommtoSys::CClientConnMainCommtoSys()
	{

	}

	CClientConnMainCommtoSys::~CClientConnMainCommtoSys()
	{

	}

	void CClientConnMainCommtoSys::SetForwardAddr(const CForwardAddr& objForwardAddr)
	{
		m_objForwardAddr = objForwardAddr;
	}

	CForwardAddr* CClientConnMainCommtoSys::GetForwardAddr()
	{
		return &m_objForwardAddr;
	}

	void CClientConnMainCommtoSys::SetConnState(const CConnState& objConnState)
	{
		m_objConnState = objConnState;
	}

	CConnState* CClientConnMainCommtoSys::GetConnState()
	{
		return &m_objConnState;
	}

	int CClientConnMainCommtoSys::StreamSize()
	{
		// 局码+单位类型+单位ID+设备类型+设备ID+客户端ID
		int nSize = m_objForwardAddr.Size();
		nSize += m_objConnState.StreamSize();
		return nSize;
	}

	int CClientConnMainCommtoSys::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		// 判断buffer的大小
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CClientConnMainCommtoSys len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = m_objForwardAddr.ToStreamBuf(pBuf);

		int nBufLeft = nBufLen-nOffset;
		nOffset += m_objConnState.ToStreamBuf(pBuf + nOffset, nBufLeft);

		return nOffset;
	}

	int CClientConnMainCommtoSys::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		if (nBufLen < CForwardAddr::Size())
		{
			LogImportant("FromStreamBuf: Buflen is less than the CClientConnMainCommtoSys forward address len\n");
			return -1;
		}

		int nOffset = m_objForwardAddr.FromStreamBuf(pBuf);

		int nRet = m_objConnState.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CClientConnMainCommtoSys connect state len\n");
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}

	//==============================================================================
	//
	//             CConnInfo
	//             服务进程连接地址信息类
	//
	//==============================================================================
	CConnInfo::CConnInfo()
		: m_byConnType(UNKNOW_CONN),
		m_wNet1Port(0), m_wNet2Port(0)
	{

	}

	CConnInfo::~CConnInfo()
	{

	}

	bool CConnInfo::operator!=(const CConnInfo& objOther)
	{
		if (m_byConnType != objOther.m_byConnType
			|| m_strIP1.compare(objOther.m_strIP1) != 0
			|| m_wNet1Port != objOther.m_wNet1Port
			|| m_strIP2.compare(objOther.m_strIP2) != 0
			|| m_wNet2Port != objOther.m_wNet2Port
			|| m_strConnPath.compare(objOther.m_strConnPath) != 0)
		{
			return true;
		}

		return false;
	}

	BYTE CConnInfo::GetConnType() const
	{
		return m_byConnType;
	}

	void CConnInfo::SetConnType(BYTE byType)
	{
		m_byConnType = byType;
	}

	string CConnInfo::GetIp1() const
	{
		return m_strIP1;
	}

	void CConnInfo::SetIp1(const string& strIp)
	{
		m_strIP1 = strIp;
	}

	WORD CConnInfo::GetNet1Port() const
	{
		return m_wNet1Port;
	}

	void CConnInfo::SetNet1Port(WORD wPort)
	{
		m_wNet1Port = wPort;
	}

	string CConnInfo::GetIp2() const
	{
		return m_strIP2;
	}

	void CConnInfo::SetIp2(const string& strIp)
	{
		m_strIP2 = strIp;
	}

	WORD CConnInfo::GetNet2Port() const
	{
		return m_wNet2Port;
	}

	void CConnInfo::SetNet2Port(WORD wPort)
	{
		m_wNet2Port = wPort;
	}

	string CConnInfo::GetConnPath() const
	{
		return m_strConnPath;
	}

	void CConnInfo::SetConnPath(const string& strPath)
	{
		m_strConnPath = strPath;
	}

	int CConnInfo::StreamSize()
	{
		// 连接类型
		int nSize = 1;

		// Ip1地址长度 + Ip1地址 + 网络1端口
		nSize += 1 + (int)m_strIP1.size() + 2;
		// Ip2地址长度 + Ip2地址 + 网络2端口
		nSize += 1 + (int)m_strIP2.size() + 2;
		// AF_UNIX连接路径长度 + AF_UNIX连接路径
		nSize += 1 + (int)m_strConnPath.size();

		return nSize;
	}

	int CConnInfo::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CConnInfo len");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;
		// 连接类型
		pBuf[nOffset] = m_byConnType;
		nOffset += 1;

		// Ip1地址长度 + Ip1地址 + 网络1端口
		BYTE byLen = (BYTE)MIN(MAX_BYTESIZE, m_strIP1.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strIP1.c_str(), byLen);
		nOffset += byLen;

		SETSTREAMSHORT(pBuf + nOffset, m_wNet1Port);
		nOffset += 2;

		// Ip2地址长度 + Ip2地址 + 网络2端口
		byLen = (BYTE)MIN(MAX_BYTESIZE, m_strIP2.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strIP2.c_str(), byLen);
		nOffset += byLen;

		SETSTREAMSHORT(pBuf + nOffset, m_wNet2Port);
		nOffset += 2;

		// AF_UNIX路径长度 + AF_UNIX路径
		byLen = (BYTE)MIN(MAX_BYTESIZE, m_strConnPath.size());
		pBuf[nOffset] = byLen;
		nOffset += 1;

		memcpy(pBuf + nOffset, m_strConnPath.c_str(), byLen);
		nOffset += byLen;

		return nOffset;
	}

	int CConnInfo::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		// 连接类型
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Conn Type len\n");
			return -1;
		}
		m_byConnType = pBuf[nOffset];
		nOffset += 1;

		// Ip1地址长度 + Ip1地址
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Ip1 address len len\n");
			return -1;
		}
		BYTE byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Ip1 address len\n");
			return -1;
		}
		char strTemp[MAX_BYTESIZE + 1];
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strIP1 = strTemp;
		nOffset += byLen;

		// 网络1端口
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Net1 Port len\n");
			return -1;
		}
		m_wNet1Port = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// Ip2地址长度 + Ip2地址
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Ip2 address len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Ip2 address len\n");
			return -1;
		}
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strIP2 = strTemp;
		nOffset += byLen;

		// 网络2端口
		if (nBufLen < nOffset + 2)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo Net2 Port len\n");
			return -1;
		}
		m_wNet2Port = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// AF_UNIX路径长度 + AF_UNIX路径
		if (nBufLen < nOffset + 1)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo AF_UNIX Path len len\n");
			return -1;
		}
		byLen = pBuf[nOffset];
		nOffset += 1;

		if (nBufLen < nOffset + byLen)
		{
			LogImportant("FromStreamBuf: Buflen is less than the CConnInfo AF_UNIX Path len\n");
			return -1;
		}
		memcpy(strTemp, pBuf + nOffset, byLen);
		strTemp[byLen] = 0;
		m_strConnPath = strTemp;
		nOffset += byLen;

		return nOffset;
	}

	std::string CConnInfo::GetDumpStr()
	{
		char szBuf[4000];
		memset(szBuf, 0, sizeof(szBuf));

#ifdef WIN32
		_snprintf_s(szBuf, sizeof(szBuf), "ConnType=%d, IP1=%s, Port1=%d, IP2=%s, Port2=%d, AF_UNIXPath=%s",
			m_byConnType, m_strIP1.c_str(), m_wNet1Port, m_strIP2.c_str(), m_wNet2Port, m_strConnPath.c_str());
#else
		snprintf(szBuf, sizeof(szBuf), "ConnType=%d, IP1=%s, Port1=%d, IP2=%s, Port2=%d, AF_UNIXPath=%s",
			m_byConnType, m_strIP1.c_str(), m_wNet1Port, m_strIP2.c_str(), m_wNet2Port, m_strConnPath.c_str());
#endif

		return szBuf;
	}

	//==============================================================================
	//
	//             CProgramServer
	//             服务程序信息类
	//
	//==============================================================================
	CProgramServer::CProgramServer()
		: m_wSvcID(0)
	{

	}

	CProgramServer::~CProgramServer()
	{

	}

	bool CProgramServer::operator==(const CProgramServer& objOther)
	{
		return (m_wSvcID == objOther.m_wSvcID);
	}

	WORD CProgramServer::GetSvcID() const
	{
		return m_wSvcID;
	}

	void CProgramServer::SetSvcID(WORD wId)
	{
		m_wSvcID = wId;
	}

	int CProgramServer::StreamSize()
	{
		return 2;
	}

	int CProgramServer::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CProgramServer len");
			nBufLen = StreamSize();
			return -1;
		}

		int nOffset = 0;
		SETSTREAMSHORT(pBuf, m_wSvcID);
		nOffset += 2;

		return nOffset;
	}

	int CProgramServer::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		int nOffset = 0;

		// SvcId
		if(nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CProgramServer SvcId len\n");
			return -1;
		}
		m_wSvcID = GETSTREAMSHORT(pBuf);
		nOffset += 2;

		return nOffset;
	}

	//==============================================================================
	//
	//				CSvrConnState
	//				服务进程的连接状态
	//
	//==============================================================================
	CSvrConnState::CSvrConnState()
	{

	}

	CSvrConnState::~CSvrConnState()
	{

	}

	void CSvrConnState::SetProgramServer(CProgramServer& objSvc)
	{
		m_objSvc = objSvc;
	}

	CProgramServer* CSvrConnState::GetProgramServer()
	{
		return &m_objSvc;
	}

	void CSvrConnState::SetConnInfo(CConnInfo& objConnInfo)
	{
		m_objConnInfo = objConnInfo;
	}

	CConnInfo* CSvrConnState::GetConnInfo()
	{
		return &m_objConnInfo;
	}

	void CSvrConnState::Clear()
	{
		m_vecClientConnMainCommtoSys.clear();
	}

	void CSvrConnState::AddClient(const CClientConnMainCommtoSys& objClient)
	{
		m_vecClientConnMainCommtoSys.push_back(objClient);
	}

	WORD CSvrConnState::GetClientCount() const
	{
		return (WORD)m_vecClientConnMainCommtoSys.size();
	}

	CClientConnMainCommtoSys* CSvrConnState::GetClient(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_vecClientConnMainCommtoSys.size())
		{
			return &(m_vecClientConnMainCommtoSys.at(nIndex));
		}
		
		return NULL;
	}

	int CSvrConnState::StreamSize()
	{
		int nSize = 0;

		// 服务端ID信息
		nSize += m_objSvc.StreamSize();
		// 服务端网络地址信息
		nSize += m_objConnInfo.StreamSize();
		// 客户端数量
		nSize += 2;

		// 客户端信息
		WORD wLen = (WORD)MIN(MAX_WORDSIZE, m_vecClientConnMainCommtoSys.size());
		for (int i = 0; i < wLen; i++)
		{
			nSize += m_vecClientConnMainCommtoSys.at(i).StreamSize();
		}

		return nSize;
	}

	int CSvrConnState::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		// 判断buffer的大小
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CSvrConnState len\n");
			nBufLen = StreamSize();
			return -1;
		}

		int nBufLeft = nBufLen;

		// 服务端ID信息
		int nOffset = m_objSvc.ToStreamBuf(pBuf, nBufLeft);

		// 服务端网络地址信息
		nBufLeft = nBufLen - nOffset;
		nOffset += m_objConnInfo.ToStreamBuf(pBuf + nOffset, nBufLeft);
		
		// 客户端数量
		WORD wLen = (WORD)MIN(MAX_WORDSIZE, m_vecClientConnMainCommtoSys.size());
		SETSTREAMSHORT(pBuf + nOffset, wLen);
		nOffset += 2;

		// 客户端信息
		for (int i = 0; i < wLen; i++)
		{
			nBufLeft = nBufLen - nOffset;
			nOffset += m_vecClientConnMainCommtoSys.at(i).ToStreamBuf(pBuf + nOffset, nBufLeft);
		}

		return nOffset;
	}

	int CSvrConnState::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		m_vecClientConnMainCommtoSys.clear();
		int nOffset = 0;

		// 服务端ID信息
		int nRet = m_objSvc.FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;
		
		// 服务端网络地址信息
		nRet = m_objConnInfo.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 客户端数量
		if (nBufLen < (nOffset + 2))
		{
			LogImportant("FromStreamBuf: Buflen is less than the CSvrConnState Client Count len\n");
			return -1;
		}
		WORD wLen = GETSTREAMSHORT(pBuf + nOffset);
		nOffset += 2;

		// 客户端信息
		for (int i = 0; i < wLen; i++)
		{
			CClientConnMainCommtoSys objClient;
			nRet = objClient.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
			if (nRet == -1)
			{
				return -1;
			}
			m_vecClientConnMainCommtoSys.push_back(objClient);
			nOffset += nRet;
		}

		return nOffset;
	}

	vector<CClientConnMainCommtoSys>& CSvrConnState::GetClientVector()
	{
		return m_vecClientConnMainCommtoSys;
	}

	//==============================================================================
	//
	//             CMainSysSvcConnStatusPkt
	//          主系统服务连接状态【主系统通信平台→RmInterSys】
	//
	//==============================================================================
	CMainSysSvcConnStatusPkt::CMainSysSvcConnStatusPkt()
		: CProtocolHead(TYPE_MAINTEN, FUNC_SYSSVRCONNSTATUS_SYSTOINTER)
	{

	}

	CMainSysSvcConnStatusPkt::~CMainSysSvcConnStatusPkt()
	{

	}

	void CMainSysSvcConnStatusPkt::SetDeviceInfo(const CDeviceInfo& objDeviceInfo)
	{
		m_objDeviceInfo = objDeviceInfo;
	}

	CDeviceInfo* CMainSysSvcConnStatusPkt::GetDeviceInfo()
	{
		return &m_objDeviceInfo;
	}

	void CMainSysSvcConnStatusPkt::SetSvrConnState(const CSvrConnState& objSvrConnState)
	{
		m_objSvrConnSt = objSvrConnState;
	}

	CSvrConnState* CMainSysSvcConnStatusPkt::GetSvrConnState()
	{
		return &m_objSvrConnSt;
	}

	CProtocolHead* CMainSysSvcConnStatusPkt::Clone()
	{
		CMainSysSvcConnStatusPkt* pClone = new CMainSysSvcConnStatusPkt();
		*pClone = *this;
		return pClone;
	}

	int CMainSysSvcConnStatusPkt::StreamSize()
	{
		// 报文头
		int nSize = CProtocolHead::StreamSize();

		// 服务器设备信息：局码+单位类型+单位ID+设备类型+设备ID+AB机标识
		nSize += m_objDeviceInfo.StreamSize();

		// 服务连接状态
		nSize += m_objSvrConnSt.StreamSize();

		return nSize;
	}

	int CMainSysSvcConnStatusPkt::ToStreamBuf(char* pBuf, int& nBufLen)
	{
		// 判断buffer的大小
		if (nBufLen < StreamSize())
		{
			LogImportant("ToStreamBuf: Buflen is less than the CMainSysSvcConnStatusPkt len\n");
			nBufLen = StreamSize();
			return -1;
		}

		// CProtocolHead
		int nBufLeft = nBufLen;
		int nOffset = CProtocolHead::ToStreamBuf(pBuf, nBufLeft);

		// 服务器设备信息
		nBufLeft = nBufLen - nOffset;
		nOffset += m_objDeviceInfo.ToStreamBuf(pBuf + nOffset, nBufLeft);

		// 服务连接状态
		nBufLeft = nBufLen - nOffset;
		nOffset += m_objSvrConnSt.ToStreamBuf(pBuf + nOffset, nBufLeft);

		return nOffset;
	}

	int CMainSysSvcConnStatusPkt::FromStreamBuf(const char* pBuf, int nBufLen)
	{
		// CProtocolHead
		int nRet = CProtocolHead::FromStreamBuf(pBuf, nBufLen);
		if (nRet == -1)
		{
			return -1;
		}
		int nOffset = nRet;

		// 服务器设备信息
		nRet = m_objDeviceInfo.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		// 服务连接状态
		nRet = m_objSvrConnSt.FromStreamBuf(pBuf + nOffset, nBufLen - nOffset);
		if (nRet == -1)
		{
			return -1;
		}
		nOffset += nRet;

		return nOffset;
	}
}
