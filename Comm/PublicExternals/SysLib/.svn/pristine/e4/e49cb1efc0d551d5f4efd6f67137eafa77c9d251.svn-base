//==============================================================================
//                      PthMutex.h
//
//begin   : 2016-08-26
//describe:
//==============================================================================
#ifndef SELFDEF_PTHMUTEX_H
#define SELFDEF_PTHMUTEX_H

#include "TypeDef.h"

//#ifdef USE_MULTI_THREAD
	#ifdef UNIX
		#include <pthread.h>
	#else
		#include <afxmt.h>
	#endif
//#endif

//==============================================================================
//                 
//                    CPthMutex
//                    
//
//==============================================================================
class MY_EXT_CLASS CPthMutex
{
#ifdef UNIX
	friend class CPthCond;
#endif

public:
    CPthMutex();
    ~CPthMutex();

public:
    void Lock();
    void Unlock();

private:
//#ifdef USE_MULTI_THREAD
	#ifdef UNIX
		pthread_mutex_t m_Mutex;
	#else
		CCriticalSection m_Mutex;
	#endif
//#endif
};

//==============================================================================
//                 
//                    CAutoLock
//                    
//
//==============================================================================
class MY_EXT_CLASS CAutoLock
{
public:
    CAutoLock(CPthMutex* p);
    ~CAutoLock();

private:
//#ifdef USE_MULTI_THREAD
	CPthMutex* m_pMutex;
//#endif
};


    
#endif



 
