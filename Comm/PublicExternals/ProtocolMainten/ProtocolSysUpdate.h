//=========================================================================================
//
//							 ProtocolSysUpdate.h
// begin    : 2016-12-27
// describe : ϵͳ����Э����
//
//=========================================================================================
#ifndef _PROTOCOLSYSUPDATE_H_
#define _PROTOCOLSYSUPDATE_H_

#include "ProtocolHead.h"
#include "ProtocolTime.h"
#include "ProtocolBase.h"
#include "ObjectBase.h"
#include "TypeDef.h"
#include <vector>

namespace ProtocolSysUpdate
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//             CReqSysCurVerPkt
	//             ����������ǰ������ݰ汾��RmControlMonitor��RmInterHost��
	//
	//==============================================================================
	class MY_EXT_CLASS CReqSysCurVerPkt : public CProtocolHead
	{
	public:
		CReqSysCurVerPkt();
		~CReqSysCurVerPkt();
		
	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;		
		// ����·��
		void AddRoute(string strRoute);
		// ��ȡ·��
		string GetRoute(int nIndex) const;
		// ·������
		int GetRouteCount() const;

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������
		string m_strHostName;
		// ·���ṹ
		vector<string> m_vecRoute;
	};

	//==============================================================================
	//
	//								CRouteVerInfo
	//             ����������ݵ�ǰ�汾��Ϣ��RmInterHost ��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteVerInfo : public CObjectBase
	{
	public:
		CRouteVerInfo();
		~CRouteVerInfo();

	public:		
		void SetRouteName(string strRouteName);	// ����·������		
		string GetRouteName() const;			// ��ȡ·������

		void SetVerID(DWORD dwVerID);			// ���ð汾��
		DWORD GetVerID() const;					// ��ȡ�汾��

		void SetLogInfo(string strLog);			// ������־��Ϣ
		string GetLogInfo() const;				// ��ȡ��־��Ϣ
		
		void SetTime(time_t tm, WORD wMs);		// ���÷���ʱ�䣬wMs�����루0~999��
		void SetTime(timeval t);
		CProtocolTime GetProCommitTime() const;	// ��ȡȷ��ʱ��
		timeval GetCommitTime() const;			// ��ȡȷ��ʱ��

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string	m_strRouteName;		// ·������
		DWORD	m_dwVerID;			// �汾��
		string	m_strLogInfo;		// ��־��Ϣ
		CProtocolTime m_Time;		// ���»��ύʱ��
	};

	//==============================================================================
	//
	//								CSysCurVerInfoPkt
	//             ����������ݵ�ǰ�汾��Ϣ��RmInterHost ��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CSysCurVerInfoPkt : public CProtocolHead
	{
	public:
		CSysCurVerInfoPkt();
		~CSysCurVerInfoPkt();
		
	public:
		void SetHostName(string strHostName);	// ������������		
		string GetHostName() const;				// ��ȡ��������

		void AddRoute(CRouteVerInfo& objRouteVerInfo); // ����·����Ϣ
		CRouteVerInfo* GetRoute(int nIndex);		   // ��ȡ·����Ϣ		
		int GetRouteCount();						   // ��ȡ·������
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		string m_strHostName; // ��������
		vector<CRouteVerInfo> m_vecRouteInfo; // ·����Ϣ
	};
	

	//==============================================================================
	//
	//						CRouteUpdateInfo
	//             ��������������ݰ汾��RmControlMonitor��RmInterHost��
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteUpdateInfo : public CObjectBase
	{
	public:
		CRouteUpdateInfo();
		~CRouteUpdateInfo();
	public:
		void SetRouteName(string strRouteName);	// ����·������
		string GetRouteName() const;			// ��ȡ·������

		void SetVerID(DWORD dwVeID);			// ���ð汾��
		DWORD GetVerID() const;					// ��ȡ�汾��

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	private:
		string	m_strRouteName;	// ·������
		DWORD	m_dwVerID;		// �汾��
	};

	//==============================================================================
	//
	//						CHostUpdateInfo
	//             ��������������ݰ汾��RmControlMonitor��RmInterHost��������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CHostUpdateInfo : public CObjectBase
	{
	public:
		CHostUpdateInfo();
		~CHostUpdateInfo();
		
	public:
		void SetHostName(string strHostName); // ������������
		string GetHostName() const;			  // ��ȡ��������

		void AddRouteUpdateInfo(CRouteUpdateInfo objRouteUpdateInfo); // ����·��������Ϣ
		CRouteUpdateInfo*  GetRouteUpdateInfo(int nIndex);  // ��ȡ·��������Ϣ	
		int GetRouteCount() const;							// ��ȡ·������

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		string m_strHostName;	//��������
		vector<CRouteUpdateInfo> m_vecRouteUpdateInfo; // ·��������Ϣ
	};
	
	//==============================================================================
	//
	//						CSysUpdatePkt
	//             ��������������ݰ汾��RmControlMonitor��RmInterHost��
	//
	//==============================================================================
	class MY_EXT_CLASS CSysUpdatePkt : public CProtocolHead
	{
	public:
		CSysUpdatePkt();
		~CSysUpdatePkt();

	public:
		void SetUpdatePara(BYTE byUpdatePara);						// ���ø��²���
		BYTE GetUpdatePara() const;									// ��ȡ���²���

		void AddHostUpdateInfo(CHostUpdateInfo& objHostUpdateInfo);	// ��������������Ϣ
		CHostUpdateInfo*  GetHostUpdateInfo(int nIndex);			// ��ȡ����������Ϣ		
		int GetHostCount() const;									// ��ȡ��������

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:

		BYTE m_byUpdatePara;							//���²���
		vector<CHostUpdateInfo> m_vecHostUpdateInfo;	// ������Ϣ
	};

	//==============================================================================
	//
	//						CRouteUpdateRep
	//             ��������������ݰ汾��ִ��RmInterHost��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteUpdateRep : public CObjectBase
	{
	public:
		CRouteUpdateRep();
		~CRouteUpdateRep();

	public:		
		void SetRouteName(string strRouteName); // ����·������
		string GetRouteName() const;		    // ��ȡ·������

		void SetRsltValue(BYTE byRsltValue);	// ���ø��½��
		BYTE GetRsltValue() const;				// ��ȡ���½��

		void SetVerID(DWORD dwVerID);			// ���ð汾��
		DWORD GetVerID() const;					// ��ȡ�汾��
		

		void SetRsltString(string strRslt);		// ���û�ִ����
		string  GetRsltString() const;			// ��ȡ��ִ����

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:		
		string	m_strRouteName;		// ·������
		BYTE	m_byRslt;			// ���½��
		DWORD	m_dwVerID;			// �汾��
		string  m_strRslt;			// ��ִ���� 
	};

	//==============================================================================
	//
	//							CHostUpdateRepPkt
	//             ��������������ݰ汾��ִ��RmInterHost��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CHostUpdateRepPkt : public CProtocolHead
	{
	public:
		CHostUpdateRepPkt();
		~CHostUpdateRepPkt();
		
	public:		
		void SetHostName(string strHostName); // ����������	
		string GetHostName() const;			  // ��ȡ������	
		
		CRouteUpdateRep* GetRouteUpdateReq(int nIndex);				// ��ȡָ���±��·�����»�ִ		
		void AddRouteUpdateRep(const CRouteUpdateRep& objRouteUdateRep);	// ���·�����»�ִ
		int GetRouteCount();												// ·������
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:		
		string m_strHostName; // ������		
		vector<CRouteUpdateRep> m_vecRouteUpdateRep;// ·�����»�ִ��Ϣ
	};

	
	
	//==============================================================================
	//
	//							CVerInfo
	//          �洢����������ݰ汾��Ϣ��RmControlMonitor���������ݴ�ȡ����
	//
	//==============================================================================
	class MY_EXT_CLASS CVerInfo : public CObjectBase
	{
	public:
		CVerInfo();
		~CVerInfo();

	public:
		void SetVerID(DWORD dwVerID); // ���ð汾��
		DWORD GetVerID() const;		  // ��ȡ�汾��

		void SetUpdateTime(time_t tm, WORD wMs);	// ���ø���ʱ�䣬wMs�����루0~999��
		void SetUpdateTime(timeval t);				// ���ø���ʱ��
		CProtocolTime GetUpateTime() const;				// ��ȡ����ʱ�䣬����ProtocolTime����
		timeval GetUpdateTimeVal() const;					// ��ȡ����ʱ�䣬����timeval����

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		DWORD m_dwVerID;			// �汾��
		CProtocolTime m_tmUpdate;	// ����ʱ��		

	};

	//==============================================================================
	//
	//							CRouteHisVerInfo
	//          �洢����������ݰ汾��Ϣ��RmControlMonitor���������ݴ�ȡ����
	//
	//==============================================================================
	class MY_EXT_CLASS CRouteHisVerInfo : public CObjectBase
	{
	public:
		CRouteHisVerInfo();
		~CRouteHisVerInfo();

	public:
		void SetRouteName(string strRouteName); // ����·������
		string GetRouteName() const;		    // ��ȡ·������

		void AddVerInfo(CVerInfo objVerInfo);	// ������ʷ�汾
		CVerInfo* GetVerInfo(int nIndex);		// ��ʷ�汾����
		int GetVerCount();						// �汾����
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		string m_strRouteName;					// ·������
		vector<CVerInfo> m_vecHisVerInfo;		// ��ʷ�汾

	};

	//==============================================================================
	//
	//							CSaveHostVerPkt
	//				�洢����������ݰ汾��Ϣ��RmControlMonitor���������ݴ�ȡ����
	//
	//==============================================================================
	class MY_EXT_CLASS CSaveHostVerPkt : public CProtocolHead
	{
	public:
		CSaveHostVerPkt();
		~CSaveHostVerPkt();
	public:
		void SetHostName(string strHostName); // ����������	
		string GetHostName() const;			  // ��ȡ������	
		
		void AddRouteInfo(const CRouteHisVerInfo objRouteHisVerInfo);	// ���·����Ϣ
		CRouteHisVerInfo* GetRouteInfo(int nIndex);			// ��ȡ·����Ϣ
		int GetRouteCount();												// ·������
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		string m_strHostName; // ������				
		vector<CRouteHisVerInfo> m_vecRouteHisVer;
	};

	//==============================================================================
	//
	//							CSaveHostVerRepPkt
	//				����������ݰ汾�洢��ִ���������ݴ�ȡ�����RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CSaveHostVerRepPkt : public CProtocolHead
	{
	public:
		CSaveHostVerRepPkt();
		~CSaveHostVerRepPkt();
	public:
		void SetRslt(DWORD dwRslt);						// ���ý��
		DWORD GetRslt() const;							// ��ȡ·����Ϣ

		void SetRsn(string strRsn);						// ����ʧ��ԭ��
		string GetRsn() const;							// ��ȡʧ��ԭ��

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
	protected:
		DWORD m_dwSaveRslt;		// ������
		string m_strRsn;		// ʧ��ԭ������
	};


	//==============================================================================
	//
	//				            CReqHostHisVerPkt
	//            �����������������ʷ�汾��Ϣ��RmControlMonitor���������ݴ�ȡ����
	//
	//==============================================================================
	class MY_EXT_CLASS CReqHostHisVerPkt : public CProtocolHead
	{
	public:
		CReqHostHisVerPkt();
		~CReqHostHisVerPkt();

	public:			
		void SetHostName(string strHostName);		// ������������
		string GetHostName() const;					// ��ȡ��������

		string GetRouteName(int nIndex) const;		// ��ȡָ���±��·������		
		void AddRouteName(string& strRouteName);	// ���·������

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strHostName;				// ������
		vector<string> m_vecRouteName;		// ����·����Ϣ
	};

	//==============================================================================
	//
	//							CHostHisVerInfoPkt
	//          �������������ʷ�汾��Ϣ���������ݴ�ȡ�����RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CHostHisVerInfoPkt : public CSaveHostVerPkt
	{
	public:
		CHostHisVerInfoPkt();
		~CHostHisVerInfoPkt();
	};
	
}

#endif
