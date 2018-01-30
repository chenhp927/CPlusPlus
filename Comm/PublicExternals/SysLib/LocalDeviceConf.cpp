//==============================================================================
//                      LocalDeviceConf.cpp
//                       
//begin   : 2016-08-22                                                 
//describe:                                              
//==============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "LocalDeviceConf.h" 
#include "tinyxml.h"
#include "Utility.h"
#include "AppPath.h"

#define CONFFILE_LOCALDEVICE "LocalDevice.xml"

//==============================================================================
//                 
//                    CLocalDeviceConf
//                    localdevice.xmlµƒ≈‰÷√∂¡»°¿‡
//
//==============================================================================
CLocalDeviceConf::CLocalDeviceConf()
{
	m_byBureauId = 0;
	m_byUnitType = 0;
	m_wUnitId = 0;
	m_byDevType = 0;
	m_wDevId = 0;
	m_byAbType = 0;
}

CLocalDeviceConf::~CLocalDeviceConf()
{
}

bool CLocalDeviceConf::Load(char* szErrInfo, int nErrLen)
{
	assert(szErrInfo);

	char szFile[FILE_NAME_LEN];

	string strPath = CAppPath::GetPublicConfPath();
	if (strPath.size() <= 0)
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), can't find PublicConf path!");
		return false;
	}
	else
	{
		snprintf(szFile, sizeof(szFile), "%s%s", strPath.c_str(), CONFFILE_LOCALDEVICE);
	}

	FILE* pFile = fopen(szFile, "r");
	if (!pFile)
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), can not find file=%s!", szFile);
		return false;
	}
	fclose(pFile);

	TiXmlDocument txdDoc(szFile);
	if (!txdDoc.LoadFile())
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to load file=%s!", szFile);
		return false;
	}

	TiXmlHandle txhRoot = txdDoc.RootElement();
	TiXmlElement* pTxeElem = NULL;

	// <BureauId>
	pTxeElem = txhRoot.FirstChild("BureauId").Element();
	if (pTxeElem) 
	{
		int nVal = CUtility::ATOI(pTxeElem->GetText()); 
		if (nVal < 1 || nVal > 254) 
		{ 
			snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), <BureauId>%d</BureauId> "
				"is out of range(1-254)!", nVal);
			return false; 
		} 
		else
		{
			m_byBureauId = (BYTE)nVal;
		}
	}
	else
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to read <BureauId>!");
		return false;
	}
	
	// <UnitType>
	pTxeElem = txhRoot.FirstChild("UnitType").Element();
	if (pTxeElem && pTxeElem->GetText())
	{
		int nVal = CUtility::ATOI(pTxeElem->GetText()); 
		if (nVal != ADDR_UNITTYPE_CENTER && nVal != ADDR_UNITTYPE_STATION) 
		{ 
			snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), <UnitType>%d</UnitType> "
				"is out of range(%d,%d)!", nVal, ADDR_UNITTYPE_CENTER, ADDR_UNITTYPE_STATION);
			return false; 
		} 
		else
		{
			m_byUnitType = (BYTE)nVal;
		}
	}
	else
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to read <UnitType>!");
		return false;
	}
	
	// <UnitId>
	pTxeElem = txhRoot.FirstChild("UnitId").Element();
	if (pTxeElem && pTxeElem->GetText())
	{
		int nVal = CUtility::ATOI(pTxeElem->GetText()); 
		if (nVal < 1 || nVal > 65534) 
		{ 
			snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), <UnitId>%d</UnitId> "
				"is out of range(1-65534)!", nVal);
			return false; 
		} 
		else
		{
			m_wUnitId = (WORD)nVal;
		}
	}
	else
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to read <UnitId>!");
		return false;
	}

	// <DeviceType>
	pTxeElem = txhRoot.FirstChild("DeviceType").Element();
	if (pTxeElem && pTxeElem->GetText())
	{
		int nVal = CUtility::ATOI(pTxeElem->GetText()); 
		if (nVal != ADDR_DEVTYPE_SVR && 
			nVal != ADDR_DEVTYPE_DDT && 
			nVal != ADDR_DEVTYPE_TERM) 
		{ 
			snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), <DeviceType>%d</DeviceType> "
				"is out of range(%d,%d,%d)!", nVal, ADDR_DEVTYPE_SVR, ADDR_DEVTYPE_DDT, ADDR_DEVTYPE_TERM);
			return false; 
		} 
		else
		{
			m_byDevType = (BYTE)nVal;
		}
	}
	else
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to read <DeviceType>!");
		return false;
	}

	// <DeviceId>
	pTxeElem = txhRoot.FirstChild("DeviceId").Element();
	if (pTxeElem && pTxeElem->GetText())
	{
		int nVal = CUtility::ATOI(pTxeElem->GetText()); 
		if (nVal < 1 || nVal > 65534) 
		{ 
			snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), <DeviceId>%d</DeviceId> "
				"is out of range(1-65534)!", nVal);
			return false; 
		} 
		else
		{
			m_wDevId = (WORD)nVal;
		}
	}
	else
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to read <DeviceId>!");
		return false;
	}
	
	// <Ab>
	pTxeElem = txhRoot.FirstChild("Ab").Element();
	if (pTxeElem && pTxeElem->GetText())
	{
		if (strcmp(pTxeElem->GetText(), "a") == 0)
		{
			m_byAbType = 'a';
		}
		else if (strcmp(pTxeElem->GetText(), "b") == 0) 
		{ 
			m_byAbType = 'b';
		} 
		else
		{
			snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), <Ab>%s</Ab> "
				"is out of range(a, b)!", pTxeElem->GetText());
			return false; 
		}
	}
	else
	{
		snprintf(szErrInfo, nErrLen, "CLocalDeviceConf::Load(), fail to read <Ab>! use default=a");
		m_byAbType = 'a';
	}
	
	return true;
}
