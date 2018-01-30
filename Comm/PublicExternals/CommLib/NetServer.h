//==============================================================================
//                      NetServer.h
//                       
//begin   : 2016-09-08                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_NETSERVER_H 
#define SELFDEF_NETSERVER_H 

#include "ServerForwardBase.h"
#include "ObjectBase.h"

//==============================================================================
//                 
//                    CServerOneNet
//                    һ�����������
//
//==============================================================================
class CNetServerDouble;
class CServerOneNetCltObj;
class MY_EXT_CLASS CServerOneNet : public CServerForwardBase
{ 
public: 
	CServerOneNet(CNetServerDouble* pParent);
	virtual ~CServerOneNet();
	
protected: 
	/* ���е�ʵ��Ϊ��������ַ��ʼ������ش��� */
	// ���ػ��ຯ��
	virtual string GetServerIdValidRangeStr();
	
	// ���ػ��ຯ��
	virtual bool IsServerIdValid();
	
	/* ���е�ʵ��Ϊ����ͻ������ӽ������رյ���ش��� */
	// ���ػ��ຯ��
	virtual void OnSessionConnect(CSid objSid); 

	// ���ػ��ຯ��
	virtual void OnSessionReady(CSid objSid); 

	// ���ػ��ຯ��
	virtual void OnSessionClose(CSid objSid); 

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ���ػ��ຯ��
	// ������յ��û����ݰ���
	// �ش�����������д���
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ
	virtual void HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf);

	/* ���е�ʵ��Ϊ����ͻ��˶������ش��� */
	// ���ػ��ຯ��
	virtual CServerBaseCltObj* CreateClientObject();
	
	// ���ػ��ຯ��
	virtual string GetCliendIdValidRangeStr();

	// ���ػ��ຯ��
	virtual bool IsClientIdValid(CServerSessionBaseCltObj* pCltObj);
	
protected:
	CNetServerDouble* m_pParent;
};

//==============================================================================
//                 
//                    CServerOneNetCltObj
//                  һ��������˵Ŀͻ��˶�����
//
//==============================================================================
class MY_EXT_CLASS CServerOneNetCltObj : public CServerForwardBaseCltObj
{ 
public: 
	CServerOneNetCltObj(CServerOneNet* pServer);
	virtual ~CServerOneNetCltObj();

};

//==============================================================================
//                 
//                   CNetServerDouble
//                    ˫���������
//
//==============================================================================
class CNetServerDoubleCltObj;
class MY_EXT_CLASS CNetServerDouble : public CObjectBase
{ 
public: 
	CNetServerDouble();
	virtual ~CNetServerDouble();

protected:
	// �ú����ڸ��๹���Ҫ�����ֶ�����һ�Σ����Կ������캯����һ���֣�
	// Ŀ����Ϊ���ڹ��������ִ���麯������Ϊ���캯����Ҫ��������麯����
	bool Create();
	
protected:
	/* ���е�ʵ��Ϊ����ͻ��˶������ش��� */
	void ConvertCltObjs(vector<int>& vecCltIndex, vector<CServerBaseCltObj*>& vecCltObj1, vector<CServerBaseCltObj*>& vecCltObj2);
	CNetServerDoubleCltObj* GetNetServerDoubleCltObj(CServerOneNetCltObj* pCltObj);

	/* ���е�ʵ��Ϊ�����û����ݰ�����ش��� */
	// ���������ʵ�֡�
	// ������յ��û����ݰ���
	// ���½ӿڴ����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ
	virtual void OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf) = 0;
	
