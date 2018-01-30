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
//                   ͨ�ù�����
//
//==============================================================================
class MY_EXT_CLASS CUtility 
{ 
public: 
	CUtility(); 
	virtual ~CUtility(); 

public: 
	/* ���еĽӿ�ΪĿ¼����ش��� */
	// ����Ŀ¼
	// �������Ϊ��Ŀ¼���ţ�'\'��'/'��Ϊ��β���ַ��������ļ����ƽ�β���ַ���
	static bool MakeDir(const char* szPath);

	// �ж�Ŀ¼�Ƿ����
	static bool IsDirExist(const char* szPath);
    
	
	/* ���еĽӿ�Ϊ��������ת������ش��� */
	// �ַ���ת������
	static int ATOI(const char* sz);

	// ����ת���ַ���
	static string ITOA(unsigned int n, const char* szFormat="%d");

	// ������ת��Ϊ16�����ַ���
	static string GetHexStr(DWORD dw);

	// �ж��ַ����Ƿ�Ϊ����
	static bool IsDigital(string str);

	// ������ת��ΪBCD��
	static BYTE DecToBCD(BYTE by);


	/* ���еĽӿ�Ϊ�ַ�������ش��� */
	// ȥ���ַ����еĿո�
	static string Trim(string& str);

	// ���ַ������ָ����ֳ��Ӵ����ŵ�vector�з��أ�ÿ���Ӵ��������ָ���
	// ��"123��345��456"����"��"�ֿ���vector�з���3���Ӵ���123-345-456
	static void SplitStr(string strSrc, string strSep, vector<string>& vecRet);

	// ��Сд��ĸ�ַ���ת��Ϊ��д
	static string MakeUpper(string& str);

	// ����д��ĸ�ַ���ת��ΪСд
	static string MakeLower(string& str);


	/* ���еĽӿ�Ϊʱ�����ش��� */
	// ����ϵͳ����������ʱ�䣬ʱ�侫��Ϊ1/sysconf(_SC_CLK_TCK)��һ��Ϊ100���룬��ϵͳ��ǰʱ���޹أ�����УʱӰ��
	static DWORD GetUptime();

	// ���ص�ǰ�͹�ȥĳ��ʱ���(GetUptime���ص�)��ȵ�ʱ��������λ��
    static DWORD GetElapseTime(DWORD dwPreUptime); 

	// ���ص�ǰ�͹�ȥĳ��ʱ���(GetUptime���ص�)��ȵ�ʱ��������λ���룬���ھ��ȵĹ�ϵ��ֻ�ܾ�ȷ��10����
    static DWORD GetElapseMs( DWORD dwPreUptime );

	// ����ʱ��ĸ�ʽ���ַ���
    static string GetTimeStr(time_t t);

	// ����**�룬����ӿڲ�Ҫ����ʹ��
	static void SleepSec(BYTE bySec);
	// ����**����
	static void SleepMs(DWORD dwMs);

	/* ���еĽӿ�Ϊ�����յ���ش��� */
	// ��ȡĳ��ʱ�������ĵ�����
	static time_t GetCtrlDay(time_t t);

	// ����ĳ��ʱ�������ĵ��Ȱ��
	static int GetShift(time_t t);

}; 
 
 
#endif 
