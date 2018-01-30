//==============================================================================
//                      ForwardCondition.cpp
//
//begin   : 2016-09-01
//describe:
//==============================================================================
#ifdef UNIX
	#include "CTime.h"
#endif

#include <assert.h>

#include "ForwardCondition.h" 
#include "StrTools.h"

//==============================================================================
//                 
//                    CCondAddr
//                   消息地址条件类
//
//==============================================================================
CForwardCondition::CCondAddr::CCondAddr()
{
	
}

CForwardCondition::CCondAddr::~CCondAddr()
{
	
}

bool CForwardCondition::CCondAddr::IsMatched(CProtocolForward::CForwardAddr& objAddr)
{
	if (!IsBureauIdMatched(objAddr.GetBureauId()))
	{
		return false;
	}

	if (!IsUnitTypeMatched(objAddr.GetUnitType()))
	{
		return false;
	}
	
	if (!IsUnitIdMatched(objAddr.GetUnitId()))
	{
		return false;
	}
	
	if (!IsDevTypeMatched(objAddr.GetDevType()))
	{
		return false;
	}
	
	if (!IsDevIdMatched(objAddr.GetDevId()))
	{
		return false;
	}
	
	if (!IsCltIdMatched(objAddr.GetCltId()))
	{
		return false;
	}
	
	return true;
}

bool CForwardCondition::CCondAddr::IsBureauIdMatched(BYTE byBureauId)
{
	if (m_vecCondBureauId.empty()) // 空条件，该条件通过
	{
		return true;
	}
	
	if (byBureauId == 0xff)
	{
		return true;
	}

	for (int x = 0; x < m_vecCondBureauId.size(); x++)
	{
		BYTE byCondBurId = m_vecCondBureauId[x];

		if (byCondBurId == 0xff || byCondBurId == byBureauId)
		{
			return true;
		}
	}

	return false;
}

bool CForwardCondition::CCondAddr::IsUnitTypeMatched(BYTE byUnitType)
{
	if (m_vecCondUnitType.empty()) // 空条件，该条件通过
	{
		return true;
	}
	
	if (byUnitType == 0xff)
	{
		return true;
	}
	
	for (int x = 0; x < m_vecCondUnitType.size(); x++)
	{
		BYTE byCondUnitType = m_vecCondUnitType[x];
		
		if (byCondUnitType == 0xff || byCondUnitType == byUnitType)
		{
			return true;
		}
	}
	
	return false;
}

bool CForwardCondition::CCondAddr::IsUnitIdMatched(WORD wUnitId)
{
	if (m_vecCondUnitId.empty() && m_vecCondUnitIdRange.empty()) // 空条件，该条件通过
	{
		return true;
	}
	
	if (wUnitId == 0xffff)
	{
		return true;
	}
	
	for (int x = 0; x < m_vecCondUnitId.size(); x++)
	{
		WORD wCondUnitId = m_vecCondUnitId[x];
		
		if (wCondUnitId == 0xffff || wCondUnitId == wUnitId)
		{
			return true;
		}
	}
	
	for (int y = 0; y < m_vecCondUnitIdRange.size(); )
	{
		WORD wCondRangeLeft = m_vecCondUnitIdRange[y];
		assert(y + 1 < m_vecCondUnitIdRange.size());
		WORD wCondRangeRight = m_vecCondUnitIdRange[y+1];

		if (wCondRangeLeft == 0xffff || wCondRangeRight == 0xffff || wCondRangeLeft <= wUnitId && wUnitId <= wCondRangeRight)
		{
			return true;
		}
		
		y += 2;
	}

	return false;
}

bool CForwardCondition::CCondAddr::IsDevTypeMatched(BYTE byDevType)
{
	if (m_vecCondDevType.empty()) // 空条件，该条件通过
	{
		return true;
	}
	
	if (byDevType == 0xff)
	{
		return true;
	}
	
	for (int x = 0; x < m_vecCondDevType.size(); x++)
	{
		BYTE byCondDevType = m_vecCondDevType[x];
		
		if (byCondDevType == 0xff || byCondDevType == byDevType)
		{
			return true;
		}
	}
	
	return false;
}

bool CForwardCondition::CCondAddr::IsDevIdMatched(WORD wDevId)
{
	if (m_vecCondDevId.empty()) // 空条件，该条件通过
	{
		return true;
	}
	
	if (wDevId == 0xffff)
	{
		return true;
	}
	
	for (int x = 0; x < m_vecCondDevId.size(); x++)
	{
		WORD wCondDevId = m_vecCondDevId[x];
		
		if (wCondDevId == 0xffff || wCondDevId == wDevId)
		{
			return true;
		}
	}
	
	return false;
}

