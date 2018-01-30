//=========================================================================================
//
//							 ProtocolMaintenComm.h
// begin    : 2016-12-16
// describe : ϵͳ״̬��ϢЭ�鼯��������״̬���֣���ͨѶƽ̨ʹ��
//
//=========================================================================================
#ifndef _PROTOCOLMAINTENCOMM_H_2016_12_20
#define _PROTOCOLMAINTENCOMM_H_2016_12_20

#include <vector>
#include "TypeDef.h"
#include "ProtocolBase.h"
#include "ProtocolHead.h"
#include "ProtocolForward.h"

typedef CProtocolForward::CForwardAddr CForwardAddr;

namespace ProtocolMaintenContent
{
	using namespace std;
	using namespace ProtocolBase;

	//==============================================================================
	//
	//             CDeviceInfo
	//             �豸��Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CDeviceInfo : public CObjectBase
	{
	public:
		CDeviceInfo();
		~CDeviceInfo();

		bool operator == (const CDeviceInfo& objOther);

	public:
		// ����·��Id
		void SetBureauId(BYTE byBureauId);
		// ��ȡ·��Id
		BYTE GetBureauId() const;

		// ���õ�λ����
		void SetUnitType(BYTE byUnitType);
		// ��ȡ��λ����
		BYTE GetUnitType() const;

		// ���õ�λId
		void SetUnitId(WORD wUnitId);
		// ��ȡ��λId
		WORD GetUnitId() const;

		// �����豸����
		void SetDeviceType(BYTE byDeviceType);
		// ��ȡ�豸����
		BYTE GetDeviceType() const;

		// �����豸Id
		void SetDeviceId(WORD wDeviceId);
		// ��ȡ�豸Id
		WORD GetDeviceId() const;

		// ����AB����ʶ
		void SetAbFlag(BYTE byAb);
		// ��ȡAB����ʶ
		BYTE GetAbFlag() const;

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		string GetDumpStr();

	private:
		BYTE m_byBureauId;				// ·��Id
		BYTE m_byUnitType;				// ��λ����
		WORD m_wUnitId;					// ��λId
		BYTE m_byDeviceType;			// �豸����
		WORD m_wDeviceId;				// �豸Id
		BYTE m_byAbFlag;				// Ab����ʶ
	};

	//==============================================================================
	//
	//			CConnState
	//			�ͻ�������״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CConnState : public CObjectBase
	{
	public:
		CConnState();
		~CConnState();

		bool operator == (const CConnState& objOther);

	public:
		//ip1��ַ
		void SetIP1(const string& strIP1);
		string GetIP1() const ;
		//����1�˿�
		void SetNet1Port(WORD wNet1Port);
		WORD GetNet1Port() const;
		//����1����״̬
		void SetNet1Conn(BYTE byNet1Conn);
		BYTE GetNet1Conn() const;
		//����1����״̬�ı�ʱ��
		void SetNet1ConnStaChgTime(time_t& tm, WORD wMs);	//	wMs�����루0~999��
		void SetNet1ConnStaChgTime(const CProtocolTime& objChgTime);
		CProtocolTime GetNet1ConnStaChgTime() const;

		//ip2��ַ
		void SetIP2(const string& strIP2);
		string GetIP2() const ;
		//����2�˿�
		void SetNet2Port(WORD wNet2Port);
		WORD GetNet2Port() const;
		//����2����״̬
		void SetNet2Conn(BYTE byNet2Conn);
		BYTE GetNet2Conn() const;
		//����2����״̬�ı�ʱ��
		void SetNet2ConnStaChgTime(time_t& tm, WORD wMs);	//	wMs�����루0~999��
		void SetNet2ConnStaChgTime(const CProtocolTime& objChgTime);
		CProtocolTime GetNet2ConnStaChgTime() const;

		//AF_UNIX����״̬
		void SetAF_UNIXState(BYTE BYAF_UNIXState);
		BYTE GetAF_UNIXState() const;

		// AF_UNIX����״̬�ı�ʱ��
		void SetAF_UNIXChgTime(time_t& tm, WORD wMs);	//	wMs�����루0~999��
		void SetAF_UNIXChgTime(const CProtocolTime& objChgTime);
		CProtocolTime GetAF_UNIXChgTime() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strIP1;					// IP1��ַ
		WORD m_wNet1Port;					// ����1�˿�
		BYTE m_byNet1Conn;					// ����1����״̬
		CProtocolTime m_Net1ConnStaChgTime;	// ����1����״̬�ı�ʱ��
		string m_strIP2;					// IP2��ַ
		WORD m_wNet2Port;					// ����2�˿�
		BYTE m_byNet2Conn;					// ����2����״̬
		CProtocolTime m_Net2ConnStaChgTime;	// ����2����״̬�ı�ʱ��
		BYTE m_byAF_UNIXState;				// AF_UNIX����״̬
		CProtocolTime m_AF_UNIXChgTime;		// AF_UNIX����״̬�ı�ʱ��
	};

