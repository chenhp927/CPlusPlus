//==============================================================================
//                      Utility.h
//
//begin   : 2016-09-21
//describe:
//==============================================================================
#ifndef SELFDEF_UTILITY_H 
#define SELFDEF_UTILITY_H 

#include <time.h>
#include <string>
#include <vector>

#include "TypeDef.h"

using namespace std;
//==============================================================================
//                 
//                    CUtility
//                   通用工具类
//
//==============================================================================
class MY_EXT_CLASS CUtility 
{ 
public: 
	CUtility(); 
	virtual ~CUtility(); 

public: 
	/* 下列的接口为目录的相关处理 */
	// 创建目录
	// 传入参数为以目录符号（'\'和'/'）为结尾的字符串或以文件名称结尾的字符串
	static bool MakeDir(const char* szPath);

	// 判定目录是否存在
	static bool IsDirExist(const char* szPath);
    
	
	/* 下列的接口为数据类型转换的相关处理 */
	// 字符串转换整数
	static int ATOI(const char* sz);

	// 整数转换字符串
	static string ITOA(unsigned int n, const char* szFormat="%d");

	// 将整数转换为16进制字符串
	static string GetHexStr(DWORD dw);

	// 判断字符串是否为数字
	static bool IsDigital(string str);

	// 将整数转换为BCD码
	static BYTE DecToBCD(BYTE by);


	/* 下列的接口为字符串的相关处理 */
	// 去除字符串中的空格
	static string Trim(string& str);

	// 把字符串按分隔符分成子串，放到vector中返回，每个子串不包含分隔符
	// 如"123；345；456"，按"；"分开后，vector中返回3个子串：123-345-456
	static void SplitStr(string strSrc, string strSep, vector<string>& vecRet);

	// 将小写字母字符串转换为大写
	static string MakeUpper(string& str);

	// 将大写字母字符串转换为小写
	static string MakeLower(string& str);


	/* 下列的接口为时间的相关处理 */
	// 返回系统启动后的相对时间，时间精度为1/sysconf(_SC_CLK_TCK)（一般为100）秒，跟系统当前时间无关，不受校时影响
	static DWORD GetUptime();

	// 返回当前和过去某个时间点(GetUptime返回的)相比的时间间隔，单位秒
    static DWORD GetElapseTime(DWORD dwPreUptime); 

	// 返回当前和过去某个时间点(GetUptime返回的)相比的时间间隔，单位毫秒，由于精度的关系，只能精确到10毫秒
    static DWORD GetElapseMs( DWORD dwPreUptime );

	// 返回时间的格式化字符串
    static string GetTimeStr(time_t t);

	// 挂起**秒，这个接口不要随意使用
	static void SleepSec(BYTE bySec);
	// 挂起**毫秒
	static void SleepMs(DWORD dwMs);

	/* 下列的接口为调度日的相关处理 */
	// 获取某个时间所属的调度日
	static time_t GetCtrlDay(time_t t);

	// 返回某个时间所属的调度班次
	static int GetShift(time_t t);

}; 
 
 
#endif 
