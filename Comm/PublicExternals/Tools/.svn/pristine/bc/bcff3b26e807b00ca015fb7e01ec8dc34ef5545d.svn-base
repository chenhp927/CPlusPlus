//==============================================================================
//                      PthreadWrap.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_PTHREADWRAP_H
#define SELFDEF_PTHREADWRAP_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <assert.h>
#include <vector>
#include <string>

#include "PthMutex.h"

using namespace std;
//==============================================================================
//                 
//                    CPthCond
//                    
//
//==============================================================================
//#ifdef USE_MULTI_THREAD
class CPthCond
{
public:
	CPthCond(CPthMutex *pmutex);
	~CPthCond();
public:
	bool WaitCond(int timeout=-1);
	void SignalCond(bool broadcast=false);

private:
	CPthMutex* m_pMutex;
	pthread_cond_t m_Cond;
};

//==============================================================================
//                 
//                    CPthGuard
//                    
//
//==============================================================================
class CPthGuard
{
public:
	CPthGuard();
	~CPthGuard();

public:
	void Lock();
	void Unlock();

	void SignalCond(bool broadcast=false);
	bool WaitCond(int timeout=-1);
	
private:
	CPthMutex mutex;
	CPthCond cond;
};
//#endif

//==============================================================================
//                 
//                    CRwLock
//                    
//
//==============================================================================
class CRwLock 
{
public:
	CRwLock();
	~CRwLock(); 

public:
	void GetReadLock();
	void ReleaseReadLock();
	void GetWriteLock();
	void ReleaseWriteLock();
	void FinalRelease();

private:
//#ifdef USE_MULTI_THREAD
	CPthGuard guard;
	int rd_cnt;
	int wr_cnt;
//#endif
};

//==============================================================================
//                 
//                    CAutoReadLock
//                    
//
//==============================================================================
class CAutoReadLock
{
public:
    CAutoReadLock(CRwLock* p);
    ~CAutoReadLock();

private:
//#ifdef USE_MULTI_THREAD
	CRwLock* m_pRwlock;
//#endif
};

//==============================================================================
//                 
//                    CAutoWriteLock
//                    
//
//==============================================================================
class CAutoWriteLock
{
public:
    CAutoWriteLock(CRwLock* p);
    ~CAutoWriteLock();

private:
//#ifdef USE_MULTI_THREAD
	CRwLock* m_pRwlock;
//#endif
};

//#ifdef USE_MULTI_THREAD
//==============================================================================
//                 
//                    CPthAttr
//                    
//
//==============================================================================
class CPthAttr
{
public:
    CPthAttr();
    ~CPthAttr();

private:
    pthread_attr_t m_Attr;

public:
    bool SetDetached();
    bool SetJoinable();
    bool GetDetachState(int *stat);
    const pthread_attr_t* GetPtr()        { return &m_Attr;   }
};

//==============================================================================
//                 
//                    CPthread
//                    
//
//==============================================================================
class CPthread
{
public:
	CPthread() : tid(0) { };
	virtual ~CPthread() { };
	bool Start();
	bool Stop();
	pthread_t GetThreadId() { return tid; };
	
protected:
	void TestCancel();
	int SetCancelType(int type);
	int SetCancelTypeAsync();
	int BlockSig();
	virtual bool Init() { return true; };
	virtual bool Uninit() { return true; };
	virtual void Run() { };
	static void Cleanup(void* arg);
	static void* ThreadProc(void* arg);
protected:
	pthread_t tid;
	
};
//#endif

    
#endif



 
