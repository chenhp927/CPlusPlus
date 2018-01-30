//==============================================================================
//                      Fdset.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_FDSET_H 
#define SELFDEF_FDSET_H 

#include "SockMacro.h"
#include "TypeDef.h"

//==============================================================================
//                 
//                    CFdset
//                    socket�¼�ѡ����fdset��װ��
//
//==============================================================================
class MY_EXT_CLASS CFdset 
{ 
public: 
	CFdset(); 
	~CFdset(); 

private:
	bool IsInFdSet(int nFd, fd_set& objFdSet);

public: 
	void Zero(); 
	void SetRead(int nFd); 
	void SetWrite(int nFd); 
	void SetExcept(int nFd); 
	bool IsSetRead(int nFd); 
	bool IsSetWrite(int nFd); 
	bool IsSetExcept(int nFd); 

	//��������0����λ����
	int Select(int nMilliseconds); 

	//��������0����λ΢��
	int SelectEx(unsigned int nMicroseconds);
	
public: 
	fd_set m_objFsRead; 
	fd_set m_objFsWrite; 
	fd_set m_objFsException; 
 
	int m_nMaxFd; 
}; 
 
#endif 
