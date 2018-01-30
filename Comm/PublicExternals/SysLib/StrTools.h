//==============================================================================
//                      StrTools.h
//
//begin   : 2016-09-26
//describe:
//==============================================================================
#ifndef SELFDEF_STRTOOLS_H 
#define SELFDEF_STRTOOLS_H 

#pragma once

#include <vector>
#include <string>
#include <string.h>

using namespace std;
//==============================================================================
//                 
//                     CStrTools
//                    string�ַ���������
//
//==============================================================================
template<class T> // TΪunsigned char, unsigned short, unsigned int
class CStrTools
{
public:
	// ɾ���ַ������˵Ŀո�Tab
	static void Trim(string& strVal);

	// ���ַ���ת��Ϊ�������ַ�����ʽ��10��0x0a��0X0A��0ah��0AH
	static int ATOI(const char* szVal);

	// ���Զ��ŷֿ��������ַ���ת��Ϊ����
	static void SplitNumsByStr(const char* szStr, vector<T>& v);

	// ���Զ��ŷֿ������ּ����ַ�Χ�ַ���ת��Ϊ����
	// szStr�ĸ�ʽ��Ϊ��3,5,6-7,8,9-11
	// ��������ת�������v�У����ַ�Χת�������v2�У�v2�е����ֳɶԳ���
	static void SplitNumsAndRangesByStr(const char* szStr, vector<T>& v, vector<T>& v2);

	// �����ּ���ת��Ϊ�Կո�ֿ����ַ���
	static string VecToStr(vector<T>& v);
	
	// �����ּ���ת��Ϊ�Կո�ֿ���16���Ƹ�ʽ�ַ���
	static string VecToHexStr(vector<T>& v);
};

template<class T>
void CStrTools<T>::Trim(string& strVal)
{
	int x,y;
	char cBlank = ' ';
	char cTab = '	';

	for (x = 0; x < strVal.size(); x++)
	{
		if (strVal[x] != cBlank && strVal[x] != cTab && strVal[x] != 0x0d && strVal[x] != 0x0a)
		{
			break;
		}
	}

	if (x != strVal.size())
	{
		for (y = strVal.size() - 1; y >= 0; y--)
		{
			if (strVal[y] != cBlank && strVal[y] != cTab && strVal[y] != 0x0d && strVal[y] != 0x0a)
			{
				break;
			}
		}
	}

	if (x == strVal.size())
	{
		strVal = "";
	}
	else if (x != 0 || y != strVal.size() - 1)
	{
		strVal = strVal.substr(x, y-x+1);
	}
}

template<class T>
int CStrTools<T>::ATOI(const char* szVal)
{
	if (strlen(szVal) <= 1)
	{
		return atoi(szVal);
	}

	if ((szVal[0] == '0' && (szVal[1] == 'x' || szVal[1] == 'X')) ||
		(szVal[strlen(szVal)-1] == 'h' || szVal[strlen(szVal)-1] == 'H'))
	{
		int nVal;
		sscanf(szVal, "%x", &nVal);
		return nVal;
	}
	else
	{
		return atoi(szVal);
	}
}

template<class T>
void CStrTools<T>::SplitNumsByStr(const char* szStr, vector<T>& v)
{
	int strLen = strlen(szStr);
	string s2 = "";
	for (int x = 0; x < strLen; x++)
	{
		if (szStr[x] == ',')
		{
			CStrTools::Trim(s2);
			if (s2 != "")
			{
				T n = CStrTools::ATOI(s2.c_str());
				v.push_back(n);
				s2 = "";
			}
		}
		else
		{
			s2 += szStr[x];
		}
	}

	CStrTools::Trim(s2);
	if (s2 != "")
	{
		T n = CStrTools::ATOI(s2.c_str());
		v.push_back(n);
		s2 = "";
	}
}

template<class T>
void CStrTools<T>::SplitNumsAndRangesByStr(const char* szStr, vector<T>& v, vector<T>& v2)
{
	int strLen = strlen(szStr);
	string s2;
	for (int x = 0; x < strLen; x++)
	{
		if (szStr[x] == ',')
		{
			CStrTools::Trim(s2);
			if (s2 != "")
			{
				int pos = s2.find('-');
				if (pos == string::npos) //û�ҵ��ַ�-���Ǹ�����
				{
					T n = CStrTools::ATOI(s2.c_str());
					v.push_back(n);
				}
				else //�ҵ��ַ�-���Ǹ����ַ�Χ
				{
					string s3 = s2.substr(0, pos);
					string s4 = s2.substr(pos + 1, s2.size() - pos - 1);
					CStrTools::Trim(s3);
					CStrTools::Trim(s4);
					T n1 = CStrTools::ATOI(s3.c_str());
					T n2 = CStrTools::ATOI(s4.c_str());
					v2.push_back(n1);
					v2.push_back(n2);
				}

				s2 = "";
			}
		}
		else
		{
			s2 += szStr[x];
		}
	}

	CStrTools::Trim(s2);
	if (s2 != "")
	{
		int pos = s2.find('-');
		if (pos == string::npos) //û�ҵ��ַ�-���Ǹ�����
		{
			T n = CStrTools::ATOI(s2.c_str());
			v.push_back(n);
		}
		else //�ҵ��ַ�-���Ǹ����ַ�Χ
		{
			string s3 = s2.substr(0, pos);
			string s4 = s2.substr(pos + 1, s2.size() - pos - 1);
			CStrTools::Trim(s3);
			CStrTools::Trim(s4);
			T n1 = CStrTools::ATOI(s3.c_str());
			T n2 = CStrTools::ATOI(s4.c_str());
			v2.push_back(n1);
			v2.push_back(n2);
		}

		s2 = "";
	}
}

template<class T>
string CStrTools<T>::VecToStr(vector<T>& v)
{
	string s = "";
	for (int x = 0; x < v.size(); x++)
	{
		char temp[64];
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "%d ", v[x]);
		s += temp;
	}

	return s;
}

template<class T>
string CStrTools<T>::VecToHexStr(vector<T>& v)
{
	string s = "";
	for (int x = 0; x < v.size(); x++)
	{
		char temp[64];
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "0x%02x ", v[x]);
		s += temp;
	}
	
	return s;
}

  
#endif 
