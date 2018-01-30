//=========================================================================================
//	
//						ProtocolSignal.h
//							   
// begin    : 2016-08-24
// describe : Э��ң����
//
//=========================================================================================
#ifndef	_PROTOCOL_SIGNAL_H
#define _PROTOCOL_SIGNAL_H

#include "ProtocolTime.h"
#include "TypeDef.h"
#include "PubDefSystem.h"
#include "ProtocolHead.h"
#include "ObjectBase.h"
#include <vector>
#include "ProtocolIndication.h"

namespace ProtocolIndication
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//	
	//	CStaCollAll
	//	ң��ȫ����Ϣ���г�վ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CStaCollAll : public CObjectBase
	{
	public:
		CStaCollAll();
		virtual ~CStaCollAll();

	public:	
		void SetStaId(WORD wStationId);								// ���ó�վId
		WORD GetStaId() const;										// ��ȡ��վId

		void SetSigStartAddr(DWORD dwStart);						// ����ң����ʼ��ַ
		DWORD GetSigStartAddr() const;								// ��ȡң����ʼ��ַ

		// ��ң�Ų���
		WORD GetSignalCount() const;								// ��ȡң������
		BYTE GetSignal(int nIndex) const;							// ��ȡң��ֵ
		void AddSignal(BYTE signal);								// ���ң��ֵ
		void Clear();												// ���ң��ֵ

		// ȫվң�Ų���
		WORD GetSigRealCnt() const;									// վ��ң������
		string GetAllSigs() const;									// ��ȡȫվң��
		void SetAllSigs(string strData);							// ��ȡȫվң��
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// ȫվң�Ű��ڴ濽����ʽ���/���
		virtual int StreamSize2();
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen);	
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen);

	private:
		WORD m_wStationId;											// ��վId
		DWORD m_dwSigStartAddr;										// ң����ʼ��ַ
		vector<BYTE> m_vecSignal;									// ң����Ϣ

		// վ��ʵ��ң������
		WORD m_wRealSigCnt;
		// վң�ż��ϣ����ڽ����ڴ濽��
		string m_strData;
	};

	//==============================================================================
	//	
	//									CSignalCollALLPkt
	//						ң��ȫ����Ϣ����InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollAllPkt : public CProtocolHead
	{
	public:
		CSignalCollAllPkt();
		virtual ~CSignalCollAllPkt();

	public:
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaCollAll* GetSta(int nIndex);							// ��ȡָ���±공վ
		void Add(CStaCollAll &sta);									// ��ӳ�վ
		void Clear();												// ��ճ�վ
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// ÿ��վ�����ڴ濽��������������
		virtual int StreamSize2();		
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen); 

	private:
		vector<CStaCollAll> m_vecSta;								// ��վ��Ϣ
	};

	//==============================================================================
	//
	//						ң�ű仯��Ϣ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalCollChange
	//						ң�ű仯��Ϣ��ң�ŵ㡾InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollChange : public CObjectBase
	{
	public:
		CSignalCollChange();
		CSignalCollChange(DWORD dwSignalAddr, BYTE byValue);
		virtual ~CSignalCollChange();

	public:
		void SetAddr(DWORD dwSignalAddr);							// ����ң�ŵ��ַ
		DWORD GetAddr() const;										// ��ȡң�ŵ��ַ
		void SetValue(BYTE byValue);								// ����ң��ֵ
		BYTE GetValue() const;										// ��ȡң��ֵ
		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwSignalAddr;										// ң�ŵ��ַ
		BYTE  m_byValue;											// ң��ֵ
	};

	//==============================================================================
	//	
	//									CStaCollChange
	//						ң�ű仯��Ϣ�г�վ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CStaCollChange : public CObjectBase
	{
	public:
		CStaCollChange();
		virtual ~CStaCollChange();

	public:
		void SetId(WORD wStationId);								// ���ó�վId
		WORD GetId() const;											// ��ȡ��վId
		WORD GetSignalCount() const;								// ��ȡң������
		CSignalCollChange* GetSignal(int nIndex);					// ��ȡָ���±��ң��
		void Add(CSignalCollChange &signal);						// ���ң��
		void Clear();												// ���ң��
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStationId;											//��վId
		vector<CSignalCollChange> m_vecSignal;						//ң����Ϣ
	};

	//==============================================================================
	//	
	//									CSignalCollChangePkt
	//						ң�ű仯��Ϣ����InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollChangePkt : public CProtocolHead
	{
	public:
		CSignalCollChangePkt();
		virtual ~CSignalCollChangePkt();

	public:
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaCollChange* GetSta(int nIndex);							// ��ȡָ���±공վ
		void Add(CStaCollChange &sta);								// ��ӳ�վ
		void Clear();												// ��ճ�վ
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaCollChange> m_vecSta;							// ��վ��Ϣ
	};

	//==============================================================================
	//
	//							ң��ȫ����Ϣ��AppSignalMgr������̨��
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalOfSignalAllInfo
	//						ң��ȫ����Ϣ����ң�ŵ㡾AppSignalMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalOfSignalAllInfo : public CObjectBase
	{
	public:
		CSignalOfSignalAllInfo();
		virtual ~CSignalOfSignalAllInfo();

	public:
		void SetSignalId(DWORD dwId);								// ����ң�ŵ�Id
		DWORD GetSignalId() const;									// ��ȡң�ŵ�Id
		void SetQltFalg(ENUM_QUALITY_FLAG_TYPE eFlag);				// ����������־
		ENUM_QUALITY_FLAG_TYPE GetQltFlag() const;					// ��ȡ������־
		void SetValue(BYTE byValue);								// ��λ����ң��ֵ
		BYTE GetValue() const;										// ��ȡң��ֵ
		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwSignalId;											// ң�ŵ�Id
		BYTE m_byQltFlag;											// ������־
		BYTE  m_byValue;											// ң��ֵ
	};

	//==============================================================================
	//	
	//										CStaOfSignalAllInfo
	//							ң��ȫ����Ϣ���г�վ��AppSignalMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CStaOfSignalAllInfo : public CObjectBase
	{
	public:
		CStaOfSignalAllInfo();
		CStaOfSignalAllInfo(WORD wId);
		virtual ~CStaOfSignalAllInfo();

	public:
		void SetId(WORD wId);											// ���ó�վId
		WORD GetId() const;												// ��ȡ��վId
		WORD GetSignalCount() const;									// ��ȡң������
		CSignalOfSignalAllInfo* GetSignal(int nIndex);					// ��ȡָ���±�ң��
		void Add(CSignalOfSignalAllInfo &signal);						// ���ң��
		virtual void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wId;														//��վId
		vector<CSignalOfSignalAllInfo> m_vecSignal;						//ң����Ϣ
	};

	//==============================================================================
	//	
	//										CSignalAllPkt
	//							ң��ȫ����Ϣ����AppSignalMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalAllPkt : public CProtocolHead
	{
	public:
		CSignalAllPkt();
		virtual ~CSignalAllPkt();

	public:		
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaOfSignalAllInfo* GetSta(int nIndex);					// ��ȡָ���±공վ
		void Add(CStaOfSignalAllInfo &sta);							// ��ӳ�վ
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaOfSignalAllInfo> m_vecSta;						// ��վ��Ϣ
	};

	//==============================================================================
	//
	//							ң�ű仯��Ϣ��AppSignalMgr������̨��
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalChangePkt
	//							ң�ű仯��Ϣ����AppSignalMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangePkt : public CSignalAllPkt
	{
	public:
		CSignalChangePkt();
	    virtual ~CSignalChangePkt();
	};

	//==============================================================================
	//
	//						ң��������Ϣ��AppMeasureMgr��InterSvr104��
	//
	//==============================================================================

	//==============================================================================
	//
	//							CSignalCollReqPkt
	//						ң��������Ϣ����AppMeasureMgr��InterSvr104��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCollReqPkt : public CProtocolHead
	{
	public:
		CSignalCollReqPkt();
		virtual ~CSignalCollReqPkt();

	public:
		WORD GetStaCount() const;									// ��ȡ��վ����
		WORD GetSta(int nIndex) const;								// ��ȡָ���±공վ
		void Add(WORD sta);											// ��ӳ�վ
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<WORD> m_vecSta;										// ��վ��Ϣ
	};

	//==============================================================================
	//
	//						ң��������Ϣ������̨��AppSignalMgr��
	//
	//==============================================================================

	//==============================================================================
	//
	//									CSignalReqPkt
	//							ң��������Ϣ��������̨��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalReqPkt : public CSignalCollReqPkt
	{
	public:
		CSignalReqPkt();
	    virtual ~CSignalReqPkt();
	};

	//==============================================================================
	//
	//							ң�ű�λ��Ϣ��AppSignalMgr��ʵʱͨ����
	//
	//==============================================================================

	//==============================================================================
	//
	//									CSignalChangeState
	//						ң�ű�λ��Ϣ��ң�ŵ㡾AppSignalMgr��ʵʱͨ����
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangeState : public CObjectBase
	{
	public:
		CSignalChangeState();
		CSignalChangeState(DWORD dwId, BYTE byValue);
		virtual ~CSignalChangeState();

	public:
		void SetId(DWORD dwId);										// ����ң�ŵ�Id
		DWORD GetId() const;										// ��ȡң�ŵ�Id
		void SetValue(BYTE byValue);								// ����ң��ֵ
		BYTE GetValue() const;										// ��ȡң��ֵ

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwId;												// ң�ŵ�Id
		BYTE m_byValue;												// ң��ֵ
	};

	//==============================================================================
	//
	//									CSignalChangeSta
	//						ң�ű�λ��Ϣ��վ��AppSignalMgr��ʵʱͨ����
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangeSta : public CObjectBase
	{
	public:
		CSignalChangeSta();
		CSignalChangeSta(WORD wId);
		virtual ~CSignalChangeSta();

	public:
		void SetId(WORD wId);										// ���ó�վId
		WORD GetId() const;											// ��ȡ��վId
		WORD GetSignalCount() const;								// ��ȡң������
		CSignalChangeState* GetSignal(int nIndex);					// ��ȡָ���±�ң��
		void Add(CSignalChangeState &signal);						// ���ң��
		void Clear();												// ���ң��

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wId;													// ��վId
		vector<CSignalChangeState> m_vecSignal;						// ң����Ϣ 
	};

	//==============================================================================
	//
	//								CSignalChangeStatePkt
	//						ң�ű�λ��Ϣ����AppSignalMgr��ʵʱͨ����
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalChangeStatePkt : public CProtocolHead
	{
	public:
		CSignalChangeStatePkt();
		virtual ~CSignalChangeStatePkt();

	public:		
		WORD GetStaCount() const;									// ��ȡ��վ����
		CSignalChangeSta* GetSta(int nIndex);						// ��ȡָ���±공վ
		void Add(CSignalChangeSta &sta);							// ��ӳ�վ
		void Clear();												// ��ճ�վ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CSignalChangeSta> m_vecSta;							// ��վ��Ϣ
	};

	//==============================================================================
	//
	//								CSignalCallPkt
	//						ң��������Ϣ����InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalCallPkt : public CSignalCollAllPkt
	{
	public:
		CSignalCallPkt();
		virtual ~CSignalCallPkt();
	};

	//==============================================================================
	//
	//						SOE��Ϣ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	
	//==============================================================================
	//	
	//									CSignalSOE
	//						SOE��Ϣ����ң�ŵ㡾InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSignalSOE : public CObjectBase
	{
	public:
		CSignalSOE();
		virtual ~CSignalSOE();

	public:
		void SetAddr(DWORD dwAddr);									// ����ң�ŵ�ַ			
		DWORD GetAddr() const;										// ��ȡң�ŵ�ַ

		void SetValue(BYTE byValue);								// ����ң��ֵ
		BYTE GetValue() const;										// ��ȡң��ֵ

		void SetSOETime(BYTE byYear, BYTE byMonth, BYTE byDay,		// ����SOEʱ�䣺�����ա�ʱ���롢����
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetSOETime(time_t t, WORD wMilliSec);					// ����SOEʱ�䣺time_t������
		void SetSOETime1(DWORD dwDate, DWORD dwTime);				// ����SOEʱ�䣺wDate�������գ�20160912��wTime��ʱ���룺101010
		void SetSOETime(timeval t);									// ����SOEʱ�䣺timeval
		void SetSOETime(CProtocolTime objTime);

		CProtocolTime GetProtocolSOETime() const;
		timeval GetSOETime() const;

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwAddr;												// ң�ŵ�ַ
		BYTE m_byValue;												// ң��ֵ
		CProtocolTime m_SOETime;									// SOEʱ��
	};

	//==============================================================================
	//	
	//									  CStaSOE
	//						SOE��Ϣ���г�վ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CStaSOE : public CObjectBase
	{
	public:
		CStaSOE();
		virtual ~CStaSOE();

	public:
		void SetStaId(WORD wStaIdSOE);								// ���ó�վId
		WORD GetStaId() const;										// ��ȡ��վId
		WORD GetSignalCount() const;								// ��ȡSOE����
		CSignalSOE* GetSignal(int nIndex);							// ��ȡָ���±�SOE
		void Add(CSignalSOE &signal);								// ���SOE

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;												// ��վId
		vector<CSignalSOE> m_vecSignal;								// SOE
	};

	//==============================================================================
	//	
	//										 CSOEPkt
	//						  SOE��Ϣ����InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEPkt : public CProtocolHead
	{
	public:
		CSOEPkt();
		virtual ~CSOEPkt();

	public:
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaSOE* GetSta(int nIndex);								// ��ȡָ���±공վ
		void Add(CStaSOE &sta);										// ��ӳ�վ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaSOE> m_vecSta;									// ��վ��Ϣ
	};

	//==============================================================================
	//	
	//	CSOEReplyPkt
	//	SOE��Ϣ��ִ����AppSignalMgr��InterSvr104��
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEReplyPkt : public CProtocolHead
	{
	public:
		CSOEReplyPkt();
		virtual ~CSOEReplyPkt();
	};

	//==============================================================================
	//
	//						SOE������Ϣ��AppSignalMgr��OSMģ�顿
	//
	//==============================================================================
	//==============================================================================
	//	
	//									CSignalSOESave
	//						SOE������Ϣ����ң�ŵ㡾AppSignalMgr��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CSOESave : public CObjectBase
	{
	public:
		CSOESave();
		virtual ~CSOESave();

	public:
		void SetId(DWORD dwId);										// ����ң��Id
		DWORD GetId() const;										// ��ȡң��Id
		
		void SetName(string strSet);								// ����ң������
		string GetName() const;										// ��ȡң������
		
		void SetLevel(BYTE bySet);									// ����ң�ŵ�ѹ�ȼ�
		BYTE GetLevel() const;										// ��ȡң�ŵ�ѹ�ȼ�

		void SetValue(BYTE byValue);								// ����ң��ֵ
		BYTE GetValue() const;										// ��ȡң��ֵ

		void SetRecTime(BYTE byYear, BYTE byMonth, BYTE byDay,		// ����SOE��¼ʱ�䣺�����ա�ʱ���롢����
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetRecTime(time_t t, WORD wMilliSec);					// ����SOE��¼ʱ�䣺time_t������
		void SetRecTime1(DWORD dwDate, DWORD dwTime);				// ����SOE��¼ʱ�䣺wDate�������գ�20160912��wTime��ʱ���룺101010
		void SetRecTime(timeval t);									// ����SOE��¼ʱ�䣺timeval
		void SetRecTime(CProtocolTime objTime);
		
		CProtocolTime* GetRecTime();								// ��ȡSOE��¼ʱ�䣺CProtocolTime
		timeval GetRecTime1() const;								// ��ȡSOE��¼ʱ�䣺timeval

		void SetProductTime(BYTE byYear, BYTE byMonth, BYTE byDay,	// ����SOE���Ҳ���ʱ�䣺�����ա�ʱ���롢����
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetProductTime(time_t t, WORD wMilliSec);				// ����SOE���Ҳ���ʱ�䣺time_t������
		void SetProductTime1(DWORD dwDate, DWORD dwTime);			// ����SOE���Ҳ���ʱ�䣺wDate�������գ�20160912��wTime��ʱ���룺101010
		void SetProductTime(timeval t);								// ����SOE���Ҳ���ʱ�䣺timeval
		void SetProductTime(CProtocolTime objTime);

		CProtocolTime* GetProductTime();							// ��ȡSOE���Ҳ���ʱ�䣺CProtocolTime
		timeval GetProductTime1() const;							// ��ȡSOE���Ҳ���ʱ�䣺timeval

		int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwId;												// ң��Id
		string m_strName;											// ң������
		BYTE m_byLevel;												// ң�ŵ�ѹ�ȼ�
		BYTE m_byValue;												// ң��ֵ
		CProtocolTime m_objProductTime;								// SOEʱ��
		CProtocolTime m_objRecTime;									// SOE��¼ʱ��
	};

	//==============================================================================
	//	
	//									  CStaSOESave
	//						SOE������Ϣ���г�վ��AppSignalMgr��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CStaSOESave : public CObjectBase
	{
	public:
		CStaSOESave();
		virtual ~CStaSOESave();

	public:
		void SetStaId(WORD wStaId);									// ���ó�վId
		WORD GetStaId() const;										// ��ȡ��վId
		WORD GetSoeCount() const;									// ��ȡSOE����
		CSOESave* GetSoe(int nIndex);								// ��ȡָ���±�SOE
		void Add(CSOESave &signal);									// ���SOE

		void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// ��վId
		vector<CSOESave> m_vecSoe;									// SOE
	};

	//==============================================================================
	//	
	//									 CSOESavePkt
	//						 SOE������Ϣ����AppSignalMgr��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CSOESavePkt : public CProtocolHead
	{
	public:
		CSOESavePkt();
		virtual ~CSOESavePkt();

	public:
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaSOESave* GetSta(int nIndex);							// ��ȡָ���±공վ
		void Add(CStaSOESave &sta);									// ��ӳ�վ

		void Clear();
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CStaSOESave> m_vecSta;								// ��վ��Ϣ
	};

	//==============================================================================
	//	
	//	CSOESaveReplyPkt
	//	SOE������Ϣ��ִ����OSMģ���AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CSOESaveReplyPkt : public CProtocolHead
	{
	public:
		CSOESaveReplyPkt();
	 	virtual ~CSOESaveReplyPkt();

	public:
		void SetReceivseRslt(DWORD dwRslt);							// ���ý��ս��
		void SetDesc(string strDesc);								// ��������
		
		DWORD GetReceiveRslt() const;								// ��ȡ���ս��
		string GetDesc() const;										// ��ȡ����

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwReceiveRslt;										// ���ս��
		string m_strDesc;											// ����
	};

	//==============================================================================
	//	
	//	CStaOfSOEQueryReq
	//	SOE��ѯ�����г�վ������ģ���OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CStaOfSOEQueryReq : public CObjectBase
	{
	public:
		CStaOfSOEQueryReq();
		virtual ~CStaOfSOEQueryReq();

	public:
		void SetStaId(WORD wStaId);									// ���ó�վId
		WORD GetStaId() const;										// ��ȡ��վId 

		void Add(DWORD dwSignal);									// ���ң�ŵ�
		WORD GetSignalCount() const;								// ��ȡң�ŵ�����
		DWORD GetSignal(int nIndex) const;							// ��ȡָ���±�ң�ŵ�

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;												// ��վId				
		vector<DWORD> m_vecSignal;									// ң�ŵ�
	};

	//==============================================================================
	//	
	//	CSOEQueryReqPkt
	//	SOE��ѯ�����������ģ���OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEQueryReqPkt : public CProtocolHead
	{
	public:
		CSOEQueryReqPkt();
		virtual ~CSOEQueryReqPkt();

	public:
		void SetStartTime(BYTE byYear, BYTE byMonth, BYTE byDay,	// ������ʼʱ�䣺�����ա�ʱ���롢����
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetStartTime(time_t t, WORD wMs);						// ������ʼʱ��
		void SetStartTime(timeval t);								// ������ʼʱ��

		void SetEndTime(BYTE byYear, BYTE byMonth, BYTE byDay,		// ������ֹʱ�䣺�����ա�ʱ���롢����
			BYTE byHour, BYTE byMinute, BYTE bySecond, WORD wMilliSec);
		void SetEndTime(time_t t, WORD wMs);						// ������ֹʱ��
		void SetEndTime(timeval t);									// ������ֹʱ��

		CProtocolTime GetStartTime() const;							// ��ȡ��ʼʱ��
		time_t GetStartTime_t() const;								// ��ȡ��ʼʱ��time_t
		timeval GetStartTimeval() const;							// ��ȡ��ʼʱ��timeval

		CProtocolTime GetEndTime() const;							// ��ȡ��ֹʱ��
		time_t GetEndTime_t() const;								// ��ȡ��ֹʱ��time_t
		timeval GetEndTimeval() const;								// ��ȡ��ֹʱ��timeval

		void Add(CStaOfSOEQueryReq Sta);							// �����һ��վ
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaOfSOEQueryReq* GetSta(int nIndex);						// ��ȡָ���±공վ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		CProtocolTime m_StartTime;									// ��ѯ��ʼʱ��
		CProtocolTime m_EndTime;									// ��ѯ��ֹʱ��
		vector<CStaOfSOEQueryReq> m_vecSta;							// ��վ
	};

	//==============================================================================
	//	
	//	CSOEQueryRsltPkt
	//	SOE��ѯ�������OSMģ�������ģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CSOEQueryRsltPkt : public CSOESavePkt
	{
	public:
		CSOEQueryRsltPkt();
		virtual ~CSOEQueryRsltPkt();
	};
}

#endif	// _PROTOCOL_SIGNAL_H