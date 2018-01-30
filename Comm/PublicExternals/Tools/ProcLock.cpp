//==============================================================================
//                      ProcLock.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <libgen.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "ProcLock.h" 
#include "CTime.h"
#include "AppPath.h"

//==============================================================================
//                 
//                    CProcLock
//                    
//
//==============================================================================
CProcLock::CProcLock() 
{ 
	memset(lockedFile, 0, sizeof(lockedFile));
	lockedFd = -1;
} 
 
CProcLock::~CProcLock() 
{
	if(lockedFile[0] != 0 && lockedFd > 0)
	{
		if (lockf(lockedFd, F_ULOCK, 0) != 0)
		{
			printf("unlock %s fail errno=%d.\n", lockedFile, errno); 
		}
	} 
} 
 
bool CProcLock::Lock(const char* szLockFile)
{
	char szFullName[PLFILENAMELEN];
	memset(szFullName, 0, sizeof(szFullName));
	string strLockPath = CAppPath::GetLockPath();
	if (strLockPath.size() <= 0)
	{
		return false;
	}
	snprintf(szFullName, sizeof(szFullName), "%s", strLockPath.c_str());	

	char cmd[1024];
	sprintf(cmd, "mkdir -p %s", szFullName);
	system(cmd);
	
	strcat(szFullName, szLockFile);
	bool bRet = true;
	int fd = open(szFullName, O_WRONLY | O_CREAT);
	if (fd < 0)
	{
		printf("open lock file %s fail.\n", szFullName);
		bRet = false;
	}
	else if (lockf(fd, F_TLOCK, 0) != 0)
	{
		printf("proc exist, lock %s fail errno=%d.\n", szFullName, errno); 
		bRet = false;
	}
	else
	{
		printf("lock %s suc\n", szFullName); 
		strcpy(lockedFile, szFullName);
		lockedFd = fd;
	}
	return bRet;
}
void CProcLock::Unlock()
{
	if(lockedFile[0] != 0 && lockedFd > 0)
	{
		printf("unlock %s\n", lockedFile);
		if (lockf(lockedFd, F_ULOCK, 0) != 0)
		{
			printf("unlock %s fail errno=%d.\n", lockedFile, errno); 
		}
	} 
}
