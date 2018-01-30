//==============================================================================
//                      DataStreamStat.h
//
//begin   : 2017-05-05
//describe:
//==============================================================================
#ifndef SELFDEF_DATASTREAMSTAT_H 
#define SELFDEF_DATASTREAMSTAT_H 

#include "ObjectBase.h"

//==============================================================================
//                 
//                    CDataStreamStat
//                    数据流统计类
//
//==============================================================================
class MY_EXT_CLASS CDataStreamStat : public CObjectBase 
{ 
public: 
	CDataStreamStat(); 
	virtual ~CDataStreamStat(); 
 
protected:
	// 重置统计
	void Reset();

public:
	// 设置统计周期（单位：秒）
	void SetPeriod(DWORD dwPeriod);
	
	// 对收发数据流的响应处理
	void OnDataStream(DWORD dwStreamSize = 0, string strLogTag = "");
	
protected: 
 	DWORD	m_dwPeriod;			// 统计的周期（单位：秒）
 	DWORD	m_dwStartTime; 		// 统计的开始时间
	DWORD	m_dwCount;			// 统计的数据流合计次数
	DWORD	m_dwSize;			// 统计的数据流合计大小
	DWORD	m_dwSizeM;			// 统计的数据流合计大小（单位是M），防止数据流达到4G时，m_dwHourSize字段越界
	DWORD	m_dwMaxSize;		// 统计期间最大的数据流大小
  
}; 

//==============================================================================
//                 
//                    CDataStreamStatMgr
//                    数据流统计管理类
//
//==============================================================================
class MY_EXT_CLASS CDataStreamStatMgr : public CObjectBase 
{ 
public: 
	CDataStreamStatMgr(); 
	virtual ~CDataStreamStatMgr(); 

public:
	/* 下列的接口为重载基类接口 */
	// 设置日志文件的标识
	virtual void SetLogFileTag(const char* szFileTag);

	// 对收发数据流的响应处理
	void OnDataStream(DWORD dwStreamSize = 0, string strLogTag = "");
	
protected: 
	CDataStreamStat	m_objDssHour;	// 小时级的统计
	CDataStreamStat	m_objDssMinute;	// 分钟级的统计
	CDataStreamStat	m_objDssSecond;	// 秒级的统计
}; 

 
#endif 