	//==============================================================================
	//
	//			CClientConnMainCommtoSys
	//			�ͻ���������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CClientConnMainCommtoSys : public CObjectBase
	{
	public:
		CClientConnMainCommtoSys();
		~CClientConnMainCommtoSys();

	public:
		// ���ÿͻ����豸��Ϣ�Ϳͻ���ID
		void SetForwardAddr(const CForwardAddr& objForwardAddr);
		// ��ȡ�ͻ����豸��Ϣ�Ϳͻ���ID
		CForwardAddr* GetForwardAddr();
		// ���ÿͻ�������״̬
		void SetConnState(const CConnState& objConnState);
		// ��ȡ�ͻ�������״̬
		CConnState* GetConnState();

	public:
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		CForwardAddr m_objForwardAddr;		// ���� ��λ���� ��λid �豸���� �豸ID �ͻ���id
		CConnState m_objConnState;			// ����״̬
	};

	//==============================================================================
	//
	//             CConnInfo
	//             ����������ӵ�ַ��Ϣ��
	//
	//==============================================================================
	class MY_EXT_CLASS CConnInfo : public CObjectBase
	{
	public:
		CConnInfo();
		~CConnInfo();

		bool operator != (const CConnInfo& objOther);

	public:
		// ��ȡ��������
		BYTE GetConnType() const;
		// ������������
		void SetConnType(BYTE byType);

		// ��ȡIp1��ַ
		string GetIp1() const;
		// ����Ip1��ַ
		void SetIp1(const string& strIp);

		// ��ȡ����1�˿�
		WORD GetNet1Port() const;
		// ��������1�˿�
		void SetNet1Port(WORD wPort);

		// ��ȡIp2��ַ
		string GetIp2() const;
		// ����Ip2��ַ
		void SetIp2(const string& strIp);

		// ��ȡ����2�˿�
		WORD GetNet2Port() const;
		// ��������2�˿�
		void SetNet2Port(WORD wPort);

		// ��ȡAF_UNIX����·��
		string GetConnPath() const;
		// ����AF_UNIX����·��
		void SetConnPath(const string& strPath);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		string GetDumpStr();

	private:
		// ��������
		BYTE m_byConnType;			
		//IP1��ַ
		string m_strIP1;
		//����1�˿�
		WORD m_wNet1Port;
		//IP2��ַ
		string m_strIP2;
		//����2�˿�
		WORD m_wNet2Port;
		// AF_UNIX����·��
		string m_strConnPath;
	};

	//==============================================================================
	//
	//             CProgramServer
	//             ���������Ϣ��
	//
	//==============================================================================
	class MY_EXT_CLASS CProgramServer : public CObjectBase
	{
	public:
		CProgramServer();
		~CProgramServer();

		bool operator == (const CProgramServer& objOther);

	public:
		// ��ȡ�����Id
		WORD GetSvcID() const;
		// ���÷����Id
		void SetSvcID(WORD wId);

		static int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		WORD m_wSvcID;					// �����Id
	};

	//==============================================================================
	//
	//			CSvrConnState
	//			������̵�����״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CSvrConnState : public CObjectBase
	{
	public:
		CSvrConnState();
		~CSvrConnState();

	public:
		// ���÷���˳�����Ϣ
		void SetProgramServer(CProgramServer& objSvc);
		// ��ȡ����˳�����Ϣ
		CProgramServer* GetProgramServer();

		// ���÷����������Ϣ
		void SetConnInfo(CConnInfo& objConnInfo);
		// ��ȡ�����������Ϣ
		CConnInfo* GetConnInfo();
		
		// ��տͻ�����Ϣ
		void Clear();
		// ��ӿͻ�����Ϣ
		void AddClient(const CClientConnMainCommtoSys& objClient);
		// ��ÿͻ�������
		WORD GetClientCount() const;
		// ���ָ���±�Ŀͻ�����Ϣ
		CClientConnMainCommtoSys* GetClient(int nIndex);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

		vector<CClientConnMainCommtoSys>& GetClientVector();

	private:
		CProgramServer m_objSvc;
		CConnInfo m_objConnInfo;
		vector<CClientConnMainCommtoSys> m_vecClientConnMainCommtoSys;
	};

	//==============================================================================
	//
	//			CMainSysSvcConnStatusPkt
	//			��ϵͳ��������״̬����ϵͳͨ��ƽ̨��RmInterSys��
	//
	//==============================================================================
	class MY_EXT_CLASS CMainSysSvcConnStatusPkt : public CProtocolHead
	{
	public:
		CMainSysSvcConnStatusPkt();
		~CMainSysSvcConnStatusPkt();

	public:
		// ���÷������ĵ�ַ��Ϣ
		void SetDeviceInfo(const CDeviceInfo& objDeviceInfo);
		// ��ȡ�������ĵ�ַ��Ϣ
		CDeviceInfo* GetDeviceInfo();
		// ���÷��������״̬
		void SetSvrConnState(const CSvrConnState& objSvrConnState);
		// ��ȡ���������״̬
		CSvrConnState* GetSvrConnState();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ����\��λ����\��λID\�豸����\�豸ID\AB����ʶ
		CDeviceInfo m_objDeviceInfo;
		// ��������״̬
		CSvrConnState m_objSvrConnSt;
	};
}

#endif	// !_PROTOCOLMAINTENCOMM_H_2016_12_20