public:
	/* ���еĽӿ�Ϊ���������ʼ������ش��� */
	// ��ʼ�����������
	bool InitCommParam(TiXmlHandle& txhServer);
	virtual string GetServerIdValidRangeStr(bool bIsAppSvr);
	virtual bool IsServerIdValid(bool bIsAppSvr, WORD wSvrId);
	bool IsRunAsSingleNet(); 
	int GetType();
	
	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	bool Startup();
	virtual void OnSessionConnect(CServerOneNetCltObj* pCltObj); 
	virtual void OnSessionReady(CServerOneNetCltObj* pCltObj); 
	virtual void OnSessionClose(CServerOneNetCltObj* pCltObj); 

	/* ���еĽӿ�Ϊͨ��select���ƻ�ȡ������Ϣ����ش��� */
	void SetFdset(CFdset& objFs);
	void SetListenFdset(CFdset& objFs);
	void SetClientFdset(CFdset& objFs);
	void SetClientFdset(CFdset& objFs, vector<int>& vecCltIndex);
	void HandleNetEvent(CFdset& objFs);
	void HandleListenNetEvent(CFdset& objFs);
	void HandleClientNetEvent(CFdset& objFs);
	void HandleClientNetEvent(CFdset& objFs, vector<int>& vecCltIndex);

	/* ���еĽӿ�Ϊ�������ݵ���ش��� */
	// ������յ��û����ݰ���
	// ����˫����š�ע�����
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void OnRecvPacket(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	void HandleRegisterPkt(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	void HandleForwardPkt(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	
	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��szCltIP��nCltPort��objSid��pCltObj��vecDestAddr��Ϊ���������
	bool SendData(const char* szCltIP, unsigned short nCltPort, char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr); 
	bool SendData(const char* szCltIP, unsigned short nCltPort, CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr); 
	bool SendData(const char* szCltIP, unsigned short nCltPort, char* szBuf, int nBufLen); 
	bool SendData(const char* szCltIP, unsigned short nCltPort, CBuffer* pBuf); 

	/* ���еĽӿ�ΪѲ�����ش��� */
	// �����Ե��ã���������Ѳ�������������Ͷ����е����ݰ����ͳ�ȥ
	void DoPatrol();
	void DoPatrol(vector<int>& vecCltIndex);

	/* ���еĽӿ�Ϊ����ͻ��˶�����Ϣ��ȡ���������ش��� */
	virtual string GetCliendIdValidRangeStr();
	virtual bool IsClientIdValid(bool bIsAppClt, WORD wCltId);

	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ����־�д�ӡ��������
	string GetDumpParamStr();
	void DumpParam();
	virtual void DumpAllocator();
	
	/* ���еĽӿ�Ϊ��־�ļ���������ش��� */
	// ���ػ��ຯ��
	// ������־�ļ��ı�ʶ
	virtual void SetLogFileTag(const char* szFileTag);
	
protected:
	// ��װ���������
	CServerOneNet* m_pServer1;
	CServerOneNet* m_pServer2;

	// ˫���ͻ��˶��󼯺ϣ�һ����˵���Ǹ���̬�����鼯��
	vector<CNetServerDoubleCltObj> m_vecNetServerDoubleCltObj;
};

//==============================================================================
//                 
//                   CNetServerDoubleCltObj
//                 ˫������˵�˫���ͻ��˶�����
//
//==============================================================================
class MY_EXT_CLASS CNetServerDoubleCltObj : public CObjectBase
{ 
public: 
	CNetServerDoubleCltObj();
	virtual ~CNetServerDoubleCltObj();

protected:
	bool DealSingleRecvSeq(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	bool DealDoubleRecvSeq(CServerOneNetCltObj* pCltObj);
	
public: 
	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	// cNetNo���������ţ�1�����һ������2����ڶ�������-1�������ܵ����
	virtual int GetClientId(char cNetNo = 1);
	virtual string GetPeerIp(char cNetNo = 1);
	virtual int GetPeerPort(char cNetNo = 1);
	virtual CCommAddr* GetClientAddr(char cNetNo = -1);
	virtual string GetTagCltInfo(char cNetNo = -1);
	virtual string GetTagConnInfo(char cNetNo = -1);
	
	/* ���еĽӿ�Ϊ�ͻ���ע����Ϣ��ȡ����ش��� */
	// cNetNo���������ţ�1�����һ������2����ڶ�������-1�������ܵ����
	virtual CForwardCondition* GetForwardCondition(char cNetNo = -1);
	virtual bool IsRegistered(CProtocolForward::CForwardHead& objForwardHead);

	/* ���еĽӿ�Ϊ�ͻ������������������رա��ж�����ش��� */
	// cNetNo���������ţ�1�����һ������2����ڶ�������-1�������ܵ����
	virtual bool IsReady(char cNetNo = -1);	

	/* ���еĽӿ�Ϊ�������ݰ�����ش��� */
	// ��Ӧ���ݰ������¼�������˫�����к���Ϣ��
	bool DealRecvPacketDataSeq(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	
	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��vecDestAddr��Ϊ���������
	bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr); 

	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ���нӿ��õ���pBuf��Ϊ���������
	bool SendRawData(CBuffer* pBuf); 

	/* ���еĽӿ�Ϊ�ͻ��˶���ָ��������ش��� */
	CServerOneNetCltObj* GetRelatedCltObjPtr1();
	void SetRelatedCltObjPtr1(CServerOneNetCltObj* p);
	CServerOneNetCltObj* GetRelatedCltObjPtr2();
	void SetRelatedCltObjPtr2(CServerOneNetCltObj* p);
	bool IsEmpty();
	bool IsFull();
	bool AddCltObj(CServerOneNetCltObj* pCltObj);
	bool RemoveCltObj(CServerOneNetCltObj* pCltObj);
	
private: 
	// ����˫����һ�Կͻ��˶���ָ�룬��һ��ָ��Ķ���ʵ�屾�����������Եķ���˶�����
	CServerOneNetCltObj* m_pRelatedCltObjPtr1; 
	CServerOneNetCltObj* m_pRelatedCltObjPtr2;

	CProtocolForward::CForwardDataSeq m_objRecvSeq1;
	CProtocolForward::CForwardDataSeq m_objRecvSeq2;
	CProtocolForward::CForwardDataSeq m_objRecvSeq;
	
	CProtocolForward::CForwardDataSeq m_objSendSeq;
};


#endif 
