//==============================================================================
//                      ProcLock.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_PROCLOCK_H
#define SELFDEF_PROCLOCK_H

#define PLFILENAMELEN	256

//==============================================================================
//                 
//                    CProcLock
//                    
//
//==============================================================================
class CProcLock
{ 
public: 
	CProcLock(); 
	~CProcLock(); 

protected:
	char lockedFile[1024];
	int lockedFd;

public: 
	bool Lock(const char* szCmdName);
	void Unlock();
}; 
 
 
#endif 
 
