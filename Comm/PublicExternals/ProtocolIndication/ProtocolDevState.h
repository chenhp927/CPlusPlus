//===================================================================================================
//	
//							 ProtocolDevState.h
//							   
//begin	  : 2016-09-18
//describe: Э���豸״̬��
//
//===================================================================================================
#ifndef _PROTOCOL_DEV_STATE_H_
#define _PROTOCOL_DEV_STATE_H_

#include "TypeDef.h"
#include "ObjectBase.h"
#include "ProtocolHead.h"
#include "ProtocolTime.h"
#include "ProtocolMeasure.h"
#include <vector>

namespace ProtocolIndication
{
	using namespace ProtocolBase;

	//===============================================================================================
	//	
	//	CReqStaCommDevPkt
	//	����վͨ���豸״̬��������ģ���InterSvr104/101��				   
	//
	//===============================================================================================
	class MY_EXT_CLASS CReqStaCommDevPkt : public CProtocolHead
	{
	public:
		CReqStaCommDevPkt();
		virtual ~CReqStaCommDevPkt();

	public:
		void Add(WORD wStaId);										// ��ӳ�վId
		WORD GetStaIdCount() const;									// ��ȡ��վ����
		WORD GetStaId(int nIndex) const;							// ��ȡָ���±�ĳ�վId								

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<WORD> m_vecStaId;									// ��վId
	};

	//===============================================================================================
	//	
	//	CStaCommDev
	//	վͨ���豸״̬��InterSvr104/101������ģ�顿				   
	//
	//===============================================================================================
	class MY_EXT_CLASS CStaCommDev : public CObjectBase
	{
	public:
		CStaCommDev();
		virtual ~CStaCommDev();

		bool operator == (const CStaCommDev& objOther);

	public:
		void SetStaId(WORD wStaId);								 	// ���ó�վID
		void SetStaName(string strStaName);							// ���ó�վ����
		void SetABFlag(BYTE byABFlag);							 	// ����AB����ʶ
		void SetMainStandby1(BYTE byMainStandby);				 	// ���ó�վͨ��1����״̬
		void SetMainStandby2(BYTE byMainStandby);				 	// ���ó�վͨ��2����״̬
		void SetChannel1(BYTE byChannel1);						 	// ����վͨ��1ͨ��״̬
		void SetChn1Ip1(string strIp);								// ����վͨ��1Ip1
		void SetChn1Ip2(string strIp);								// ����վͨ��1Ip2
		void SetChannel2(BYTE byChannel2);						 	// ����վͨ��2ͨ��״̬
		void SetChn2Ip1(string strIp);								// ����վͨ��2Ip1
		void SetChn2Ip2(string strIp);								// ����վͨ��2Ip2
																 	
		void SetChannel1ChgTime(time_t t, WORD wMS);			 	// ����վͨ��1ͨ��״̬�仯ʱ��
		void SetChannel1ChgTime(timeval t);						 	// ����վͨ��1ͨ��״̬�仯ʱ��
																 	  
		void SetChannel2ChgTime(time_t t, WORD wMs);			 	// ����վͨ��2ͨ��״̬�仯ʱ��
		void SetChannel2ChgTime(timeval t);						 	// ����վͨ��2ͨ��״̬�仯ʱ��

		WORD GetStaId() const;									 	// ��ȡ��վID	
		string GetStaName() const;									// ��ȡ��վ����
		BYTE GetABFlag() const;									 	// ��ȡAB����ʶ
		BYTE GetMainStandby1() const;							 	// ��ȡ��վͨ��1����״̬
		BYTE GetMainStandby2() const;							 	// ��ȡ��վͨ��2����״̬
		BYTE GetChannel1() const;								 	// ��ȡվͨ��1ͨ��״̬
		string GetChn1Ip1() const;									// ��ȡվͨ��1Ip1
		string GetChn1Ip2() const;									// ��ȡվͨ��1Ip2
		BYTE GetChannel2() const;								 	// ��ȡվͨ��2ͨ��״̬
		string GetChn2Ip1() const;									// ��ȡվͨ��2Ip1
		string GetChn2Ip2() const;									// ��ȡվͨ��2Ip2

		CProtocolTime GetChannel1ProChgTime() const;				// ��ȡվͨ��1ͨ��״̬�仯ʱ��
		time_t GetChannel1ChgTime() const;						 	// ��ȡվͨ��1ͨ��״̬�仯ʱ��

