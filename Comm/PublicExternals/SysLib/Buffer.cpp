//==============================================================================
//                      Buffer.cpp
//
//begin   : 2016-08-23
//describe: 缓存分配与管理
//==============================================================================
#include <stdio.h>

#include "Buffer.h"

//==============================================================================
//                 
//                    CBuffer
//                    
//
//==============================================================================
CBuffer::CBuffer(CBuffer::CAllocator &objAlloc, int nSize, int nExtend) : 
			m_nBufSize(nSize), 
			m_nBufUsed(0),  
			m_nExtendSize(nExtend),
			m_nInitSize(nSize),
			m_nWParam(0),
			m_nLParam(0),
			m_nFlag(0),
			m_objAllocator(objAlloc)
{
	assert(nSize > 0 && nExtend > 0);
	m_pBuf = new char[m_nBufSize];
	
	if(!m_pBuf)
	{
		printf("new buf fail in CBuffer::CBuffer()\n");
	}

	m_strDesc = "";
}

CBuffer::~CBuffer()
{
	if(m_pBuf)
	{
		delete []m_pBuf;
	}
}

bool CBuffer::Extend(int nSizeNeed)
{
	if(nSizeNeed <= 0)
	{
		return false;
	}

	// nExtend为需要扩展的空间
	int nExtend = nSizeNeed - GetAvailableSize(); 
	if(nExtend <= 0) //不需要扩展，那么返回成功
	{
		return true;
	}
	
	// 需要扩展的空间是m_nExtendSize的整数倍
	if(nExtend % m_nExtendSize != 0)
	{
		nExtend = (nExtend / m_nExtendSize + 1) * m_nExtendSize; 
	}
	
	assert(nExtend > 0);
	if (nExtend > 0)
	{
		// buffer对象的内存是以m_nExtendSize的整数倍来分配的
		char* pBuf = new char[m_nBufSize + nExtend]; 
		if (!pBuf)
		{
			return false;
		}
			
		if(m_nBufUsed > 0)
		{
			memcpy(pBuf, m_pBuf, m_nBufUsed);
		}
			
		delete []m_pBuf;
		m_pBuf = pBuf;
		m_nBufSize += nExtend;
	}
	
	return true;
}

bool CBuffer::Shrink()
{
	 // 如果可用空间大于初始分配空间，那么需要收缩
	if (m_nBufSize - m_nBufUsed > m_nInitSize)
	{
		int nNewSize = ((m_nBufUsed / m_nInitSize) + 1) * m_nInitSize;
		if(m_nBufUsed > 0 && m_nBufUsed % m_nInitSize == 0)
		{
			nNewSize = m_nBufUsed;
		}
		
		 // 新分配的内存大小能容下已用空间，并且为初始分配大小的整数倍
		char *pBuf = new char[nNewSize];
		if (!pBuf) 
		{
			return false;
		}
			
		if(m_nBufUsed > 0)
		{
			memcpy(pBuf, m_pBuf, m_nBufUsed);
		}
			
		delete []m_pBuf;
		m_pBuf = pBuf;
		m_nBufSize = nNewSize;
	}
	
	return true;
}

char* CBuffer::GetBuffer()
{
	return m_pBuf;
}

int CBuffer::GetBufSize()
{
	return m_nBufSize;
}

void CBuffer::Use(int nUsed)
{
	assert(nUsed >= 0 && m_nBufUsed + nUsed <= m_nBufSize);
	m_nBufUsed += nUsed; 
}

int	CBuffer::GetUsed() const
{
	return m_nBufUsed;
}

char* CBuffer::GetAvailableBuf()
{
	return m_pBuf + m_nBufUsed;
}

int	CBuffer::GetAvailableSize()
{
	assert(m_nBufSize - m_nBufUsed >= 0);
	return m_nBufSize - m_nBufUsed;
}

int	CBuffer::AddData(const char *pData, int nSize)
{
	if(!pData || nSize <= 0)
	{
		return -1;
	}
		
	if ((m_nBufUsed + nSize) > m_nBufSize && !Extend(nSize))
	{
		return -1;
	}
		
	memcpy(m_pBuf + m_nBufUsed, pData, nSize);
	m_nBufUsed += nSize;
	
	return m_nBufUsed;
}

int CBuffer::AddData(CBuffer* pBuf)
{
	return AddData(pBuf->GetBuffer(), pBuf->GetUsed());
}

int CBuffer::AddData(void *pData, int nBlockSize, int nBlockCount)
{
	return AddData((const char *)pData, nBlockSize * nBlockCount);
}

int	CBuffer::Insert(int nPos, void *pData, int nSize)
{
	if (nPos < 0 || nPos > m_nBufUsed || nSize <= 0)
	{
		return -1;
	}

	if ((m_nBufUsed + nSize) > m_nBufSize && !Extend(nSize))
	{
		return -1;
	}

	if(m_nBufUsed - nPos > 0)
	{
		memmove(m_pBuf + nPos + nSize, m_pBuf + nPos, m_nBufUsed - nPos);
	}
		
	memcpy(m_pBuf + nPos, pData, nSize);
	m_nBufUsed += nSize;
	
	return m_nBufUsed;
}

