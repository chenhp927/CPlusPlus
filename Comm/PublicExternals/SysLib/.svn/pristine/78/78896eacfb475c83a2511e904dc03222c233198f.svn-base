//==============================================================================
//                      PthMutex.cpp
//
//begin   : 2016-08-26
//describe:
//==============================================================================

#include "PthMutex.h"

//==============================================================================
//                 
//                    CPthMutex
//                    
//
//==============================================================================
CPthMutex::CPthMutex()
{
//#ifdef USE_MULTI_THREAD
	#ifdef UNIX
		pthread_mutex_init(&m_Mutex, NULL);
	#else
	#endif
//#endif
}

CPthMutex::~CPthMutex()
{
//#ifdef USE_MULTI_THREAD
	#ifdef UNIX
		pthread_mutex_destroy(&m_Mutex);
	#else
	#endif
//#endif
}

void CPthMutex::Lock()
{
//#ifdef USE_MULTI_THREAD
	#ifdef UNIX
		pthread_mutex_lock(&m_Mutex);
	#else
		m_Mutex.Lock();
	#endif
//#endif
}

void CPthMutex::Unlock()
{
//#ifdef USE_MULTI_THREAD
	#ifdef UNIX
		pthread_mutex_unlock(&m_Mutex);
	#else
		m_Mutex.Unlock();
	#endif
//#endif
}

//==============================================================================
//                 
//                    CAutoLock
//                    
//
//==============================================================================
CAutoLock::CAutoLock(CPthMutex* p)
{
//#ifdef USE_MULTI_THREAD
	m_pMutex = p;
	m_pMutex->Lock();
//#endif
};
CAutoLock::~CAutoLock()
{
//#ifdef USE_MULTI_THREAD
	m_pMutex->Unlock();
//#endif
};


