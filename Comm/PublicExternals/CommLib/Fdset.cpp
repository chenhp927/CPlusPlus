//==============================================================================
//                      Fdset.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#include "Fdset.h" 

//==============================================================================
//                 
//                    CFdset
//                    socket事件选择器fdset封装类
//
//==============================================================================
CFdset::CFdset() 
{ 
	Zero(); 
} 
 
CFdset::~CFdset() 
{ 
} 

void CFdset::Zero() 
{ 
	FD_ZERO(&m_objFsRead); 
	FD_ZERO(&m_objFsWrite); 
	FD_ZERO(&m_objFsException); 
	m_nMaxFd = 0; 
} 
 
void CFdset::SetRead(int nFd)   
{ 
	assert(nFd >= 0); 
	FD_SET((unsigned int)nFd, &m_objFsRead); 
	if (m_nMaxFd < nFd)
	{
		m_nMaxFd = nFd; 
	}
} 
 
void CFdset::SetWrite(int nFd) 
{ 
	assert(nFd >= 0); 
	FD_SET((unsigned int)nFd, &m_objFsWrite); 
	if (m_nMaxFd < nFd)
	{
		m_nMaxFd = nFd; 
	}
} 
 
void CFdset::SetExcept(int nFd) 
{ 
	assert(nFd >= 0); 
	FD_SET((unsigned int)nFd, &m_objFsException); 
	if (m_nMaxFd < nFd)
	{
		m_nMaxFd = nFd; 
	}
} 
 
bool CFdset::IsInFdSet( int nFd, fd_set& objFdSet )
{
#ifdef UNIX
	return FD_ISSET(nFd, &objFdSet);
#else
	return FD_ISSET(nFd, &objFdSet) > 0;
#endif
}

bool CFdset::IsSetRead(int nFd) 
{ 
	assert(nFd >= 0); 
	return IsInFdSet(nFd, m_objFsRead); 
} 
 
bool CFdset::IsSetWrite(int nFd) 
{ 
	assert(nFd >= 0); 
	return IsInFdSet(nFd, m_objFsWrite); 
} 
 
bool CFdset::IsSetExcept(int nFd) 
{ 
	assert(nFd >= 0); 
	return IsInFdSet(nFd, m_objFsException); 
} 
 
int CFdset::Select(int nMilliseconds) 
{ 
	assert(nMilliseconds >= 0); 
	struct timeval tv; 
	tv.tv_sec = nMilliseconds / 1000; 
	tv.tv_usec = (nMilliseconds - tv.tv_sec * 1000) * 1000; 

	return select(m_nMaxFd + 1, &m_objFsRead, &m_objFsWrite, &m_objFsException, &tv); 
} 

int CFdset::SelectEx(unsigned int nMicroseconds)
{
	struct timeval tv;
	tv.tv_sec = nMicroseconds / (1000*1000);
	tv.tv_usec = (nMicroseconds - tv.tv_sec * 1000 * 1000);

	return select(m_nMaxFd + 1, &m_objFsRead, &m_objFsWrite, &m_objFsException, &tv);
}
