//==============================================================================
//                      PthreadWrap.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#include <errno.h>
#include <iostream>
#include <sys/time.h>

#include "PthreadWrap.h"

//==============================================================================
//                 
//                    CPthCond
//                    
//
//==============================================================================
//#ifdef USE_MULTI_THREAD
CPthCond::CPthCond(CPthMutex *pmutex) : m_pMutex(pmutex)
{
	int re = pthread_cond_init(&m_Cond, NULL);
	assert(re == 0);
};

CPthCond::~CPthCond()
{
	pthread_cond_destroy(&m_Cond);
};

bool CPthCond::WaitCond(int timeout)
{
	if(timeout <= -1)
	{
		int re = pthread_cond_wait(&m_Cond, &m_pMutex->m_Mutex);
		assert(re == 0);
		return true;
	}
	else
	{
		struct timeval now;
		struct timespec waittime;
		gettimeofday(&now, NULL);
		waittime.tv_sec = now.tv_sec + timeout;
		waittime.tv_nsec = now.tv_usec * 1000;
		int re = pthread_cond_timedwait(&m_Cond, &m_pMutex->m_Mutex, &waittime);
		if(re == ETIMEDOUT)
		{
			return false;
		}
		else
		{
			assert(re == 0);
			return true;
		}
	}
};

void CPthCond::SignalCond(bool broadcast)
{
	int re = -1;
	if(broadcast)
		re = pthread_cond_broadcast(&m_Cond);
	else
		re = pthread_cond_signal(&m_Cond);
	assert(re == 0);
};

//==============================================================================
//                 
//                    CPthGuard
//                    
//
//==============================================================================
CPthGuard::CPthGuard() : cond(&mutex) 
{ 
};
CPthGuard::~CPthGuard()
{ 
};

void CPthGuard::Lock()
{
	mutex.Lock();
};

void CPthGuard::Unlock()
{
	mutex.Unlock();
};

void CPthGuard::SignalCond(bool broadcast)
{
	cond.SignalCond(broadcast);
};

bool CPthGuard::WaitCond(int timeout)
{
	return cond.WaitCond(timeout);
};
//#endif

//==============================================================================
//                 
//                    CRwLock
//                    
//
//==============================================================================
//#ifdef USE_MULTI_THREAD
CRwLock::CRwLock(): rd_cnt(0),wr_cnt(0) 
//#else
//CRwLock::CRwLock()
//#endif
{
}
CRwLock::~CRwLock() 
{
}

void CRwLock::GetReadLock() 
{
//#ifdef USE_MULTI_THREAD
	guard.Lock();
	while (wr_cnt >0) 
	{
		guard.WaitCond();
	}
	rd_cnt++;
	guard.Unlock(); 
//#endif
}

void CRwLock::ReleaseReadLock()
{
//#ifdef USE_MULTI_THREAD
	guard.Lock();
	rd_cnt--;
	if (0 == rd_cnt) 
	{
		guard.SignalCond();
	}
	guard.Unlock(); 
//#endif
}

void CRwLock::GetWriteLock() 
{
//#ifdef USE_MULTI_THREAD
	guard.Lock();
	while (rd_cnt+wr_cnt>0) 
	{
		guard.WaitCond();
	}
	wr_cnt++;
	guard.Unlock(); 
//#endif
}

void CRwLock::ReleaseWriteLock() 
{
//#ifdef USE_MULTI_THREAD
	guard.Lock();
	wr_cnt--;
	guard.SignalCond(true);
	guard.Unlock(); 
//#endif
}
void CRwLock::FinalRelease()
{
//#ifdef USE_MULTI_THREAD
	guard.Unlock();
//#endif
};

//==============================================================================
//                 
//                    CAutoReadLock
//                    
//
//==============================================================================
CAutoReadLock::CAutoReadLock(CRwLock* p)
{
//#ifdef USE_MULTI_THREAD
	m_pRwlock = p;
	m_pRwlock->GetReadLock();
//#endif
};
CAutoReadLock::~CAutoReadLock()
{
//#ifdef USE_MULTI_THREAD
	m_pRwlock->ReleaseReadLock();
//#endif
};

