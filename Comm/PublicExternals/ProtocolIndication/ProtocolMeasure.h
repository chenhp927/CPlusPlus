//=========================================================================================
//	
//							 ProtocolMeasure.h
//							   
// begin    : 2016-08-26
// describe : 协议遥测类
//
//=========================================================================================
#ifndef _PROTOCOL_MEASURE_H
#define _PROTOCOL_MEASURE_H

#include "ProtocolTime.h"
#include "TypeDef.h"
#include "PubDefSystem.h"
#include "ProtocolHead.h"
#include <vector>
#include "ProtocolIndication.h"

namespace ProtocolIndication
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//						全体遥测信息【InterSvr104→AppMeasureMgr】
	//
	//==============================================================================

	//==============================================================================
	//	
	//									CMeasureCollAll
	//						全体遥测信息中遥测点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollAll : public CObjectBase
	{
	public:
		CMeasureCollAll();
		CMeasureCollAll(BYTE byFlag, float fValue);
	    virtual ~CMeasureCollAll();

	public:
		void SetFlag(BYTE byFlag);									// 设置遥测值有效性标识
		BYTE GetFlag() const;										// 获取遥测值有效性标识
		void SetValue(float fValue);								// 设置遥测值
		float GetValue() const;										// 获取遥测值

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		BYTE m_byFlag;												// 遥测值有效性标识
		float m_fValue;												// 遥测值，4字节浮点数
	};

	//==============================================================================
	//	
	//									 CMeasureStaCollAll
	//						全体遥测信息中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureStaCollAll : public CObjectBase
	{
	public:
		CMeasureStaCollAll();
		CMeasureStaCollAll(WORD wStaId);
	    virtual ~CMeasureStaCollAll();

	public:
		void SetId(WORD wStaId);
		WORD GetId() const;

		void SetMeaStartAddr(DWORD dwStart);
		DWORD GetMeaStartAddr() const;

		// 单个遥测读取/存储
		WORD GetMeasureCount() const;
		CMeasureCollAll* GetMeasure(int nIndex);
		void Add(CMeasureCollAll &measure);

		// 整个车站实际遥测读取/存储
		WORD GetRealMeaCnt() const;
		string GetAllMeas();
		void SetAllMeas(string strData);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// 整个车站实际遥测组包/解包
		virtual int StreamSize2();
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;									// 车站Id
		DWORD m_dwMeaStartAddr;							// 遥测起始地址
		vector<CMeasureCollAll> m_vecMeasure;			// 遥测信息

		// 车站内实际遥测数量
		WORD m_wRealMeaCnt;
		// 车站内遥测值集合
		string m_strData;
	};

	//==============================================================================
	//	
	//								  CMeasureCollAllPkt
	//						全体遥测信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollAllPkt : public CProtocolHead
	{
	public:
		CMeasureCollAllPkt();
	    virtual ~CMeasureCollAllPkt();

	public:
		WORD GetStaCount() const;									// 获取车站数量
		CMeasureStaCollAll* GetSta(int nIndex);						// 获取指定下标车站信息
		void Add(CMeasureStaCollAll &sta);							// 添加车站信息
		void Clear();												// 清除所有车站信息

		virtual CProtocolHead* Clone();  
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// 整站遥测拷贝组/解包方式
		virtual int StreamSize2();
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen); 

	private:
		vector<CMeasureStaCollAll> m_vecSta;						// 车站信息
	};

	//==============================================================================
	//
	//						变化遥测信息【InterSvr104→AppMeasureMgr】
	//
	//==============================================================================

	//==============================================================================
	//	
	//								 CMeasureCollChange
	//					变化遥测信息中遥测点【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollChange : public CObjectBase
	{
	public:
		CMeasureCollChange();
	    virtual ~CMeasureCollChange();

	public:
		void SetAddr(DWORD dwAddr);									// 设置遥测地址
		DWORD GetAddr() const;										// 获取遥测地址
		void SetFlag(BYTE byFlag);									// 设置遥测值有效性标识
		BYTE GetFlag() const;										// 获取遥测值有效性标识
		void SetValue(float fValue);								// 设置遥测值
		float GetValue() const;										// 获取遥测值

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwAddr;												// 遥测地址
		BYTE m_byFlag;												// 遥测值有效性标识
		float m_fValue;												// 遥测值，4字节浮点数
	};

	//==============================================================================
	//	
	//								CMeasureStaCollChange
	//						变化遥测信息中车站【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureStaCollChange : public CObjectBase
	{
	public:
		CMeasureStaCollChange();
		CMeasureStaCollChange(WORD wStaId);
	    virtual ~CMeasureStaCollChange();

	public:
		void SetId(WORD wStaId);									// 设置车站Id
		WORD GetId() const;											// 获取车站Id
		
		WORD GetMeasureCount() const;								// 获取遥测数量
		CMeasureCollChange* GetMeasure(int nIndex);					// 获取指定下标的遥测信息
		void Add(CMeasureCollChange &measure);						// 添加遥测信息

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;												// 车站Id
		vector<CMeasureCollChange> m_vecMeasure;					// 遥测信息
	};

	//==============================================================================
	//	
	//									CMeasureCollChangePkt
	//							变化遥测信息包【InterSvr104→AppSignalMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollChangePkt : public CProtocolHead
	{
	public:
		CMeasureCollChangePkt();
	    virtual ~CMeasureCollChangePkt();

	public:
		WORD GetStaCount() const;								// 获取车站数量
		CMeasureStaCollChange* GetSta(int nIndex);				// 获取指定下标车站信息
		void Add(CMeasureStaCollChange &sta);					// 添加车站信息

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CMeasureStaCollChange> m_vecSta;					// 车站信息
	};

	//==============================================================================
	//
	//						请求遥测信息【AppMeasureMgr→InterSvr104】
	//
	//==============================================================================
	//==============================================================================
	//
	//									CMeasureCollReqPkt
	//						请求遥测信息包【AppMeasureMgr→InterSvr104】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollReqPkt : public CProtocolHead
	{
	public:
		CMeasureCollReqPkt();
		virtual ~CMeasureCollReqPkt();

	public:
		WORD GetStaCount() const;				// 获取车站数量
		WORD GetSta(int nIndex) const;			// 获取指定下标车站信息
		void Add(WORD sta);						// 添加车站信息

		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<WORD> m_vecSta;			// 车站信息
	};

	//==============================================================================
	//
	//									CMeasureAll
	//						全体遥测信息中遥测点【AppMeasureMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureAll : public CObjectBase
	{
	public:
		CMeasureAll();
	    virtual ~CMeasureAll();

	public:
		void SetId(DWORD dwId);										// 设置遥测点Id
		void SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag);				// 设置质量标志
		void SetValue(float fValue);								// 设置遥测值

		DWORD GetId() const;										// 获取遥测点Id
		ENUM_QUALITY_FLAG_TYPE GetQltFalg() const;					// 获取质量标志
		float GetValue() const;										// 获取遥测值

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwId;												// 遥测点Id
		BYTE m_byQltFlag;											// 质量标志
		float m_fValue;												// 遥测值
	};
	
	//==============================================================================
	//
	//									CMeasureAllSta
	//						全体遥测信息中车站【AppMeasureMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureAllSta : public CObjectBase
	{
	public:
		CMeasureAllSta();
	    virtual ~CMeasureAllSta();
	
	public:
		void SetId(WORD wSId);										// 设置车站Id
		void Add(CMeasureAll Measure);								// 添加遥测

		WORD GetId() const;											// 获取车站Id
		CMeasureAll* GetMeasure(int nIndex);						// 获取指定下标遥测
		WORD GetMeasureCount() const;								// 获取遥测数量

		virtual void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// 车站Id
		vector<CMeasureAll> m_vecMeasure;							// 遥测
	};

	//==============================================================================
	//
	//									CMeasureAllPkt
	//						全体遥测信息包【AppMeasureMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureAllPkt : public CProtocolHead
	{
	public:
		CMeasureAllPkt();
	    virtual ~CMeasureAllPkt();

	public:
		void Add(CMeasureAllSta Sta);								// 添加车站
		CMeasureAllSta* GetSta(int nIndex);							// 获取指定下标车站
		WORD GetStaCount() const;									// 获取车站数量

		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CMeasureAllSta> m_vecSta;							// 车站
	};

	//==============================================================================
	//
	//								CMeasureChangePkt
	//						变化遥测信息【AppMeasureMgr→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureChangePkt : public CMeasureAllPkt
	{
	public:
		CMeasureChangePkt();
	    virtual ~CMeasureChangePkt();
	};

	//==============================================================================
	//
	//								CMeasSave
	//						遥测信息存储包【AppMeasureMgr→OSM模块】-遥测点
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasSave : public CObjectBase
	{
	public:
		CMeasSave();
		virtual ~CMeasSave();
		
	public:
		void SetMeasId(DWORD dwId);									// 设置遥测点Id
		void SetSaveTime(timeval tSet);								// 设置记录时间,不调用则默认使用类对象的定义时间
		void SetMeasName(string strSet);							// 设置遥测名称
		void SetMeasUnit(string strSet);							// 设置单位
		void SetMeasType(ENUM_MEASURE_TYPE eSet);					// 设置遥测点类型
		void SetVolLevel(BYTE bySet);								// 设置遥测电压等级
		void SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag);				// 设置质量标志
		void SetValue(float fValue);								// 设置遥测值
		
		DWORD GetMeasId() const;									// 获取遥测点Id
		CProtocolTime* GetSaveTime();								// 获取记录时间
		string GetMeasName() const;									// 获取遥测名称
		string GetMeasUnit() const;									// 获取单位
		ENUM_MEASURE_TYPE GetMeasType() const;						// 获取遥测点类型	
		BYTE GetVolLevel() const;									// 获取遥测电压等级
		ENUM_QUALITY_FLAG_TYPE GetQltFalg() const;					// 获取质量标志
		float GetValue() const;										// 获取遥测值
		
		int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		DWORD m_dwId;												// 遥测点Id
		CProtocolTime m_objTime;									// 记录时间
		string m_strName;											// 遥测名称
		string m_strUnit;											// 单位
		BYTE m_byMeasType;											// 遥测点类型
		BYTE m_byLevel;												// 遥测电压等级
		BYTE m_byQltFlag;											// 质量标志
		float m_fValue;												// 遥测值
	};
	
	
	//==============================================================================
	//
	//								CStaMeasSave
	//						遥测信息存储包【AppMeasureMgr→OSM模块】-车站
	//
	//==============================================================================
	class MY_EXT_CLASS CStaMeasSave : public CObjectBase
	{
	public:
		CStaMeasSave();
		virtual ~CStaMeasSave();
		
	public:
		void SetStaId(WORD wSId);									// 设置车站Id
		void Add(CMeasSave& objAdd);								// 添加遥测
		
		WORD GetStaId() const;										// 获取车站Id
		WORD GetMeasCount() const;									// 获取遥测数量
		CMeasSave* GetMeas(int nIndex);								// 获取指定下标遥测
		
		virtual void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		WORD m_wStaId;												// 车站Id
		vector<CMeasSave> m_vecMeasure;								// 遥测
	};
	
	
	//==============================================================================
	//
	//								CMeasureSavePkt
	//						遥测值信息存储包【AppMeasureMgr→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasSavePkt : public CProtocolHead
	{
	public:
		CMeasSavePkt();
		virtual ~CMeasSavePkt();
		
	public:
		void Add(CStaMeasSave& objAdd);								// 添加车站

		WORD GetStaCount() const;									// 获取车站数量
		CStaMeasSave* GetSta(int nIndex);							// 获取指定下标车站

		void Clear();
		
		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		vector<CStaMeasSave> m_vecSta;								// 车站
	};

	//==============================================================================
	//
	//								CMeasLimitSave
	//						遥测越限信息存储包【AppMeasureMgr→OSM模块】-越限记录
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasLimitSave : public CObjectBase
	{
	public:
		CMeasLimitSave();
		virtual ~CMeasLimitSave();
		
	public:
		void SetStaId(WORD wSet);									// 车站Id
		void SetStaName(string strSet);								// 车站名称
		void SetObjId(DWORD dwSet);									// 对象Id
		void SetObjName(string strSet);								// 对象名称
		void SetVolLevel(BYTE bySet);								// 电压等级
		void SetSaveTime(timeval tSet);								// 发生时间,不调用则默认使用类对象的定义时间
		void SetRecoverFlag(ENUM_LIMIT_RECOVER eSet);				// 产生恢复标志
		void SetAlarmType(ENUM_ALARM_TYPE eSet);					// 报警事件类型
		void SetLimitType(ENUM_LIMIT_SAVE_TYPE eSet);				// 越限类型
		void SetValue(float fValue);								// 越限值
		void SetUnit(string strSet);								// 单位
		void SetDesc(string strSet);								// 描述
		
		WORD GetStaId();											// 车站Id
		string GetStaName();										// 车站名称
		DWORD GetObjId();											// 对象Id
		string GetObjName();										// 对象名称
		BYTE GetVolLevel();											// 电压等级
		CProtocolTime* GetSaveTime();								// 发生时间,不调用则默认使用类对象的定义时间
		ENUM_LIMIT_RECOVER GetRecoverFlag();						// 产生恢复标志
		ENUM_ALARM_TYPE GetAlarmType();								// 报警事件类型
		ENUM_LIMIT_SAVE_TYPE GetLimitType();						// 越限类型
		float GetValue();											// 越限值
		string GetUnit();											// 单位
		string GetDesc();											// 描述
		
		int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		WORD m_wStaId;												// 车站ID
		string m_strStaName;										// 车站名称
		DWORD m_dwObjId;											// 对象ID
		string m_strObjName;										// 对象名称
		BYTE m_byLevel;												// 电压等级
		CProtocolTime m_objTime;									// 发生时间
		BYTE m_byRecoverFlag;										// 产生恢复标志
		WORD m_wAlarmType;											// 报警事件类型
		BYTE m_byLimitType;											// 越限类型
		float m_fValue;												// 越限值
		string m_strUnit;											// 单位
		string m_strDesc;											// 描述长度
	};
	//==============================================================================
	//
	//								CMeasLimitSavePkt
	//						遥测越限信息存储包【AppMeasureMgr→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasLimitSavePkt : public CProtocolHead
	{
	public:
		CMeasLimitSavePkt();
		virtual ~CMeasLimitSavePkt();
		
	public:
		void Add(CMeasLimitSave& objAdd);							// 添加记录
		
		WORD GetSaveCount() const;									// 获取记录数量
		CMeasLimitSave* GetSave(int nIndex);						// 获取指定下标记录
		
		void Clear();
		
		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		vector<CMeasLimitSave> m_vecSave;							// 记录
	};

	//==============================================================================
	//
	//								CMeasureSaveReplyPkt
	//						遥测信息存储回执包【OSM模块→AppMeasureMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureSaveReplyPkt : public CProtocolHead
	{
	public:
		CMeasureSaveReplyPkt();
	    virtual ~CMeasureSaveReplyPkt();

	public:
		void SetRst(DWORD dwRst);									// 设置接收结果
		void SetDesc(string strDesc);								// 设置描述

		DWORD GetRst() const;										// 获取接收结果
		string GetDesc() const;										// 获取描述

		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwRst;												// 接收结果
		string m_strDesc;											// 描述
	};

	//==============================================================================
	//
	//								CMeasureHis
	//						遥测历史信息查询中车站【操作台→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureHis : public CObjectBase
	{
	public:
		CMeasureHis();
	    virtual ~CMeasureHis();

	public:
		void SetStaId(WORD wStaId);									// 设置车站Id
		WORD GetStaId() const;										// 获取车站Id

		void Add(DWORD dwMeasure);									// 添加遥测点
		WORD GetMeasureCount() const;								// 获取遥测数量
		DWORD GetMeasure(int nIndex) const;							// 获取指定下标遥测点

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// 车站Id
		vector<DWORD> m_vecMessure;									// 遥测点Id
	};

	//==============================================================================
	//
	//								CMeasureHisPkt
	//						遥测历史信息查询包【操作台→OSM模块】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureHisPkt : public CProtocolHead
	{
	public:
		CMeasureHisPkt();
	    virtual ~CMeasureHisPkt();

	public:
		void SetStartTime(time_t t, WORD wMilliSec);				// 设置起始时间
		void SetEndTime(time_t t, WORD wMilliSec);					// 设置终止时间

		timeval GetStartTime() const;								// 获取起始时间
		timeval GetEndTime() const;									// 获取终止时间

		void Add(CMeasureHis His);									// 添加车站
		CMeasureHis* GetSta(int nIndex);							// 获取指定下标的车站
		WORD GetStaCount() const;									// 获取车站数量

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		virtual ProtocolBase::CProtocolHead* Clone();
	private:
		CProtocolTime m_StartTime;									// 查询起始时间
		CProtocolTime m_EndTime;									// 查询终止时间
		vector<CMeasureHis> m_vecSta;								// 车站
	};

	//==============================================================================
	//
	//								CMeasureHisReplyPkt
	//						遥测信息查询回执包【OSM模块→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureHisReplyPkt : public CMeasSavePkt
	{
	public:
		CMeasureHisReplyPkt();
	    virtual ~CMeasureHisReplyPkt();
	};

	//==============================================================================
	//
	//								CMeasureReqPkt
	//						请求遥测信息包【操作台→AppMeasureMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureReqPkt : public CMeasureCollReqPkt
	{
	public:
		CMeasureReqPkt();
	    virtual ~CMeasureReqPkt();
	};

	//==============================================================================
	//
	//								CMeasureCallPkt
	//						遥测总召信息包【InterSvr104/101→AppMeasureMgr】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCallPkt : public CMeasureCollAllPkt
	{
	public:
		CMeasureCallPkt();
		virtual ~CMeasureCallPkt();
	};
	
	//==============================================================================
	//
	//								CMeasureQueryFailRepPkt
	//						遥测信息查询失败回执包【OSM模块→操作台】
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureQueryFailRepPkt : public CMeasureSaveReplyPkt
	{
	public:
		CMeasureQueryFailRepPkt();
		virtual ~CMeasureQueryFailRepPkt();
	};
}

#endif	// _PROTOCOL_MEASURE_H