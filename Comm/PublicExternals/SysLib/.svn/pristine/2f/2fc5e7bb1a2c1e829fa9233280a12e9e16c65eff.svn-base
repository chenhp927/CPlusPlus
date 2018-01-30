//==============================================================================
//	
//							 PubFuncSystem.h
//							   
//begin	  : 2016/09/17
//describe: 系统公共函数定义
//
//==============================================================================

#ifndef _PUB_FUNC_SYSTEM_INCLUDE_
#define _PUB_FUNC_SYSTEM_INCLUDE_

#include "PubDefSystem.h"
#include "TypeDef.h"

#include <string>
using namespace std;

//==============================================================================
//
//                   CPubFuncSystem
//                 系统公共函数定义类
//
//==============================================================================

class MY_EXT_CLASS CPubFuncSystem
{
public:
	CPubFuncSystem();
	virtual ~CPubFuncSystem();

public:
	// 将车站类型ID转换为车站类型名称
	static string ConStaTypeIdToName(ENUM_STA_TYPE eStaTypeId);
	// 将车站类型名称转换为车站类型ID
	static ENUM_STA_TYPE ConStaTypeNameToId(string strStaTypeName);

	// 将车站判事故方式ID转换为判事故方式名称
	static string ConAccArbIdToName(ENUM_ACCIDENT_SENTENCE eAccArbId);
	// 将车站判事故方式名称转换为车站判事故方式ID
	static ENUM_ACCIDENT_SENTENCE ConAccArbNameToId(string strAccArbName);

	// 将定值类型ID转换为定值类型名称
	static string ConFixTypeIdToName(ENUM_FIXVALUE_TYPE eFixTypeId);
	// 将定值类型名称转换为定值类型ID
	static ENUM_FIXVALUE_TYPE ConFixTypeNameToId(string strFixTypeName);

	// 将变比类型ID转换为变比类型名称
	static string ConChgRadioTypeIdToName(ENUM_TROUBLEPOW_TYPE eChgRadioTypeId);
	// 将变比类型名称转换为变比类型ID
	static ENUM_TROUBLEPOW_TYPE ConChgRadioTypeNameToId(string strChgRadioTypeName);

	// 将遥信点类型ID转换为遥信点类型名称
	static string ConSignalTypeIdToName(ENUM_CTRLSIGNAL_TYPE eSignalTypeId);
	// 将遥信点类型名称转换为遥信点类型ID
	static ENUM_CTRLSIGNAL_TYPE ConSignalTypeNameToId(string strSignalType);

	// 将遥控点类型ID转换为遥控点类型名称
	static string ConCtrlTypeIdToName(ENUM_CTRLSIGNAL_TYPE eCtrlTypeId);
	// 将遥控点类型名称转换为遥控点类型ID
	static ENUM_CTRLSIGNAL_TYPE ConCtrlTypeNameToId(string strCtrlType);

	// 将报警事件类型ID转换为报警事件类型名称
	static string ConAlmTypeIdToName(ENUM_ALARM_TYPE eAlmTypeId);
	// 将报警事件类型名称转换为报价事件类型ID
	static ENUM_ALARM_TYPE ConAlmTypeNameToId(string strAlmType);
	// 报警事件类型分报警、事件，是否事件。是事件：返回true，是报警：返回false
	static bool IsAlarmEvtTypeEvt(ENUM_ALARM_TYPE eType);

	// 将遥测点类型ID转换为遥测点类型名称
	static string ConMeasureTypeIdToName(ENUM_MEASURE_TYPE eMeasureTypeId);
	// 将遥测点类型名称转换为遥测点类型ID
	static ENUM_MEASURE_TYPE ConMeasureTypeNameToId(string strMeasureType);

	// 将电压等级ID转换为电压等级名称
	static string ConVGradeIdToName(ENUM_VOL_RATE eVCLevelId);
	// 将电压等级名称转换为电压等级ID
	static ENUM_VOL_RATE ConVGradeNameToId(string strVCLevelType);

	// 将历史存盘方式ID转换为历史存盘方式名称
	static string ConDiskTypeIdToName(ENUM_HISSAVE_TYPE eDiskTypeId);
	// 将历史存盘方式名称转换为历史存盘方式ID
	static ENUM_HISSAVE_TYPE ConDiskTypeNameToId(string strDiskType);
	// 将历史存盘方式ID转换为历史存盘时间，单位：秒
	static DWORD ConDiskTypeIdToTime(ENUM_HISSAVE_TYPE eDiskType);

	// 将故障报告类型ID转换为故障报告类型名称
	static string ConReportTypeIdToName(ENUM_FAULTREPORT_TYPE eReportTypeId);
	// 将故障报告类型名称转换为故障报告类型ID
	static ENUM_FAULTREPORT_TYPE ConReportTypeNameToId(string strReportType);

	// 将车站类型转换为供电类型类型ID
	static ENUM_POWERSUPPLY_TYPE ConStaTypeIdToPowerSupplyTypeId(ENUM_STA_TYPE eStaTypeId);
	// 将供电类型类型ID转换为供电类型名称
	static string ConPowerSupplyTypeIdToName(ENUM_POWERSUPPLY_TYPE ePowerSupplyTypeId);

	// 将操控设备类型ID转换为类型名称
	static string ConOperDevTypeIdToName(ENUM_OPERDEV_TYPE eOperDevTypeId);

	// 将故障测距节点类型ID转换为故障测距节点类型名称
	static string ConFaultDistancePotTypeIdToName(ENUM_FAULTDISTANCE_POTTYPE eFaultDistancePotTypeId);
	static ENUM_FAULTDISTANCE_POTTYPE ConFaultDistancePotTypeNameToId(string strFaultDistancePotType);

	// 将故障测距方向ID转换为故障测距方向名称
	static string ConFaultDistanceDirectionIdToName(ENUM_FAULTDISTANCE_DIRECTION eFaultDistanceDirectionId);
	static ENUM_FAULTDISTANCE_DIRECTION ConFaultDistanceDirectionNameToId(string strFaultDistanceDirection);

	// 将故障电量数据类型ID转换为故障电量数据类型名称
	static string ConDataTypeIdToName(ENUM_FAULTDATA_TYPE eDataTypeId);
	// 将故障电量数据类型名称转换为故障电量数据类型ID
	static ENUM_FAULTDATA_TYPE ConDataTypeNameToId(string strDataType);

	// 将遥测参数值类型ID转换为遥测参数值类型名称
	static string ConMeasureConfigTypeIdToName(ENUM_MEASURECONFIG_TYPE eMeasureConfigType);
	// 将遥测参数值类型名称转换为遥测参数值类型ID
	static ENUM_MEASURECONFIG_TYPE ConMeasureConfigTypeNameToId(string strMeasureConfigType);
};




#endif		// _PUB_FUNC_SYSTEM_INCLUDE_