//==============================================================================
//                      ProcMutex.h
//
//begin   : 2016-08-09
//describe: 该文件是进程间同步类的头文件，支持跨平台Windows/Linux
//          Windows下进程间同步采用互斥锁，Linux下进程间同步采用信号量
//备注    ：在Linux平台下，由于源文件中使用了clock_gettime()，编译时要加上 -lrt 选项！
//==============================================================================
#ifndef __PROC_MUTEX_H__
#define __PROC_MUTEX_H__

#include <assert.h>

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef UNIX
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
	
const int TIMEWAIT = 1000*2; //默认进程锁最大等待时间
#endif

//==============================================================================               
//                    CProcMutex
//                 	多线程同步锁，创建对象后必须设置锁名
//==============================================================================
class CProcMutex
{
public:
    CProcMutex(const char* pName);
	// !!!!!!用不带参数的构造函数创建对象后必须设置锁名!!!!!!!
    CProcMutex();
    ~CProcMutex();
    
    //加锁
    void Lock();
    //解锁
    void UnLock();

	// 设置锁名
	bool SetLockName(const char* pName);
    
#ifdef UNIX
	//设置最大等待时间
	bool SetTimeWait(int nTimeWaitMs);
#endif


private:
#ifdef UNIX
	//SemWaitTime是为了防止进程意外退出造成的死锁，可以通过设置最大等待时间，超过等待时间后，进程会自动获取等待锁的所有权
	int SemWaitTime(int nWaitMs = TIMEWAIT);
#endif
	
private:
#ifdef WIN32
    HANDLE m_hMutex;          //互斥锁句柄
#endif

#ifdef UNIX
    sem_t* m_pSem;      	  //信号量
    int m_nTimeWaitMs;  	  //进程锁最大等待时间
#endif
    char m_chMutexName[100];  //进程名称
};

#endif