bool CForwardCondition::CCondAddr::IsCltIdMatched(WORD wCltId)
{
	if (m_vecCondCltId.empty()) // 空条件，该条件通过
	{
		return true;
	}
	
	if (wCltId == 0xffff)
	{
		return true;
	}
	
	for (int x = 0; x < m_vecCondCltId.size(); x++)
	{
		WORD wCondCltId = m_vecCondCltId[x];
		
		if (wCondCltId == 0xffff || wCondCltId == wCltId)
		{
			return true;
		}
	}
	
	return false;
}

bool CForwardCondition::CCondAddr::IsEmpty()
{
	return m_vecCondBureauId.empty() && 
		m_vecCondUnitType.empty() && 
		m_vecCondUnitId.empty() && 
		m_vecCondUnitIdRange.empty() &&
		m_vecCondDevType.empty() && 
		m_vecCondDevId.empty() && 
		m_vecCondCltId.empty() ;
}

//==============================================================================
//                 
//                    CCondTypeFunc
//                消息类型码功能码条件类
//
//==============================================================================
CForwardCondition::CCondTypeFunc::CCondTypeFunc()
{
	
}

CForwardCondition::CCondTypeFunc::~CCondTypeFunc()
{
	
}

bool CForwardCondition::CCondTypeFunc::IsMatched(CProtocolForward::CForwardTypeFunc& objTypeFunc)
{
	if (!IsTypFuncMatched(objTypeFunc.GetType(), objTypeFunc.GetFunc()))
	{
		return false;
	}
	
	if (IsNotTypFuncMatched(objTypeFunc.GetType(), objTypeFunc.GetFunc()))
	{
		return false;
	}

	return true;
}

bool CForwardCondition::CCondTypeFunc::IsTypFuncMatched(BYTE byType, BYTE byFunc)
{
	assert(m_vecCondType.size() == m_vecCondFunc.size());

	if (m_vecCondType.empty() && m_vecCondFunc.empty())
	{
		return true;
	}

	for (int x = 0; x < m_vecCondType.size(); x++)
	{
		BYTE byCondType = m_vecCondType[x];
		bool bTypeMatched = false;
		if (byCondType == 0xff || byType == 0xff || byCondType == byType)
		{
			bTypeMatched = true;
		}
		
		BYTE byCondFunc = m_vecCondFunc[x];
		bool bFuncMatched = false;
		if (byCondFunc == 0xff || byFunc == 0xff ||  byCondFunc == byFunc)
		{
			bFuncMatched = true;
		}
		
		if (bTypeMatched && bFuncMatched)
		{
			return true;
		}
	}
	
	return false;
}

bool CForwardCondition::CCondTypeFunc::IsNotTypFuncMatched(BYTE byType, BYTE byFunc)
{
	assert(m_vecCondNotType.size() == m_vecCondNotFunc.size());

	if (m_vecCondNotType.empty() && m_vecCondNotFunc.empty())
	{
		return false;
	}
	
	for (int x = 0; x < m_vecCondNotType.size(); x++)
	{
		BYTE byCondNotType = m_vecCondNotType[x];
		bool bNotTypeMatched = false;
		if (byCondNotType == 0xff || byCondNotType == byType)
		{
			bNotTypeMatched = true;
		}
		
		BYTE byCondNotFunc = m_vecCondNotFunc[x];
		bool bNotFuncMatched = false;
		if (byCondNotFunc == 0xff || byCondNotFunc == byFunc)
		{
			bNotFuncMatched = true;
		}
		
		if (bNotTypeMatched && bNotFuncMatched)
		{
			return true;
		}
	}
	
	return false;
}

bool CForwardCondition::CCondTypeFunc::IsEmpty()
{
	return m_vecCondType.empty() &&
		m_vecCondFunc.empty() &&
		m_vecCondNotType.empty() &&
		m_vecCondNotFunc.empty();
}

//==============================================================================
//                 
//                    CCondRec
//                 消息条件记录类
//
//==============================================================================
CForwardCondition::CCondRec::CCondRec()
	: m_byPlugId(0)
{

}

CForwardCondition::CCondRec::~CCondRec()
{
	
}