int	CBuffer::Interspace(int nPos, int nSize)
{
	if (nPos < 0 || nPos > m_nBufUsed || nSize <= 0)
	{
		return -1;
	}

	if ((m_nBufUsed + nSize) > m_nBufSize && !Extend(nSize))
	{
		return -1;
	}

	if(m_nBufUsed - nPos > 0)
	{
		memmove(m_pBuf + nPos + nSize, m_pBuf + nPos, m_nBufUsed - nPos);
	}
		
	memset(m_pBuf + nPos, 0, nSize);
	m_nBufUsed += nSize;
	
	return m_nBufUsed;
}

void CBuffer::Cut(int nPos, int nSize)
{
	if (nPos < 0 || nPos > m_nBufUsed || nSize <= 0)
	{
		return;
	}

	if (nSize > m_nBufUsed - nPos)
	{
		nSize = m_nBufUsed - nPos;
	}
	
	if (nSize > 0)
	{
		if(m_nBufUsed - nPos - nSize > 0)
		{
			memmove(m_pBuf + nPos, m_pBuf + nPos + nSize, m_nBufUsed - nPos - nSize);
		}
		
		m_nBufUsed -= nSize;
	}
}

CBuffer* CBuffer::Split(int nSplit)
{
	if (nSplit <= 0 || nSplit > m_nBufSize)
	{
		return NULL;
	}
		
	CBuffer *p = m_objAllocator.Allocate();
	if (!p)
	{
		return NULL;
	}
		
	if (p->AddData(m_pBuf, nSplit) == -1)
	{
		p->Release();
		return NULL;
	}
	
	m_nBufUsed -= nSplit;
	if(m_nBufUsed > 0)
	{
		memmove(m_pBuf, m_pBuf + nSplit, m_nBufUsed);
	}

	return p;
}

CBuffer* CBuffer::Dup()
{
	CBuffer *p = m_objAllocator.Allocate();
	if (!p)
	{
		return NULL;
	}
		
	if (p->AddData(m_pBuf, m_nBufUsed) == -1)
	{
		p->Release();
		return NULL;
	}
	
	p->m_nWParam = m_nWParam;
	p->m_nLParam = m_nLParam;
	p->m_nFlag = m_nFlag;
	p->m_strDesc = m_strDesc;
	
	return p;	
}

void CBuffer::Release()
{
	m_nBufUsed = 0;
	m_objAllocator.Release(this);	
}

void CBuffer::Empty() 
{
	m_nBufUsed = 0;
}

void CBuffer::SetWParam(int w)
{
	m_nWParam = w;
}

void CBuffer::SetLParam(int l)
{
	m_nLParam = l;
}

int CBuffer::GetWParam()
{
	return m_nWParam;
}

int	CBuffer::GetLParam()
{
	return m_nLParam;
}

void CBuffer::SetFlag(int nFlag)
{
	m_nFlag = nFlag;
}

int CBuffer::GetFlag() const
{
	return m_nFlag;
}

void CBuffer::SetDesc(const char* szDesc)
{
	if(szDesc == NULL)
	{
		m_strDesc = "";
	}
	else
	{
		m_strDesc = szDesc;
	}
}

string CBuffer::GetDesc() const
{ 
	return m_strDesc;
}

//==============================================================================
//                 
//                    CBuffer::CAllocator
//                    
//
//==============================================================================
CBuffer::CAllocator::CAllocator(int nBufSize, int nExtend, int nMaxFreeBufs) :
		m_nBufSize(nBufSize), 
		m_nExtendSize(nExtend), 
		m_nMaxFreeBufs(nMaxFreeBufs)
{
}

CBuffer::CAllocator::~CAllocator()
{
	CAutoLock objAutoLock(&m_objMutex);

	CBuffer* pBuf;
	while (!m_lstFreeBuf.empty()) 
	{
		pBuf = m_lstFreeBuf.front();
		m_lstFreeBuf.pop_front();
		delete pBuf;
		pBuf = NULL;
	}
	
	while (!m_lstActiveBuf.empty()) 
	{
		pBuf = m_lstActiveBuf.front();
		m_lstActiveBuf.pop_front();
		delete pBuf;
		pBuf = NULL;
	}
}

