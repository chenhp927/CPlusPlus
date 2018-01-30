//==============================================================================
//                      Queue.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_QUEUE_H 
#define SELFDEF_QUEUE_H

#include "TypeDef.h"
#include "PthMutex.h"

//==============================================================================
//                 
//                    CQueue
//                    ∂”¡–¿‡
//
//==============================================================================
template <class T>
class CQueue
{
private:
	struct Node_T
	{
		T data;
		Node_T *pNext;
	};
	
public:
	CQueue();
	~CQueue();

protected:
	 bool	IsFull();
	 bool	Expand();
	 bool	_IsEmpty();
	 void	Release();

public:
	bool	AddHead(const T& t);
	bool	AddTail(const T& t);
	bool	GetHead(T& t);
	bool	GetTail(T& t);
	bool	PeekHead(T& t);
	bool	PeekTail(T& t);
	bool	IsEmpty();
	void	Empty();
	int		GetSize() const;
	void	SetMaxSize(int iMaxSize = 0);
	bool	IsOverflow();

private:
	Node_T *m_pHead, *m_pTail;
	int	m_iSize;
	int m_iMaxSize;
	CPthMutex	 m_objMutex;

};

template <class T>
CQueue<T>::CQueue() : m_iSize(0), m_iMaxSize(0)
{
	m_pHead = new Node_T;
	m_pTail = m_pHead;
	m_pTail->pNext = m_pHead;
}

template <class T>
CQueue<T>::~CQueue()
{
	Release();
}

template <class T>
bool CQueue<T>::IsFull()
{
	return (m_pHead == m_pTail->pNext);
}

template <class T> 
bool CQueue<T>::_IsEmpty()
{
	return (m_pHead == m_pTail);
}

template <class T>
bool CQueue<T>::Expand()
{
	if (!m_pTail)
	{
		return false;
	}

	if (m_iMaxSize != 0 && m_iSize >= m_iMaxSize)
	{
		return false;
	}

	Node_T *pNode = new Node_T;
	if (!pNode)
	{
		return false;
	}
	pNode->pNext = m_pTail->pNext;
	m_pTail->pNext = pNode;

	return true;
}

template <class T>
void CQueue<T>::Release()
{
	CAutoLock objAutoLock(&m_objMutex);

	Node_T *pNode;
	pNode = m_pTail->pNext;
	while (pNode != m_pTail)
	{
		m_pTail->pNext = pNode->pNext;
		delete pNode;
		pNode = m_pTail->pNext;
	}
	delete pNode;
	m_pHead = m_pTail = NULL;
	m_iSize = 0;
}

template <class T>
bool CQueue<T>::AddHead(const T& t)
{
	CAutoLock objAutoLock(&m_objMutex);

	if (IsFull() && !Expand())
	{
		return false;
	}
	Node_T *pNode = m_pTail;
	while (pNode->pNext != m_pHead)
	{
		pNode = pNode->pNext;
	}

	pNode->data = t;
	m_pHead = pNode;
	m_iSize ++;

	return true;
}

template <class T>
bool CQueue<T>::AddTail(const T& t)
{
	CAutoLock objAutoLock(&m_objMutex);

	if (IsFull() && !Expand())
	{
		return false;
	}
	m_pTail->data = t;
	m_pTail = m_pTail->pNext;
	m_iSize++;

	return true;
}

template <class T>
bool CQueue<T>::GetHead(T& t)
{
	CAutoLock objAutoLock(&m_objMutex);

	bool bGet = true;
	if (_IsEmpty() || !m_pHead)
	{
		bGet = false;
	}
	else
	{
		t = m_pHead->data;
		m_pHead = m_pHead->pNext;
		m_iSize --;
	}

	return bGet;
}

template <class T>
bool CQueue<T>::GetTail(T& t)
{
	CAutoLock objAutoLock(&m_objMutex);

	bool bGet = true;
	if (_IsEmpty() || !m_pHead)
	{
		bGet = false;
	}
	else
	{
		Node_T *pNode = m_pHead;
		while (pNode->pNext != m_pTail)
			pNode = pNode->pNext;
		t = pNode->data;
		m_pTail = pNode;
		m_iSize--;
	}

	return bGet;
}

template <class T>
bool CQueue<T>::PeekHead(T& t)
{
	CAutoLock objAutoLock(&m_objMutex);

	bool bPeek = true;
	if (_IsEmpty() || !m_pHead)
	{
		bPeek = false;
	}
	else
	{
		t = m_pHead->data;
	}

	return bPeek;
}

template <class T>
bool CQueue<T>::PeekTail(T& t)
{
	CAutoLock objAutoLock(&m_objMutex);

	bool bPeek = true;
	if (_IsEmpty() || !m_pTail)
	{
		bPeek = false;
	}
	else
	{
		t = m_pTail->data;
	}

	return bPeek;
}

template <class T>
bool CQueue<T>::IsEmpty()
{
	bool bEmpty;
	bEmpty = _IsEmpty();

	return bEmpty;
}

template <class T>
int CQueue<T>::GetSize() const
{
	return m_iSize;
}

template <class T>
void CQueue<T>::SetMaxSize(int iMaxSize)
{
	if (iMaxSize >= 0)
	{
		m_iMaxSize = iMaxSize;
	}
}

template <class T>
bool CQueue<T>::IsOverflow()
{
	return (m_iMaxSize != 0 && m_iSize >= m_iMaxSize);
}

template <class T>
void CQueue<T>::Empty()
{
	CAutoLock objAutoLock(&m_objMutex);

	m_pHead = m_pTail;
	m_iSize = 0;
}

#endif

