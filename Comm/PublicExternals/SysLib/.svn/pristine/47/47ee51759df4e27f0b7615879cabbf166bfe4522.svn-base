//==============================================================================
//                      LocalDeviceConf.h
//                       
//begin   : 2016-08-22                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_LOCALDEVICECONF_H 
#define SELFDEF_LOCALDEVICECONF_H 

#define ADDR_UNITTYPE_CENTER			1
#define ADDR_UNITTYPE_STATION			2
#define ADDR_DEVTYPE_SVR				1
#define ADDR_DEVTYPE_DDT				2
#define ADDR_DEVTYPE_TERM				3

#include "TypeDef.h"

//==============================================================================
//                 
//                    CLocalDeviceConf
//                LocalDevice.xml的配置读取类
//
//==============================================================================
class MY_EXT_CLASS CLocalDeviceConf
{
public:
	CLocalDeviceConf();
	virtual ~CLocalDeviceConf();
	
public:
	bool Load(char* szErrInfo = "", int nErrLen = 0);
	
public:
	BYTE m_byBureauId;	// 局码
	BYTE m_byUnitType;	// 单位类型
	WORD m_wUnitId;		// 单位ID
	BYTE m_byDevType;	// 设备类型
	WORD m_wDevId;		// 设备ID
	BYTE m_byAbType;	// A, B机类型，A=1，B=2
};

  
#endif 