		CProtocolTime GetChannel2ProChgTime() const;			 	// ��ȡվͨ��2ͨ��״̬�仯ʱ��
		time_t GetChannel2ChgTime() const;						 	// ��ȡվͨ��2ͨ��״̬�仯ʱ��

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;											 	// ��վID	
		string m_strStaName;										// ��վ����
		BYTE m_byABFlag;										 	// AB����ʶ
		BYTE m_byMainStandby1;									 	// ��վͨ��1����״̬
		BYTE m_byMainStandby2;									 	// ��վͨ��2����״̬
		BYTE m_byChannel1;										 	// վͨ��1ͨ��״̬
		string m_strChn1Ip1;										// վͨ��1Ip1
		string m_strChn1Ip2;										// վͨ��1Ip2
		CProtocolTime m_Channel1ChgTime;						 	// վͨ��1ͨ��״̬�仯ʱ��
		BYTE m_byChannel2;										 	// վͨ��2ͨ��״̬
		string m_strChn2Ip1;										// վͨ��2Ip1
		string m_strChn2Ip2;										// վͨ��2Ip2
		CProtocolTime m_Channel2ChgTime;						 	// վͨ��2ͨ��״̬�仯ʱ��
	};

	//===============================================================================================
	//	
	//	CStaCommDevPkt
	//	վͨ���豸״̬��������InterSvr104/101��AppStatusMgr��					   
	//
	//===============================================================================================
	class MY_EXT_CLASS CStaCommDevPkt : public CProtocolHead
	{
	public:
		CStaCommDevPkt();
	    virtual ~CStaCommDevPkt();

	public:
		void Add(CStaCommDev State);								// ��ӳ�վվͨ���豸״̬
		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaCommDev* GetSta(int nIndex);							// ��ȡָ���±공վ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CStaCommDev> m_vecSta;								// ��վ
	};

	//===============================================================================================
	//	
	//	CReqAllOperDevPkt
	//	����ȫ��ٿ��豸״̬��������̨��AppStatusMgr��			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CReqAllOperDevPkt : public CReqStaCommDevPkt
	{
	public:
		CReqAllOperDevPkt();
		virtual ~CReqAllOperDevPkt();
	};

	//==============================================================================
	//
	//	CAllOperDevMeasure
	//	ȫ��ٿ��豸״̬��ң��㡾AppMeasureMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CAllOperDevMeasure : public CObjectBase
	{
	public:
		CAllOperDevMeasure();
	    virtual ~CAllOperDevMeasure();

	public:
		void SetId(DWORD dwId);										// ����ң���Id
		void SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag);				// ����������־
		void SetValue(float fValue);								// ����ң��ֵ

