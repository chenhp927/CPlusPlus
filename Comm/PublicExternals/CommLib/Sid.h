//==============================================================================
//                      Sid.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_SID_H 
#define SELFDEF_SID_H

#include "TypeDef.h"

//==============================================================================
//                 
//                    CSid
//               ª·ª∞id£®session id£©¿‡
//
//==============================================================================
class MY_EXT_CLASS CSid 
{ 
public: 
	CSid(); 
	virtual ~CSid(); 
 
public: 
	void Asign(unsigned int nHigh = 0); 
	void Abolish(); 
	bool IsValid() const; 
	int  GetHigh() const;
	int  GetLow() const;
	void Import(int nHigh, int nLow); 

	static int StreamSize();
	int ToStreamBuf(char* szBuf) const;
	int FromStreamBuf(const char* szBuf);

public:
	CSid& operator=(CSid& objSid);
	bool operator==(CSid objSid) const; 
	bool operator!=(CSid objSid) const; 

private:
	static int m_nCounter; 
 
	unsigned int m_nHigh; 
	unsigned int m_nLow;
}; 
 
#endif 
 
