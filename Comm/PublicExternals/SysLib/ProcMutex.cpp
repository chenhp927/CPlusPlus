//==============================================================================
//                      ProcMutex.cpp
//
//begin   : 2016-08-09
//describe: 该文件是进程间同步类的源文件，支持跨平台Windows/Linux
//          Windows下进程间同步采用互斥锁，Linux下进程间同步采用信号量
//=============================================================================

#include "ProcMutex.h"

#ifdef WIN32
//==============================================================================               
//                    CProcMutex
//					  多进程同步锁
//==============================================================================
CProcMutex::CProcMutex(const char* pName)
{
	SetLockName(pName);
}

CProcMutex::CProcMutex()
{
}

CProcMutex::~CProcMutex()
{
	//关闭互斥锁句柄
    CloseHandle(m_hMutex);
}

void CProcMutex::Lock()
{
	//等待互斥锁
    DWORD nRet = WaitForSingleObject(m_hMutex, INFINITE);
}

void CProcMutex::UnLock()
{
	//释放互斥锁
	ReleaseMutex(m_hMutex);
}

bool CProcMutex::SetLockName(const char* pName)
{
	assert(pName != NULL);
	//设置进程名称
    memset(m_chMutexName, 0 ,sizeof(m_chMutexName));
    int min = strlen(pName)>(sizeof(m_chMutexName)-1)?(sizeof(m_chMutexName)-1):strlen(pName);
    strncpy(m_chMutexName, pName, min);
    //初始化互斥锁
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, m_chMutexName);
	if (!m_hMutex)
	{
		//创建并初始化互斥锁
		m_hMutex = CreateMutex(NULL, FALSE, m_chMutexName);
	}

	return true;
}

#endif

//==============================================================================               
//                    CProcMutex
//					  多进程同步锁
//==============================================================================
#ifdef UNIX
CProcMutex::CProcMutex(const char* pName)
	:m_nTimeWaitMs(TIMEWAIT)
{
	SetLockName(pName);
}

CProcMutex::CProcMutex()
	:m_nTimeWaitMs(TIMEWAIT)
{
}

CProcMutex::~CProcMutex()
{
	//关闭有名信号量
	sem_close(m_pSem);
  	//从系统中删除该信号量
  	sem_unlink(m_chMutexName);
}

void CProcMutex::Lock()
{
	//等待共享资源
	if(-1 == SemWaitTime(m_nTimeWaitMs))
	{
		return;
	}
}

void CProcMutex::UnLock()
{
	//释放共享资源
	sem_post(m_pSem);
}

bool CProcMutex::SetLockName(const char* pName)
{
	assert(pName != NULL);
	//设置进程名称
	memset(m_chMutexName, 0 ,sizeof(m_chMutexName));
	int min = strlen(pName)>(sizeof(m_chMutexName)-1)?(sizeof(m_chMutexName)-1):strlen(pName);
	strncpy(m_chMutexName, pName, min);
	//创建并初始化有名信号量
	m_pSem = sem_open(pName, O_RDWR | O_CREAT, 0644, 1);
	
	return true;
}

bool CProcMutex::SetTimeWait(int nTimeWaitMs)
{
	if(nTimeWaitMs < 0)
	{
		return false;
	}
	m_nTimeWaitMs = nTimeWaitMs;
	return true;
}

int CProcMutex::SemWaitTime(int nWaitMs)
{
	timespec ts;                           
  	clock_gettime(CLOCK_REALTIME, &ts );     //获取当前时间  
  	ts.tv_sec += (nWaitMs / 1000 );        	 //加上等待时间的秒数  
  	ts.tv_nsec += ( nWaitMs % 1000 ) * 1000; //加上等待时间纳秒数  
  	
 	int nRet = sem_timedwait(m_pSem, &ts );                            
 	while( (nRet != 0) && (errno ==  EINTR) )   //等待信号量，errno==EINTR屏蔽其他信号事件引起的等待中断  
  	{
  		nRet = sem_timedwait(m_pSem, &ts );
  	}
  	return nRet;     
}

#endif