void CBuffer::CAllocator::Release(CBuffer *pBuf)
{
	CAutoLock objAutoLock(&m_objMutex);

	if (!pBuf)
	{
		return;
	}
	
	// 先从active list中查找pBuf指针	
	bool bFind = false;
	list<CBuffer *>::iterator iter;
	for (iter = m_lstActiveBuf.begin(); iter != m_lstActiveBuf.end(); iter++)
	{
		if (*iter == pBuf)
		{
			bFind = true;
			break;
		}
	}
	
	if (bFind)
	{
		// 将找到的pBuf指针从active list中移除	
		m_lstActiveBuf.erase(iter);

		// 如果free list的大小没有到其最大值，将其放入free list中
		if (m_nMaxFreeBufs == 0 || m_lstFreeBuf.size() < m_nMaxFreeBufs)
		{
			pBuf->SetLParam(0);
			pBuf->SetWParam(0);
			pBuf->SetFlag(0);
			pBuf->SetDesc("");

			m_lstFreeBuf.push_front(pBuf);
		}
		// 如果free list的大小已达最大值，将内存释放
		else
		{
			delete pBuf;
		}
	}
	else
	{
		assert(0);
	}
}		

CBuffer *CBuffer::CAllocator::Allocate(const char* szDesc)
{
	CAutoLock objAutoLock(&m_objMutex);

	CBuffer *pBuf = NULL;
	if (!m_lstFreeBuf.empty()) 
	{
		pBuf = m_lstFreeBuf.front();
		m_lstFreeBuf.pop_front();
	}
	else
	{
		pBuf = new CBuffer(*this, m_nBufSize, m_nExtendSize);
		if (!pBuf)
		{
			printf("CBuffer::CAllocator new buffer fail.\n");
		}
	}
	
	if (pBuf)
	{
		pBuf->SetLParam(0);
		pBuf->SetWParam(0);
		pBuf->SetFlag(0);
		pBuf->SetDesc(szDesc);

		m_lstActiveBuf.push_front(pBuf);
	}
	
	return pBuf;
}

void CBuffer::CAllocator::SetMaxFreeBufs(unsigned int nMaxFreeBufs)
{
	CAutoLock objAutoLock(&m_objMutex);

	if(nMaxFreeBufs > MAXFREEBUFS)
	{
		printf("SetMaxFreeBufs too big\n");
		return;
	}

	//设置free list的最大长度
	//先清除多余的free buffer
	CBuffer* pBuf = NULL;
	while (!m_lstFreeBuf.empty())// && m_lstFreeBuf.size() > nMaxFreeBufs)
	{
		pBuf = m_lstFreeBuf.front();
		m_lstFreeBuf.pop_front();
		if(pBuf)
		{
			delete pBuf;
		}
	}
	
	m_nMaxFreeBufs = nMaxFreeBufs;
}

void CBuffer::CAllocator::CleanFreeList()
{
	CAutoLock objAutoLock(&m_objMutex);

	CBuffer* pBuf = NULL;
	while (!m_lstFreeBuf.empty())
	{
		pBuf = m_lstFreeBuf.front();
		m_lstFreeBuf.pop_front();
		if(pBuf)
		{
			delete pBuf;
		}
	}
}

int CBuffer::CAllocator::GetFreeSize() const
{
	return m_lstFreeBuf.size();
}

int CBuffer::CAllocator::GetActiveSize() const
{
	return m_lstActiveBuf.size();
}

std::string CBuffer::CAllocator::GetDumpStr()
{
	CAutoLock objAutoLock(&m_objMutex);
	
	string strRet = "";

	char chTmp[1024];
	memset(chTmp, 0, sizeof(chTmp));

	// m_lstActiveBuf
	snprintf(chTmp, sizeof(chTmp), "m_lstActiveBuf: cur count=%d, buffers=", m_lstActiveBuf.size());
	strRet += chTmp;
	int nAllSize = 0;

	list<CBuffer *>::iterator iter;
	for (iter = m_lstActiveBuf.begin(); iter != m_lstActiveBuf.end(); iter++)
	{
		CBuffer* pBuf = *iter;
		assert(pBuf);

		if (nAllSize > 0)
		{
			strRet += " + ";
		}
		nAllSize += pBuf->GetBufSize();
		snprintf(chTmp, sizeof(chTmp), "%d", pBuf->GetBufSize());
		strRet += chTmp;
	}

	snprintf(chTmp, sizeof(chTmp), "=%d\n", nAllSize);
	strRet += chTmp;

	// m_lstFreeBuf
	snprintf(chTmp, sizeof(chTmp), "m_lstFreeBuf: max count=%d, cur count=%d, buffers=", m_nMaxFreeBufs, m_lstFreeBuf.size());
	strRet += chTmp;
	nAllSize = 0;
	
	for (iter = m_lstFreeBuf.begin(); iter != m_lstFreeBuf.end(); iter++)
	{
		CBuffer* pBuf = *iter;
		assert(pBuf);
		
		if (nAllSize > 0)
		{
			strRet += " + ";
		}
		nAllSize += pBuf->GetBufSize();
		snprintf(chTmp, sizeof(chTmp), "%d", pBuf->GetBufSize());
		strRet += chTmp;
	}
	
	snprintf(chTmp, sizeof(chTmp), "=%d\n", nAllSize);
	strRet += chTmp;

	return strRet;
}