bool CForwardCondition::CCondRec::IsMatched(CProtocolForward::CForwardHead& objForwardHead)
{
	// 消息中，各个不同条件是与的关系，相同条件之间是或的关系
	
	// 检验消息中的源地址与注册条件中的源地址是否匹配
	CProtocolForward::CForwardAddr objSrcAdd = objForwardHead.GetSrcAddr();	
	if (!m_objSrcAddrCond.IsMatched(objSrcAdd))
	{
		return false;
	}

	// 检验消息中的类型码功能码与注册条件中的类型码功能码是否匹配
	CProtocolForward::CForwardTypeFunc objTypeFunc = objForwardHead.GetTypeFunc();
	if (!m_objTypeFuncCond.IsMatched(objTypeFunc))
	{
		return false;
	}

	return true;
}

bool CForwardCondition::CCondRec::IsEmpty()
{
	return m_objSrcAddrCond.IsEmpty() && m_objTypeFuncCond.IsEmpty();
}

//==============================================================================
//                 
//                    CForwardCondition
//                     转发条件类
//
//==============================================================================
CForwardCondition::CForwardCondition()
{
	m_strInitRegXml = "";
	m_bIsEmptyAllowAll = false;
}

CForwardCondition::~CForwardCondition()
{
	
}

void CForwardCondition::SetEmptyAllowAll(bool bIsEmptyAllowAll)
{
	m_bIsEmptyAllowAll = bIsEmptyAllowAll;
}

string& CForwardCondition::GetXmlStr()
{
	return m_strInitRegXml;
}

bool CForwardCondition::IsMatched(CProtocolForward::CForwardHead& objForwardHead)
{
	// 消息条件集合为空，由设置的m_bIsEmptyAllowAll控制是否通过，
	if (m_vecCondRec.empty())
	{
		if (m_bIsEmptyAllowAll) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	// 有一个消息条件为真，则放行；否则，禁止放行
	for (int x = 0; x < m_vecCondRec.size(); x++)
	{
		if (m_vecCondRec[x].IsMatched(objForwardHead))
		{
			return true;
		}
	}
	
	return false;
}

int CForwardCondition::GetCount()
{
	return m_vecCondRec.size();
}

CForwardCondition::CCondRec& CForwardCondition::Get(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_vecCondRec.size());
	return m_vecCondRec[nIndex];
}

int CForwardCondition::Add(const CForwardCondition::CCondRec& msgCond)
{
	m_vecCondRec.push_back(msgCond);
	return m_vecCondRec.size();
}

void CForwardCondition::Clear()
{
	m_strInitRegXml = "";
	m_vecCondRec.clear();
}

bool CForwardCondition::IsEmpty()
{
	return GetCount() == 0;
}

bool CForwardCondition::Init(const char* szRegXml)
{
	// 传进来的串为xml格式的<InCondition>、 <OutCondition>，也即配置文件中的注册信息部分
	if (!szRegXml || szRegXml[0] == 0) // 空串表示错误信息
	{
		return false;
	}
	
	TiXmlDocument txdDoc;
	txdDoc.Parse(szRegXml);
	if (txdDoc.Error())
	{
		return false;
	}
	
	TiXmlHandle txhReg(txdDoc.RootElement());
	if (!Init(txhReg))
	{
		return false;
	}

	return true;
}

bool CForwardCondition::Init(TiXmlHandle& txhReg)
{
	if (!txhReg.Element())
	{
		return false;
	}
	
	// 每次注册都是一个全体，因此要先清除现有的
	Clear(); 

	// 遍历每一个注册条件进行解析
	TiXmlElement* pTxeRec = txhReg.FirstChildElement("Rec").Element();
	for ( ; pTxeRec; pTxeRec = pTxeRec->NextSiblingElement("Rec"))
	{
		CCondRec objCondRec;
		if (!InitCondRec(objCondRec, pTxeRec))
		{
			return false;
		}

		if (objCondRec.IsEmpty())
		{
			return false;
		}

		Add(objCondRec);
	}

	TIXML_OSTREAM os;
	txhReg.Element()->StreamOut(&os);
	m_strInitRegXml = os.c_str();
	
	return true;
}

bool CForwardCondition::InitCondRec(CCondRec& objCondRec, TiXmlElement* pTxeElem)
{
	// 读取rec里的条件
	
	// 源地址条件
	TiXmlElement* pTxeNode = pTxeElem->FirstChildElement("SrcAddrCond");
	if (pTxeNode) // 如果没有找到，那么为空
	{
		if (!InitCondAddr(objCondRec.m_objSrcAddrCond, pTxeNode))
		{
			return false;
		}
	}

	// 类型码功能码条件
	pTxeNode = pTxeElem->FirstChildElement("TypeFuncCond");
	if (pTxeNode) // 如果没有找到，那么为空
	{
		if (!InitCondTypeFunc(objCondRec.m_objTypeFuncCond, pTxeNode))
		{
			return false;
		}

		// 非类型码功能码条件
		TiXmlElement* pTxeNode2 = pTxeNode->FirstChildElement("MustNot");
		if (pTxeNode2)
		{
			if (!InitCondNotTypeFunc(objCondRec.m_objTypeFuncCond, pTxeNode2))
			{
				return false;
			}
		}
	}

	// 读取rec的属性 "PlugId"
	int nPlugId = 0;
	if (pTxeElem->Attribute("PlugId", &nPlugId))
	{
		if (nPlugId > 0 && nPlugId <= 0xff)
		{
			objCondRec.m_byPlugId = (BYTE)nPlugId;
		}
		else
		{
			objCondRec.m_byPlugId = 0xff;
		}
	}
	
	return true;
}