		DWORD GetId() const;										// ��ȡң���Id
		ENUM_QUALITY_FLAG_TYPE GetQltFalg() const;					// ��ȡ������־
		float GetValue() const;										// ��ȡң��ֵ

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwId;												// ң���Id
		BYTE m_byQltFlag;											// ������־
		float m_fValue;												// ң��ֵ
	};

	//===============================================================================================
	//	
	//	CAllOperDevCard
	//	ȫ��ٿ��豸״̬�б�־�ơ�AppStatusMgr������̨��			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevCard : public CObjectBase
	{
	public:
		CAllOperDevCard();
		virtual ~CAllOperDevCard();

	public:
		void SetCardType(BYTE byCardType);							// ���ñ�־������
		void SetCmdCode(string strSet);								// ������ҵ������
		void SetWorkRange(string strSet);							// ������ҵ��Χ
		void SetDevId(WORD wSet);									// �����豸ID
		void SetDispatcherName(string strDispatcherName);		 	// ���õ���̨����
		void SetOperName(string strOperName);					 	// ���ò���Ա����
		void SetOperLevel(BYTE byOperLevel);						// ���ò���Ա����
		void SetPassWord(string strSet);							// ��������
		void SetQuoteTime(DWORD dwQuoteTime);					 	// ���ù���ʱ��
		void SetOffsetX(int nOffsetX);							 	// ����ƫ��λ��X
		void SetOffsetY(int nOffsetY);							 	// ����ƫ��λ��Y
		void SetContent(string strContent);						 	// ���ù�������

		BYTE GetCardType() const;								 	// ��ȡ��־������
		string GetCmdCode() const;									// ��ȡ��ҵ������
		string GetWorkRange() const;								// ��ȡ��ҵ��Χ
		WORD GetDevId() const;										// ��ȡ�豸Id
		string GetDispatcherName() const;						 	// ��ȡ����̨����
		string GetOperName() const;								 	// ��ȡ����Ա����
		BYTE   GetOperLevel() const;								// ��ȡ����Ա����
		string GetPassWord() const;									// ��ȡ����
		DWORD GetQuoteTime() const;								 	// ��ȡ����ʱ��
		int GetOffsetX() const;									 	// ��ȡƫ��λ��X
		int GetOffsetY() const;									 	// ��ȡƫ��λ��Y
		string GetContent() const;								 	// ��ȡ��������

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		BYTE m_byCardType;										 	// ��־������
		string m_strCmdCode;										// ��ҵ������
		string m_strWorkRange;										// ��ҵ��Χ
		WORD m_wDevId;												// �����豸Id
		string m_strDispatcherName;								 	// ����̨����
		string m_strOperName;									 	// ����Ա����
		BYTE   m_byOperLevel;										// ����Ա����
		string m_strPassWord;										// ����
		DWORD m_dwQuoteTime;									 	// ����ʱ��
		int m_nOffsetX;											 	// ƫ��λ��X
		int m_nOffsetY;											 	// ƫ��λ��Y
		string m_strContent;									 	// ��������
	};

	//===============================================================================================
	//	
	//	CHandSet
	//	ȫ��ٿ��豸״̬���ֶ�������Ϣ��AppStatusMgr������̨��	   
	//
	//===============================================================================================
	class MY_EXT_CLASS CHandSet : public CObjectBase
	{
	public:
		CHandSet();
		virtual ~CHandSet();

	public:
		void SetCollPointId(DWORD dwCollPointId);				 	// �����ֶ����òɼ���Id
		void SetType(ENUM_HAND_SET_TYPE byType);				 	// ������������
		void SetDispatcherName(string strDispatcherName);		 	// ���õ���̨����
		void SetOperName(string strOperName);					 	// ���ò���Ա����
		void SetTime(DWORD dwTm);								 	// ��������ʱ��
		void SetValue(float fValue);							 	// ��������ֵ

		DWORD GetCollPointId() const;							 	// ��ȡ�ֶ����òɼ���Id
		ENUM_HAND_SET_TYPE GetType() const;						 	// ��ȡ��������
		string GetDispatcherName() const;						 	// ��ȡ����̨����
		string GetOperName() const;								 	// ��ȡ����Ա����
		DWORD GetTime() const;									 	// ��ȡ����ʱ��
		float GetValue() const;									 	// ��ȡ����ֵ

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwCollPointId;									 	// �ֶ����òɼ���Id
		BYTE m_byType;											 	// ��������
		string m_strDispatcherName;								 	// ����̨����
		string m_strOperName;									 	// ����Ա����
		DWORD m_dwTime;											 	// ����ʱ��
		float m_fValue;										 		// ����ֵ
	};


	//===============================================================================================
	//	
	//	CAllOperDevInfo
	//	ȫ��ٿ��豸״̬�вٿ��豸��AppStatusMgr������̨��	   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevInfo : public CObjectBase
	{
	public:
		CAllOperDevInfo();
		virtual ~CAllOperDevInfo();

	public:
		void SetOperDevId(DWORD dwOperDevId);					 	// ���òٿ��豸Id
		void SetExpress(BYTE* pExpress);						 	// ���ñ�ʾ��Ϣ

		DWORD GetOperDevId() const;								 	// ��ȡ�ٿ��豸Id
		BYTE* GetExpress();										 	// ��ȡ��ʾ��Ϣ

		void AddMesure(CAllOperDevMeasure Measure);					 	// ���ң���
		WORD GetMeasureCount() const;							 	// ��ȡң�������
		CAllOperDevMeasure* GetMeasure(int nIndex);					 	// ��ȡָ���±�ң���

		void AddCard(CAllOperDevCard Card);						 	// ��ӱ�־��
		BYTE GetCardCount() const;								 	// ��ȡ��־������
		CAllOperDevCard* GetCard(int nIndex);					 	// ��ȡָ���±��־��

		void AddHandSet(CHandSet HandSet);						 	// ����ֶ�������Ϣ
		BYTE GetHandSetCount() const;							 	// ��ȡ�ֶ�������Ϣ����
		CHandSet* GetHandSet(int nIndex);						 	// ��ȡָ���±���ֶ�������Ϣ

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwOperDevId;									 	// �ٿ��豸Id
		BYTE m_byExpress[8];									 	// ��ʾ��Ϣ
		vector<CAllOperDevMeasure> m_vecMeasure;				 	// ң�����Ϣ
		vector<CAllOperDevCard> m_vecCard;						 	// ��־����Ϣ
		vector<CHandSet> m_vecHandSet;							 	// �ֶ�������Ϣ
	};

	//===============================================================================================
	//	
	//	CAllOperDevSta
	//	ȫ��ٿ��豸״̬�г�վ��AppStatusMgr������̨��		   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevSta : public CObjectBase
	{
	public:
		CAllOperDevSta();
		virtual ~CAllOperDevSta();

	public:
		void SetStaId(WORD wStaId);									// ���ó�վId
		WORD GetStaId() const;										// ��ȡ��վId

		void AddOperDev(CAllOperDevInfo OperDev);					// ��Ӳٿ��豸
		WORD GetOperDevCount() const;								// ��ȡ�ٿ��豸����
		CAllOperDevInfo* GetOperDev(int nIndex);					// ��ȡָ���±�Ĳٿ��豸

		virtual int StreamSize();									
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// ��վId
		vector<CAllOperDevInfo> m_vecOperDev;						// �ٿ��豸
	};

	//===============================================================================================
	//	
	//	CAllOperDevPkt
	//	ȫ��ٿ��豸״̬����AppStatusMgr������̨��			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllOperDevPkt : public CProtocolHead
	{
	public:
		CAllOperDevPkt();
		virtual ~CAllOperDevPkt();

	public:
		void AddSta(CAllOperDevSta Sta);							// ��ӳ�վ
		WORD GetStaCount() const;									// ��ȡ��վ����
		CAllOperDevSta* GetSta(int nIndex);							// ��ȡָ���±공վ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CAllOperDevSta> m_vecSta;							// ��վ
	};

	//===============================================================================================
	//	
	//	CChgOperDevPkt
	//	�仯�ٿ��豸״̬����AppStatusMgr������̨��			   
	//
	//===============================================================================================
	class MY_EXT_CLASS CChgOperDevPkt : public CAllOperDevPkt
	{
	public:
		CChgOperDevPkt();
		virtual ~CChgOperDevPkt();
	};

	//===============================================================================================
	//	
	//	CReqAllDispDevPkt
	//	����ȫ������豸״̬��������̨��AppStatusMgr��		   
	//
	//===============================================================================================
	class MY_EXT_CLASS CReqAllDispDevPkt : public CReqStaCommDevPkt
	{
	public:
		CReqAllDispDevPkt();
		virtual ~CReqAllDispDevPkt();
	};

	//===============================================================================================
	//	
	//	CAllDispDevInfo
	//	ȫ������豸״̬�е����豸��AppStatusMgr������̨��
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllDispDevInfo : public CObjectBase
	{
	public:
		CAllDispDevInfo();
		virtual ~CAllDispDevInfo();

	public:
		void SetId(DWORD dwId);										// ���õ����豸Id
		void SetValue(BYTE byValue);								// ���õ����豸״ֵ̬

		DWORD GetId() const;										// ��ȡ�����豸Id
		BYTE GetValue() const;										// ��ȡ�����豸״ֵ̬

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwId;												// �����豸Id
		BYTE m_byValue;												// �����豸״ֵ̬
	};

	//===============================================================================================
	//	
	//	CAllDispDevSta
	//	ȫ������豸״̬�г�վ��AppStatusMgr������̨��
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllDispDevSta : public CObjectBase
	{
	public:
		CAllDispDevSta();
		virtual ~CAllDispDevSta();

	public:
		void SetStaId(WORD wStaId);									// ���ó�վId
		WORD GetStaId();											// ��ȡ��վId

		void AddDispDev(CAllDispDevInfo DispDev);					// ��ӵ����豸
		WORD GetDispDevCount() const;								// ��ȡ�����豸����
		CAllDispDevInfo* GetDispDev(int nIndex);					// ��ȡָ���±�����豸

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// ��վId
		vector<CAllDispDevInfo> m_vecDispDev;						// �����豸
	};

	//===============================================================================================
	//	
	//	CAllDispDevPkt
	//	ȫ������豸״̬����AppStatusMgr������̨��
	//
	//===============================================================================================
	class MY_EXT_CLASS CAllDispDevPkt : public CProtocolHead
	{
	public:
		CAllDispDevPkt();
		virtual ~CAllDispDevPkt();

	public:
		void AddSta(CAllDispDevSta Sta);							// ��ӳ�վ
		WORD GetStaCount() const;									// ��ȡ��վ����
		CAllDispDevSta* GetSta(int nIndex);							// ��ȡָ���±공վ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CAllDispDevSta> m_vecSta;							// ��վ
	};

	//===============================================================================================
	//	
	//	CChgDispDevPkt
	//	�仯�����豸״̬����AppStatusMgr������̨��
	//
	//===============================================================================================
	class MY_EXT_CLASS CChgDispDevPkt : public CAllDispDevPkt
	{
	public:
		CChgDispDevPkt();
		virtual ~CChgDispDevPkt();
	};

	//===============================================================================================
	//	
	//	CChgDispDevFromSvrPkt
	//	�仯�����豸״̬����AppElecCalc��AppStatusMgr��
	//
	//===============================================================================================
	class MY_EXT_CLASS CChgDispDevFromSvrPkt : public CAllDispDevPkt
	{
	public:
		CChgDispDevFromSvrPkt();
		virtual ~CChgDispDevFromSvrPkt();
	};
}

#endif // !_PROTOCOL_DEV_STATE_H_