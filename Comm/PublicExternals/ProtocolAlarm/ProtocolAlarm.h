//================================================================================================
//	
//									ProtocolAlarm.h
//							   
//begin	  : 2016-09-08
//describe: 协议报警事件
//
//================================================================================================
#ifndef _PROTOCOL_ALARM_H_
#define _PROTOCOL_ALARM_H_

namespace ProtocolAlarm
{
	//============================================================================================
	//	
	//											类型码定义
	//
	//============================================================================================
	#define TYPE_ALARM			0X18	// 报警事件信息

	//============================================================================================
	//	
	//											功能码定义
	//
	//============================================================================================
	/*****************************************报警事件功能码**************************************/
	#define FUNC_ALARMGENERATE	0X01	// 报警事件生成【其他模块->AppAlarmMgr】
	#define FUNC_ALARMALL		0X02	// 报警事件全体信息【AppAlarmMgr->操作台】
	#define	FUNC_ALARMCHANGE	0X03	// 报警事件变化信息【AppAlarmMgr->操作台】
	#define FUNC_ALARMREQ		0X04	// 报警事件请求信息【操作台->AppAlarmMgr】
	#define FUNC_ALARMCOMMIT	0X05	// 报警事件确认【操作台->AppAlarmMgr】

	#define FUNC_ALARMSYNREQ	0x06	// 报警事件请求同步信息【备AppAlarmMgr->主AppAlarmMgr】
	#define FUNC_ALARMSYNALL	0x07	// 报警事件全体同步信息【主AppAlarmMgr->备AppAlarmMgr】
	#define FUNC_ALARMSYNCHG	0x08	// 报警事件变化同步信息【主AppAlarmMgr->备AppAlarmMgr】

	#define FUNC_ALARMSAVE		0X10	// 报警事件存库【AppAlarmMgr->OsmAlarm】
	#define FUNC_ALARMUPDATE	0X11	// 报警事件更新存库【AppAlarmMgr->OsmAlarm】
	#define FUNC_ALARMSAVERET	0X12	// 报警事件存库回执【OsmAlarm->AppAlarmMgr】
	#define FUNC_ALARMQUERY		0X20	// 报警事件查询【OsmAlarm->查询模块】

	/******************************************报警相关定义***************************************/
	// 报警确认标识
	#define ALARMCOMMIT_NO		0X00	// 报警确认标识，是否已确认：否
	#define ALARMCOMMIT_YES		0X01	// 报警确认标识，是否已确认：是

	//报警解除标识字段的含义：【AppAlarmMgr->操作台】时，告诉终端报警是否已经解除
	#define ALARMRELIEVE_NO		0X00	// 报警解除标识，是否已解除：否
	#define ALARMRELIEVE_YES	0X01	// 报警解除标识，是否已解除：是	
	//报警解除标识字段的含义：【AppAlarmMgr<->报警存取模块】时，区分报警是产生还是恢复
	#define ALARM_PRODUCT		0X00	// 报警产生
	#define ALARM_RELIEVE		0X01	// 报警解除

	// 报警中车站ID字段无效值
	#define ALARM_STA_ID_NONE	0xFFFF

	/*******************报警请求信息【操作台->AppAlarmMgr】*****************************************/
	#define ALARMREQ_ALLDISPATCH	0XFFFF	// 查询所有调度台的报警信息

	/*******************报警回执信息相关************************************************************/
	#define ALARM_RET_RESULT_RIGHT	0X0000	// 报警回执的正确码
}

#endif	//_PROTOCOL_ALARM_H