bool CForwardCondition::InitCondAddr(CCondAddr& objCondAddr, TiXmlElement* pTxeElem)
{
	TiXmlElement* pTxeNode = pTxeElem->FirstChildElement("BureauId");
	if (pTxeNode && pTxeNode->GetText())
	{
		CStrTools<BYTE>::SplitNumsByStr(pTxeNode->GetText(), objCondAddr.m_vecCondBureauId);
	}
	
	pTxeNode = pTxeElem->FirstChildElement("UnitType");
	if (pTxeNode && pTxeNode->GetText())
	{
		CStrTools<BYTE>::SplitNumsByStr(pTxeNode->GetText(), objCondAddr.m_vecCondUnitType);
	}
	
	pTxeNode = pTxeElem->FirstChildElement("UnitId");
	if (pTxeNode && pTxeNode->GetText())
	{
		CStrTools<WORD>::SplitNumsAndRangesByStr(pTxeNode->GetText(), objCondAddr.m_vecCondUnitId, objCondAddr.m_vecCondUnitIdRange);
	}
	
	pTxeNode = pTxeElem->FirstChildElement("DeviceType");
	if (pTxeNode && pTxeNode->GetText())
	{
		CStrTools<BYTE>::SplitNumsByStr(pTxeNode->GetText(), objCondAddr.m_vecCondDevType);
	}
	
	pTxeNode = pTxeElem->FirstChildElement("DeviceId");
	if (pTxeNode && pTxeNode->GetText())
	{
		CStrTools<WORD>::SplitNumsByStr(pTxeNode->GetText(), objCondAddr.m_vecCondDevId);
	}
	
	pTxeNode = pTxeElem->FirstChildElement("ClientId");
	if (pTxeNode && pTxeNode->GetText())
	{
		CStrTools<WORD>::SplitNumsByStr(pTxeNode->GetText(), objCondAddr.m_vecCondCltId);
	}
	
	return true;
}

bool CForwardCondition::InitCondTypeFunc(CCondTypeFunc& objCondTypeFunc, TiXmlElement* pTxeElem)
{
	TiXmlElement* pTxeNode = pTxeElem->FirstChildElement("TypeFunc");
	for (; pTxeNode; pTxeNode=pTxeNode->NextSiblingElement("TypeFunc"))
	{
		if (pTxeNode && pTxeNode->GetText())
		{
			vector<BYTE> v;
			CStrTools<BYTE>::SplitNumsByStr(pTxeNode->GetText(), v);
			if (v.size() == 2)
			{
				objCondTypeFunc.m_vecCondType.push_back(v[0]);
				objCondTypeFunc.m_vecCondFunc.push_back(v[1]);
			}
			else if (v.size() == 1)
			{
				objCondTypeFunc.m_vecCondType.push_back(v[0]);
				objCondTypeFunc.m_vecCondFunc.push_back(0xff);
			}
		}
	}

	return true;
}

bool CForwardCondition::InitCondNotTypeFunc(CCondTypeFunc& objCondTypeFunc, TiXmlElement* pTxeElem)
{
	TiXmlElement *pTxeNode = pTxeElem->FirstChildElement("TypeFunc");
	for (; pTxeNode; pTxeNode=pTxeNode->NextSiblingElement("TypeFunc"))
	{
		if (pTxeNode && pTxeNode->GetText())
		{
			vector<BYTE> v;
			CStrTools<BYTE>::SplitNumsByStr(pTxeNode->GetText(), v);
			if (v.size() == 2)
			{
				objCondTypeFunc.m_vecCondNotType.push_back(v[0]);
				objCondTypeFunc.m_vecCondNotFunc.push_back(v[1]);
			}
			else if (v.size() == 1)
			{
				objCondTypeFunc.m_vecCondNotType.push_back(v[0]);
				objCondTypeFunc.m_vecCondNotFunc.push_back(0xff);
			}
		}
	}

	return true;
}