//==============================================================================
//                 
//                    CAutoWriteLock
//                    
//
//==============================================================================
CAutoWriteLock::CAutoWriteLock(CRwLock* p)
{
//#ifdef USE_MULTI_THREAD
	m_pRwlock = p;
	m_pRwlock->GetWriteLock();
//#endif
};
CAutoWriteLock::~CAutoWriteLock()
{
//#ifdef USE_MULTI_THREAD
	m_pRwlock->ReleaseWriteLock();
//#endif
};

//#ifdef USE_MULTI_THREAD

//==============================================================================
//                 
//                    CPthAttr
//                    
//
//==============================================================================
CPthAttr::CPthAttr()
{
    pthread_attr_init(&m_Attr);
}

CPthAttr::~CPthAttr()
{
    pthread_attr_destroy(&m_Attr);
}

bool CPthAttr::SetDetached()
{
    int iRet = pthread_attr_setdetachstate(&m_Attr, PTHREAD_CREATE_DETACHED);
    if (!iRet)
        return true;
    else
    {
        std::cout << "pthread_attr_setdetachstate detach fail ret=" 
		<< iRet << std::endl;
        return false;
    }
}
    
bool CPthAttr::SetJoinable()
{
    int iRet = pthread_attr_setdetachstate(&m_Attr, PTHREAD_CREATE_JOINABLE);
    if (!iRet)
        return true;
    else
    {
        std::cout << "pthread_attr_setdetachstate join fail ret=" << iRet << std::endl;
        return false;
    }

}

bool CPthAttr::GetDetachState(int *stat) 
{
    return !pthread_attr_getdetachstate(&m_Attr, stat);
}

//==============================================================================
//                 
//                    CPthread
//                    
//
//==============================================================================
bool CPthread::Start()
{
	tid = 0;
	pthread_attr_t attr;
	int re = pthread_attr_init(&attr);
	assert(re == 0);
	//size_t default_th_stacksize;
	//re = pthread_attr_getstacksize(&attr, &default_th_stacksize);
	//assert(re == 0);
	//printf("default_th_stacksize=%d\n",default_th_stacksize);
	re = pthread_attr_setstacksize(&attr, 1*1024*1024);
	assert(re == 0);
	re = pthread_create(&tid, &attr, ThreadProc, this);
	re = pthread_attr_destroy(&attr);
	assert(re == 0);
	return (re == 0);
};

bool CPthread::Stop()
{
	int re = pthread_cancel(tid);
	if(re < 0) return false;
	re = pthread_join(tid, NULL);
	if(re == ESRCH)
	{
		printf("pthread_join=ESRCH\n");
		return false;
	}
	if(re == EINVAL)
	{
		printf("pthread_join=EINVAL\n");
		return false;
	}
	if(re == EDEADLK)
	{
		printf("pthread_join=EDEADLK\n");
		return false;
	}
	return (re == 0);
};

void CPthread::TestCancel()
{
	pthread_testcancel();
};

int CPthread::SetCancelType(int type)
{
	int oldtype = 0;
	int re = pthread_setcanceltype(type, &oldtype);
	assert(re == 0);
	return oldtype;
}

int CPthread::SetCancelTypeAsync()
{
	return SetCancelType(PTHREAD_CANCEL_ASYNCHRONOUS);
}

int CPthread::BlockSig()
{
	sigset_t mask;
	int re;
	sigfillset(&mask); /* Mask all allowed signals */
	re = pthread_sigmask(SIG_BLOCK, &mask, NULL);
	if(re < 0)
	{
//	    printf("pthread_sigmask err, %d, %s\n", re, strerror(re));
	}
	return re;
}

void CPthread::Cleanup(void* arg)
{
	CPthread* pthis = (CPthread*)arg;
	pthis->Uninit();
};

void* CPthread::ThreadProc(void* arg)
{
	CPthread* pthis = (CPthread*)arg;
	//防止线程接收到退出信号,这对于linux好像没有用,对unix的真正的线程还是有用的
	pthis->BlockSig();
	pthread_cleanup_push(Cleanup, pthis);
	if(!pthis->Init())
	{
		printf("thread init fail\n");
		pthis->Uninit();
		return 0;
	}
	pthis->Run();
	pthis->Uninit();
	pthread_cleanup_pop(0);
	return 0;
};
//#endif //end of USE_MULTI_THREAD
