//==============================================================================
//                      SessionProtocol.h
//
//begin   : 2016-05-13
//describe:
//==============================================================================
#ifndef SELFDEF_SESSION_PROTOCOL_H 
#define SELFDEF_SESSION_PROTOCOL_H 
 
#include "Buffer.h" 
#include "CommAddr.h"
 
#define CUR_SESSIONVERSION		1 
#define MAX_SESSIONPACKETSIZE	(1024*1024) 
 
// Session Protocol Type define 
#define SPT_PARAMNEG		1 
#define SPT_PARAMACK		2 
#define SPT_HEART			3 
#define SPT_DATA			4 
#define SPT_DATAACK			5 

// 客户端连接标识定义
#define CLIENTTAG_APP		1	// 业务程序客户端
#define CLIENTTAG_COMM		2	// 通信程序客户端
 
#pragma pack(1) 
 
//==============================================================================
//                 
//                    CProtocolSession
//                     会话层协议类
//
//==============================================================================
class MY_EXT_CLASS CProtocolSession 
{ 
public: 
	CProtocolSession(); 
	virtual ~CProtocolSession(); 
	 
public: 
	//==============================================================================
	//                 
	//                     CSessionHead
	//                     会话层包头类
	//
	//==============================================================================
	class MY_EXT_CLASS CSessionHead 
	{ 
	public: 
		DWORD m_dwVersion; 
		DWORD m_dwDataLen; 
		char  m_cType; 
		DWORD m_dwSeq; 
 
	public: 
		CSessionHead(); 
		CSessionHead(DWORD dwDataLen, char cType, DWORD dwSeq); 
		virtual ~CSessionHead(); 
		 
		static int  HeaderSize();
		static bool IsHeadValid(const char* szBuf, int nLen); 
		virtual int ToStreamBuf(char* szBuf) const; 
		virtual int FromStreamBuf(const char* szBuf); 
	}; 
 
	//==============================================================================
	//                 
	//                     CCSessionNegReq
	//                   会话层协商请求包类
	//
	//==============================================================================
	class MY_EXT_CLASS CCSessionNegReq : public CSessionHead 
	{ 
	public: 
		CCommAddr	m_objClientAddr;
		BYTE		m_byClientTag;
 		DWORD		m_dwPairNetIp; 

	public: 
		CCSessionNegReq(); 
		CCSessionNegReq(DWORD dwNextSeq); 
		virtual ~CCSessionNegReq(); 
		 
		static int Size(); 
		static bool IsValid(const char* szBuf, int nLen); 
		virtual int ToStreamBuf(char* szBuf); 
		virtual int FromStreamBuf(const char* szBuf); 
	}; 
 
	//==============================================================================
	//                 
	//                     CCSessionNegAck
	//                   会话层协商应答包类
	//
	//==============================================================================
	class MY_EXT_CLASS CCSessionNegAck : public CSessionHead 
	{ 
	public: 
		char m_cSendWindowSize; 
		char m_cAckInterval; 
		char m_cAckTimeout; 
		char m_cHeartInterval; 
		char m_cHeartTimeout; 
 
	public: 
		CCSessionNegAck(); 
		CCSessionNegAck(DWORD dwNextSeq, char cWinSize, char cAckInter, 
					  char cAckTimeout, char cHeartInter, char cHeartTimeout); 
		virtual ~CCSessionNegAck(); 
 
		static int Size(); 
		static bool IsValid(const char* szBuf, int nLen); 
		virtual int ToStreamBuf(char* szBuf); 
		virtual int FromStreamBuf(const char* szBuf); 
	}; 
 
protected: 
	static int  DataType(const char* szBuf); 
 
public:  
	static int  HeaderSize(); 
	static bool IsValid(const char* szBuf, int nLen);  
 
	static int  StreamPacketSize(const char* szBuf);  
	static int  StreamDataSize(const char* szBuf);   
	static int  MinPacketSize();  
	static int  MaxPacketSize();  
 
	static bool IsHeart(const char* szBuf); 
	static bool IsData(const char* szBuf); 
	static bool IsDataAck(const char* szBuf); 
	static bool IsNegReq(const char* szBuf); 
	static bool IsNegAck(const char* szBuf); 
 
	static int MakeHeartPacket(char* szBuf, int nLen); 
	static int MakeDataAckPacket(char* szBuf, int nLen, DWORD dwSeq); 
	static int MakeNegReqPacket(char* szBuf, int nLen, DWORD dwNextSeq, CCommAddr& objCltAddr, BYTE byCltTag, DWORD dwPairNetIp);  
	static int MakeNegAckPacket(char* szBuf, int nLen,	DWORD dwNextSeq, char cWinSize, 
		char cAckInter, char cAckTimeout, char cHeartInter, char cHeartTimeout);  
 
	static DWORD GetPacketSeq(const char* szBuf);
	static void SetPacketSeq(char* szBuf, DWORD dwSeq);
 
	static char GetWinSizeFromNegAck(const char* szBuf); 
	static char GetAckIntervalFromNegAck(const char* szBuf); 
	static char GetAckTimeoutFromNegAck(const char* szBuf); 
	static char GetHeartIntervalFromNegAck(const char* szBuf); 
	static char GetHeartTimeoutFromNegAck(const char* szBuf); 
}; 
 
#pragma pack() 
 
#endif 
