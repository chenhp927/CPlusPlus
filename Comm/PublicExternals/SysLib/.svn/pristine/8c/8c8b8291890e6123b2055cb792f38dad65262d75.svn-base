//==============================================================================
//                      ObjectBase.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_OBJECT_BASE_H 
#define SELFDEF_OBJECT_BASE_H 

#pragma warning(disable : 4251)

#include <string>
#include "TypeDef.h"

using namespace std; 
//==============================================================================
//                 
//                    CObjectBase
//                    对象基类
//
//==============================================================================
class MY_EXT_CLASS CObjectBase  
{ 
public: 
	CObjectBase(); 
	virtual ~CObjectBase(); 

public:
	/* 下列的接口为日志文件操作的相关处理 */
	// 设置日志文件的标识，日志名为：***_标识_***
	virtual void SetLogFileTag(const char* szFileTag);
	// 获取日志基本路径
	string GetLogFileBase();

	// 获取标识
	char* GetLogFileTag() const;

	// 使能重要日志
	virtual void EnableLogImportant(bool bEnable = true);

	// 使能普通日志
	virtual void EnableLogDebug(bool bEnable = true);

	/* 下列的接口为日志记录操作的相关处理 */
	virtual void LogImportant(const char* szFormat, ...); 
	virtual void LogDebug(const char* szFormat, ...); 
	virtual void LogDataImportant(const char *pData, int len, const char* szDesc = NULL); 
	virtual void LogDataDebug(const char *pData, int len, const char* szDesc = NULL); 

	/* 下列的接口为类名称操作的相关处理 */
	// 获取类名称，用于基类指针向派生类强制转换时的检验
	void SetMyClassName(string strClassName);
	bool IsMyClassName(string strClassName);

protected: 
	string m_strLogFileTag;	/* 日志文件的标识，，如：程序启动时设置的
		LogFileBase = "AAA", 本接口设置FileTag = "XXX", 则自动生成两个
		日志文件：AAA_XXX.log 和 AAA_XXX_debug.log		*/
	
	bool m_bEnableLogImportant;
	bool m_bEnableLogDebug;

	string m_strClassName;	// 本类自己的名称，类构造时设置，常用于从基类指针强制转换为派生类指针的时的校验
}; 
 
#endif 