string CForwardCondition::GetDumpStr()
{
	// 输出消息条件集合，调试用
	char szBuffer[256];
	memset(szBuffer, 0, sizeof(szBuffer));
	snprintf(szBuffer, sizeof(szBuffer), "CondRecInfo vector(size=%d):\n", m_vecCondRec.size());

	string strRet = string(szBuffer);
	for (int x = 0; x < m_vecCondRec.size(); ++x)
	{
		CCondRec& objCondRec = m_vecCondRec.at(x);
		strRet += GetCondRecDumpStr(objCondRec);
	}
	
	return strRet;
}

string CForwardCondition::GetCondRecDumpStr(CCondRec& objCondRec)
{
	string strRet = "CondRec";
	if (objCondRec.m_byPlugId > 0)
	{
		char szTmp[32];
		snprintf(szTmp, sizeof(szTmp), "(PlugId=%d):\n", objCondRec.m_byPlugId);
		strRet += szTmp;
	}
	else
	{
		strRet += ":\n";
	}

	strRet += GetCondAddrDumpStr(objCondRec.m_objSrcAddrCond, 1);
	strRet += "\n";

	strRet += GetCondTypeFuncDumpStr(objCondRec.m_objTypeFuncCond);
	strRet += "\n";

	strRet += GetCondNotTypeFuncDumpStr(objCondRec.m_objTypeFuncCond);
	strRet += "\n";

	return strRet;
}

string CForwardCondition::GetCondAddrDumpStr(CCondAddr& objCondAddr, int nFlag)
{
	string strRet;
	if (nFlag == 1)
	{
		strRet = "SrcAddrCond:\n";
	}

	if (!objCondAddr.m_vecCondBureauId.empty())
	{
		strRet += "BureauIds=" + CStrTools<BYTE>::VecToStr(objCondAddr.m_vecCondBureauId) + "\n";
	}

	if (!objCondAddr.m_vecCondUnitType.empty())
	{
		strRet += "UnitTypes=" + CStrTools<BYTE>::VecToStr(objCondAddr.m_vecCondUnitType) + "\n";
	}

	if (!objCondAddr.m_vecCondUnitId.empty())
	{
		strRet += "UnitIds=" + CStrTools<WORD>::VecToStr(objCondAddr.m_vecCondUnitId) + "\n";
	}

	if (!objCondAddr.m_vecCondUnitIdRange.empty())
	{
		strRet += "UnitIdRanges=" + CStrTools<WORD>::VecToStr(objCondAddr.m_vecCondUnitIdRange) + "\n";
	}

	if (!objCondAddr.m_vecCondDevType.empty())
	{
		strRet += "DevTypes=" + CStrTools<BYTE>::VecToStr(objCondAddr.m_vecCondDevType) + "\n";
	}

	if (!objCondAddr.m_vecCondDevId.empty())
	{
		strRet += "DevIds=" + CStrTools<WORD>::VecToStr(objCondAddr.m_vecCondDevId) + "\n";
	}

	if (!objCondAddr.m_vecCondCltId.empty())
	{
		strRet += "CltIds=" + CStrTools<WORD>::VecToStr(objCondAddr.m_vecCondCltId) + "\n";
	}

	return strRet;
}

string CForwardCondition::GetCondTypeFuncDumpStr(CCondTypeFunc& objCondTypeFunc)
{
	string strRet = "TypeFuncCond:\n";

	if (!objCondTypeFunc.m_vecCondType.empty())
	{
		strRet += "Types=" + CStrTools<BYTE>::VecToHexStr(objCondTypeFunc.m_vecCondType) + "\n";
	}

	if (!objCondTypeFunc.m_vecCondFunc.empty())
	{
		strRet += "Funcs=" + CStrTools<BYTE>::VecToHexStr(objCondTypeFunc.m_vecCondFunc) + "\n";
	}

	return strRet;
}

string CForwardCondition::GetCondNotTypeFuncDumpStr(CCondTypeFunc& objCondTypeFunc)
{
	string strRet = "NotTypeFuncCond:\n";

	if (!objCondTypeFunc.m_vecCondNotType.empty())
	{
		strRet += "NotTypes=" + CStrTools<BYTE>::VecToHexStr(objCondTypeFunc.m_vecCondNotType) + "\n";
	}

	if (!objCondTypeFunc.m_vecCondNotFunc.empty())
	{
		strRet += "NotFuncs=" +CStrTools<BYTE>:: VecToHexStr(objCondTypeFunc.m_vecCondNotFunc) + "\n";
	}

	return strRet;
}


