//=========================================================================================
//	
//							 ProtocolMeasure.h
//							   
// begin    : 2016-08-26
// describe : Э��ң����
//
//=========================================================================================
#ifndef _PROTOCOL_MEASURE_H
#define _PROTOCOL_MEASURE_H

#include "ProtocolTime.h"
#include "TypeDef.h"
#include "PubDefSystem.h"
#include "ProtocolHead.h"
#include <vector>
#include "ProtocolIndication.h"

namespace ProtocolIndication
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//						ȫ��ң����Ϣ��InterSvr104��AppMeasureMgr��
	//
	//==============================================================================

	//==============================================================================
	//	
	//									CMeasureCollAll
	//						ȫ��ң����Ϣ��ң��㡾InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollAll : public CObjectBase
	{
	public:
		CMeasureCollAll();
		CMeasureCollAll(BYTE byFlag, float fValue);
	    virtual ~CMeasureCollAll();

	public:
		void SetFlag(BYTE byFlag);									// ����ң��ֵ��Ч�Ա�ʶ
		BYTE GetFlag() const;										// ��ȡң��ֵ��Ч�Ա�ʶ
		void SetValue(float fValue);								// ����ң��ֵ
		float GetValue() const;										// ��ȡң��ֵ

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		BYTE m_byFlag;												// ң��ֵ��Ч�Ա�ʶ
		float m_fValue;												// ң��ֵ��4�ֽڸ�����
	};

	//==============================================================================
	//	
	//									 CMeasureStaCollAll
	//						ȫ��ң����Ϣ�г�վ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureStaCollAll : public CObjectBase
	{
	public:
		CMeasureStaCollAll();
		CMeasureStaCollAll(WORD wStaId);
	    virtual ~CMeasureStaCollAll();

	public:
		void SetId(WORD wStaId);
		WORD GetId() const;

		void SetMeaStartAddr(DWORD dwStart);
		DWORD GetMeaStartAddr() const;

		// ����ң���ȡ/�洢
		WORD GetMeasureCount() const;
		CMeasureCollAll* GetMeasure(int nIndex);
		void Add(CMeasureCollAll &measure);

		// ������վʵ��ң���ȡ/�洢
		WORD GetRealMeaCnt() const;
		string GetAllMeas();
		void SetAllMeas(string strData);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// ������վʵ��ң�����/���
		virtual int StreamSize2();
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;									// ��վId
		DWORD m_dwMeaStartAddr;							// ң����ʼ��ַ
		vector<CMeasureCollAll> m_vecMeasure;			// ң����Ϣ

		// ��վ��ʵ��ң������
		WORD m_wRealMeaCnt;
		// ��վ��ң��ֵ����
		string m_strData;
	};

	//==============================================================================
	//	
	//								  CMeasureCollAllPkt
	//						ȫ��ң����Ϣ����InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollAllPkt : public CProtocolHead
	{
	public:
		CMeasureCollAllPkt();
	    virtual ~CMeasureCollAllPkt();

	public:
		WORD GetStaCount() const;									// ��ȡ��վ����
		CMeasureStaCollAll* GetSta(int nIndex);						// ��ȡָ���±공վ��Ϣ
		void Add(CMeasureStaCollAll &sta);							// ��ӳ�վ��Ϣ
		void Clear();												// ������г�վ��Ϣ

		virtual CProtocolHead* Clone();  
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

		// ��վң�⿽����/�����ʽ
		virtual int StreamSize2();
		virtual int ToStreamBuf2(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf2(const char* pBuf, int nBufLen); 

	private:
		vector<CMeasureStaCollAll> m_vecSta;						// ��վ��Ϣ
	};

	//==============================================================================
	//
	//						�仯ң����Ϣ��InterSvr104��AppMeasureMgr��
	//
	//==============================================================================

	//==============================================================================
	//	
	//								 CMeasureCollChange
	//					�仯ң����Ϣ��ң��㡾InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollChange : public CObjectBase
	{
	public:
		CMeasureCollChange();
	    virtual ~CMeasureCollChange();

	public:
		void SetAddr(DWORD dwAddr);									// ����ң���ַ
		DWORD GetAddr() const;										// ��ȡң���ַ
		void SetFlag(BYTE byFlag);									// ����ң��ֵ��Ч�Ա�ʶ
		BYTE GetFlag() const;										// ��ȡң��ֵ��Ч�Ա�ʶ
		void SetValue(float fValue);								// ����ң��ֵ
		float GetValue() const;										// ��ȡң��ֵ

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		DWORD m_dwAddr;												// ң���ַ
		BYTE m_byFlag;												// ң��ֵ��Ч�Ա�ʶ
		float m_fValue;												// ң��ֵ��4�ֽڸ�����
	};

	//==============================================================================
	//	
	//								CMeasureStaCollChange
	//						�仯ң����Ϣ�г�վ��InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureStaCollChange : public CObjectBase
	{
	public:
		CMeasureStaCollChange();
		CMeasureStaCollChange(WORD wStaId);
	    virtual ~CMeasureStaCollChange();

	public:
		void SetId(WORD wStaId);									// ���ó�վId
		WORD GetId() const;											// ��ȡ��վId
		
		WORD GetMeasureCount() const;								// ��ȡң������
		CMeasureCollChange* GetMeasure(int nIndex);					// ��ȡָ���±��ң����Ϣ
		void Add(CMeasureCollChange &measure);						// ���ң����Ϣ

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		WORD m_wStaId;												// ��վId
		vector<CMeasureCollChange> m_vecMeasure;					// ң����Ϣ
	};

	//==============================================================================
	//	
	//									CMeasureCollChangePkt
	//							�仯ң����Ϣ����InterSvr104��AppSignalMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollChangePkt : public CProtocolHead
	{
	public:
		CMeasureCollChangePkt();
	    virtual ~CMeasureCollChangePkt();

	public:
		WORD GetStaCount() const;								// ��ȡ��վ����
		CMeasureStaCollChange* GetSta(int nIndex);				// ��ȡָ���±공վ��Ϣ
		void Add(CMeasureStaCollChange &sta);					// ��ӳ�վ��Ϣ

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<CMeasureStaCollChange> m_vecSta;					// ��վ��Ϣ
	};

	//==============================================================================
	//
	//						����ң����Ϣ��AppMeasureMgr��InterSvr104��
	//
	//==============================================================================
	//==============================================================================
	//
	//									CMeasureCollReqPkt
	//						����ң����Ϣ����AppMeasureMgr��InterSvr104��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCollReqPkt : public CProtocolHead
	{
	public:
		CMeasureCollReqPkt();
		virtual ~CMeasureCollReqPkt();

	public:
		WORD GetStaCount() const;				// ��ȡ��վ����
		WORD GetSta(int nIndex) const;			// ��ȡָ���±공վ��Ϣ
		void Add(WORD sta);						// ��ӳ�վ��Ϣ

		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen); 
		virtual int FromStreamBuf(const char* pBuf, int nBufLen); 

	private:
		vector<WORD> m_vecSta;			// ��վ��Ϣ
	};

	//==============================================================================
	//
	//									CMeasureAll
	//						ȫ��ң����Ϣ��ң��㡾AppMeasureMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureAll : public CObjectBase
	{
	public:
		CMeasureAll();
	    virtual ~CMeasureAll();

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
	
	//==============================================================================
	//
	//									CMeasureAllSta
	//						ȫ��ң����Ϣ�г�վ��AppMeasureMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureAllSta : public CObjectBase
	{
	public:
		CMeasureAllSta();
	    virtual ~CMeasureAllSta();
	
	public:
		void SetId(WORD wSId);										// ���ó�վId
		void Add(CMeasureAll Measure);								// ���ң��

		WORD GetId() const;											// ��ȡ��վId
		CMeasureAll* GetMeasure(int nIndex);						// ��ȡָ���±�ң��
		WORD GetMeasureCount() const;								// ��ȡң������

		virtual void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// ��վId
		vector<CMeasureAll> m_vecMeasure;							// ң��
	};

	//==============================================================================
	//
	//									CMeasureAllPkt
	//						ȫ��ң����Ϣ����AppMeasureMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureAllPkt : public CProtocolHead
	{
	public:
		CMeasureAllPkt();
	    virtual ~CMeasureAllPkt();

	public:
		void Add(CMeasureAllSta Sta);								// ��ӳ�վ
		CMeasureAllSta* GetSta(int nIndex);							// ��ȡָ���±공վ
		WORD GetStaCount() const;									// ��ȡ��վ����

		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CMeasureAllSta> m_vecSta;							// ��վ
	};

	//==============================================================================
	//
	//								CMeasureChangePkt
	//						�仯ң����Ϣ��AppMeasureMgr������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureChangePkt : public CMeasureAllPkt
	{
	public:
		CMeasureChangePkt();
	    virtual ~CMeasureChangePkt();
	};

	//==============================================================================
	//
	//								CMeasSave
	//						ң����Ϣ�洢����AppMeasureMgr��OSMģ�顿-ң���
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasSave : public CObjectBase
	{
	public:
		CMeasSave();
		virtual ~CMeasSave();
		
	public:
		void SetMeasId(DWORD dwId);									// ����ң���Id
		void SetSaveTime(timeval tSet);								// ���ü�¼ʱ��,��������Ĭ��ʹ�������Ķ���ʱ��
		void SetMeasName(string strSet);							// ����ң������
		void SetMeasUnit(string strSet);							// ���õ�λ
		void SetMeasType(ENUM_MEASURE_TYPE eSet);					// ����ң�������
		void SetVolLevel(BYTE bySet);								// ����ң���ѹ�ȼ�
		void SetQltFlag(ENUM_QUALITY_FLAG_TYPE eFlag);				// ����������־
		void SetValue(float fValue);								// ����ң��ֵ
		
		DWORD GetMeasId() const;									// ��ȡң���Id
		CProtocolTime* GetSaveTime();								// ��ȡ��¼ʱ��
		string GetMeasName() const;									// ��ȡң������
		string GetMeasUnit() const;									// ��ȡ��λ
		ENUM_MEASURE_TYPE GetMeasType() const;						// ��ȡң�������	
		BYTE GetVolLevel() const;									// ��ȡң���ѹ�ȼ�
		ENUM_QUALITY_FLAG_TYPE GetQltFalg() const;					// ��ȡ������־
		float GetValue() const;										// ��ȡң��ֵ
		
		int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		DWORD m_dwId;												// ң���Id
		CProtocolTime m_objTime;									// ��¼ʱ��
		string m_strName;											// ң������
		string m_strUnit;											// ��λ
		BYTE m_byMeasType;											// ң�������
		BYTE m_byLevel;												// ң���ѹ�ȼ�
		BYTE m_byQltFlag;											// ������־
		float m_fValue;												// ң��ֵ
	};
	
	
	//==============================================================================
	//
	//								CStaMeasSave
	//						ң����Ϣ�洢����AppMeasureMgr��OSMģ�顿-��վ
	//
	//==============================================================================
	class MY_EXT_CLASS CStaMeasSave : public CObjectBase
	{
	public:
		CStaMeasSave();
		virtual ~CStaMeasSave();
		
	public:
		void SetStaId(WORD wSId);									// ���ó�վId
		void Add(CMeasSave& objAdd);								// ���ң��
		
		WORD GetStaId() const;										// ��ȡ��վId
		WORD GetMeasCount() const;									// ��ȡң������
		CMeasSave* GetMeas(int nIndex);								// ��ȡָ���±�ң��
		
		virtual void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		WORD m_wStaId;												// ��վId
		vector<CMeasSave> m_vecMeasure;								// ң��
	};
	
	
	//==============================================================================
	//
	//								CMeasureSavePkt
	//						ң��ֵ��Ϣ�洢����AppMeasureMgr��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasSavePkt : public CProtocolHead
	{
	public:
		CMeasSavePkt();
		virtual ~CMeasSavePkt();
		
	public:
		void Add(CStaMeasSave& objAdd);								// ��ӳ�վ

		WORD GetStaCount() const;									// ��ȡ��վ����
		CStaMeasSave* GetSta(int nIndex);							// ��ȡָ���±공վ

		void Clear();
		
		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		vector<CStaMeasSave> m_vecSta;								// ��վ
	};

	//==============================================================================
	//
	//								CMeasLimitSave
	//						ң��Խ����Ϣ�洢����AppMeasureMgr��OSMģ�顿-Խ�޼�¼
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasLimitSave : public CObjectBase
	{
	public:
		CMeasLimitSave();
		virtual ~CMeasLimitSave();
		
	public:
		void SetStaId(WORD wSet);									// ��վId
		void SetStaName(string strSet);								// ��վ����
		void SetObjId(DWORD dwSet);									// ����Id
		void SetObjName(string strSet);								// ��������
		void SetVolLevel(BYTE bySet);								// ��ѹ�ȼ�
		void SetSaveTime(timeval tSet);								// ����ʱ��,��������Ĭ��ʹ�������Ķ���ʱ��
		void SetRecoverFlag(ENUM_LIMIT_RECOVER eSet);				// �����ָ���־
		void SetAlarmType(ENUM_ALARM_TYPE eSet);					// �����¼�����
		void SetLimitType(ENUM_LIMIT_SAVE_TYPE eSet);				// Խ������
		void SetValue(float fValue);								// Խ��ֵ
		void SetUnit(string strSet);								// ��λ
		void SetDesc(string strSet);								// ����
		
		WORD GetStaId();											// ��վId
		string GetStaName();										// ��վ����
		DWORD GetObjId();											// ����Id
		string GetObjName();										// ��������
		BYTE GetVolLevel();											// ��ѹ�ȼ�
		CProtocolTime* GetSaveTime();								// ����ʱ��,��������Ĭ��ʹ�������Ķ���ʱ��
		ENUM_LIMIT_RECOVER GetRecoverFlag();						// �����ָ���־
		ENUM_ALARM_TYPE GetAlarmType();								// �����¼�����
		ENUM_LIMIT_SAVE_TYPE GetLimitType();						// Խ������
		float GetValue();											// Խ��ֵ
		string GetUnit();											// ��λ
		string GetDesc();											// ����
		
		int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		WORD m_wStaId;												// ��վID
		string m_strStaName;										// ��վ����
		DWORD m_dwObjId;											// ����ID
		string m_strObjName;										// ��������
		BYTE m_byLevel;												// ��ѹ�ȼ�
		CProtocolTime m_objTime;									// ����ʱ��
		BYTE m_byRecoverFlag;										// �����ָ���־
		WORD m_wAlarmType;											// �����¼�����
		BYTE m_byLimitType;											// Խ������
		float m_fValue;												// Խ��ֵ
		string m_strUnit;											// ��λ
		string m_strDesc;											// ��������
	};
	//==============================================================================
	//
	//								CMeasLimitSavePkt
	//						ң��Խ����Ϣ�洢����AppMeasureMgr��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasLimitSavePkt : public CProtocolHead
	{
	public:
		CMeasLimitSavePkt();
		virtual ~CMeasLimitSavePkt();
		
	public:
		void Add(CMeasLimitSave& objAdd);							// ��Ӽ�¼
		
		WORD GetSaveCount() const;									// ��ȡ��¼����
		CMeasLimitSave* GetSave(int nIndex);						// ��ȡָ���±��¼
		
		void Clear();
		
		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		vector<CMeasLimitSave> m_vecSave;							// ��¼
	};

	//==============================================================================
	//
	//								CMeasureSaveReplyPkt
	//						ң����Ϣ�洢��ִ����OSMģ���AppMeasureMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureSaveReplyPkt : public CProtocolHead
	{
	public:
		CMeasureSaveReplyPkt();
	    virtual ~CMeasureSaveReplyPkt();

	public:
		void SetRst(DWORD dwRst);									// ���ý��ս��
		void SetDesc(string strDesc);								// ��������

		DWORD GetRst() const;										// ��ȡ���ս��
		string GetDesc() const;										// ��ȡ����

		CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		DWORD m_dwRst;												// ���ս��
		string m_strDesc;											// ����
	};

	//==============================================================================
	//
	//								CMeasureHis
	//						ң����ʷ��Ϣ��ѯ�г�վ������̨��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureHis : public CObjectBase
	{
	public:
		CMeasureHis();
	    virtual ~CMeasureHis();

	public:
		void SetStaId(WORD wStaId);									// ���ó�վId
		WORD GetStaId() const;										// ��ȡ��վId

		void Add(DWORD dwMeasure);									// ���ң���
		WORD GetMeasureCount() const;								// ��ȡң������
		DWORD GetMeasure(int nIndex) const;							// ��ȡָ���±�ң���

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wStaId;												// ��վId
		vector<DWORD> m_vecMessure;									// ң���Id
	};

	//==============================================================================
	//
	//								CMeasureHisPkt
	//						ң����ʷ��Ϣ��ѯ��������̨��OSMģ�顿
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureHisPkt : public CProtocolHead
	{
	public:
		CMeasureHisPkt();
	    virtual ~CMeasureHisPkt();

	public:
		void SetStartTime(time_t t, WORD wMilliSec);				// ������ʼʱ��
		void SetEndTime(time_t t, WORD wMilliSec);					// ������ֹʱ��

		timeval GetStartTime() const;								// ��ȡ��ʼʱ��
		timeval GetEndTime() const;									// ��ȡ��ֹʱ��

		void Add(CMeasureHis His);									// ��ӳ�վ
		CMeasureHis* GetSta(int nIndex);							// ��ȡָ���±�ĳ�վ
		WORD GetStaCount() const;									// ��ȡ��վ����

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		virtual ProtocolBase::CProtocolHead* Clone();
	private:
		CProtocolTime m_StartTime;									// ��ѯ��ʼʱ��
		CProtocolTime m_EndTime;									// ��ѯ��ֹʱ��
		vector<CMeasureHis> m_vecSta;								// ��վ
	};

	//==============================================================================
	//
	//								CMeasureHisReplyPkt
	//						ң����Ϣ��ѯ��ִ����OSMģ�������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureHisReplyPkt : public CMeasSavePkt
	{
	public:
		CMeasureHisReplyPkt();
	    virtual ~CMeasureHisReplyPkt();
	};

	//==============================================================================
	//
	//								CMeasureReqPkt
	//						����ң����Ϣ��������̨��AppMeasureMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureReqPkt : public CMeasureCollReqPkt
	{
	public:
		CMeasureReqPkt();
	    virtual ~CMeasureReqPkt();
	};

	//==============================================================================
	//
	//								CMeasureCallPkt
	//						ң��������Ϣ����InterSvr104/101��AppMeasureMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureCallPkt : public CMeasureCollAllPkt
	{
	public:
		CMeasureCallPkt();
		virtual ~CMeasureCallPkt();
	};
	
	//==============================================================================
	//
	//								CMeasureQueryFailRepPkt
	//						ң����Ϣ��ѯʧ�ܻ�ִ����OSMģ�������̨��
	//
	//==============================================================================
	class MY_EXT_CLASS CMeasureQueryFailRepPkt : public CMeasureSaveReplyPkt
	{
	public:
		CMeasureQueryFailRepPkt();
		virtual ~CMeasureQueryFailRepPkt();
	};
}

#endif	// _PROTOCOL_MEASURE_H