//==============================================================================
//                      NetClient.h
//                       
//begin   : 2016-09-03                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_NETCLIENT_H 
#define SELFDEF_NETCLIENT_H 

#include "ClientForwardBase.h"

//==============================================================================
//                 
//                    CClientOneNet
//                    һ�����ͻ�����
//
//==============================================================================
class CNetClientInterface;
class MY_EXT_CLASS CClientOneNet : public CClientForwardBase
{ 
public: 
	CClientOneNet(CNetClientInterface* pParent, bool bIsForSingleNet = true);
	virtual ~CClientOneNet();
	
protected:
	/* ���е�ʵ��Ϊ����ͻ������ӽ������رյ���ش��� */
	// ���ػ��ຯ��
	virtual void OnConnect(); 

	// ���ػ��ຯ��
	virtual void OnClose(); 

	// ���ػ��ຯ��
	virtual void OnSessionReady(); 

	// ���ػ��ຯ��
	virtual void OnSessionClose(); 

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ���ػ��ຯ��
	// ������յ��û����ݰ���
	// ���ڵ���������ʽͬ���ࣻ����˫�����ش�����������д���
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	virtual void HandleUserData(CBuffer* pBuf);
	
protected:
	CNetClientInterface* m_pParent;
	bool m_bIsForSingleNet;
};

//==============================================================================
//                 
//                    CClientInterface
//                    �ͻ��˽ӿ���
// ˵����ͳһ�����ͻ��˺�˫���ͻ��˵Ľӿ�
//==============================================================================
class MY_EXT_CLASS CNetClientInterface
{
public:
	// �������нӿ��õ���cNetNo�����������ţ�1�����һ������2����ڶ�������-1 �����ܵ����
	
	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ�����������
	virtual bool InitCommParam(TiXmlHandle& txhClient) = 0;

	// ��ʼ�����������Ϣ��ע�ᣩ
	virtual bool InitInCondition(TiXmlHandle& txhClient) = 0;
	
	// ��ʼ������������Ϣ
	virtual bool InitOutCondition(TiXmlHandle& txhClient) = 0;

	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	virtual int GetType(char cNetNo = 1) = 0;
	virtual const char* GetUnixPath(char cNetNo = 1) = 0;
	virtual string GetSvrIp(char cNetNo = 1) = 0;
	virtual int GetSvrPort(char cNetNo = 1) = 0;
	virtual string GetMyIp(char cNetNo = 1) = 0;
	virtual int GetMyPort(char cNetNo = 1) = 0;
	virtual WORD GetClientId(char cNetNo = 1) = 0;

	/* ���еĽӿ�Ϊ�������Ϣ����ش��� */
	virtual CCommAddr& GetClientAddr() = 0;
	virtual CForwardCondition* GetOutCondition() = 0;
	virtual CForwardCondition* GetInCondition() = 0;
	virtual bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead) = 0;
	virtual void SetAppClient(bool bIsAppClient = true) = 0; 
	
	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	virtual bool Connect() = 0; 
	virtual void Close() = 0; 
	virtual bool IsConnect(char cNetNo = -1) = 0;
	virtual bool IsReady(char cNetNo = -1) = 0;	
	
	virtual void OnConnect(CClientOneNet* pClient) = 0; 
	virtual void OnClose(CClientOneNet* pClient) = 0; 
	virtual void OnSessionReady(CClientOneNet* pClient) = 0; 
	virtual void OnSessionClose(CClientOneNet* pClient) = 0; 

	/* ���еĽӿ�Ϊͨ��select���ƻ�ȡ������Ϣ����ش��� */
	virtual void SetFdset(CFdset& objFs) = 0; 
	virtual void HandleNetEvent(CFdset& objFs) = 0;  

	/* ���еĽӿ�Ϊ�������ݵ���ش��� */
	// �������ݰ������¼���
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ
	virtual void OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf) = 0;

	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	virtual bool IsHasData() = 0; 

	// ��ȡ���յ���ת�������ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���Ĳ�ȥ���˻Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���봫��������
	virtual bool RecvData(char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr) = 0; 
	virtual bool RecvData(CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr) = 0; 
	virtual bool RecvData(char* szBuf, int& nBufLen) = 0; 
	virtual bool RecvData(CBuffer* pBuf) = 0; 

	// ��ȡ���յ���ԭʼ���ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���İ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ���ݰ���
	// ���нӿ��õ���pBuf��Ϊ���봫��������
	virtual bool RecvRawData(CBuffer* pBuf) = 0; 

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���������
	virtual bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false) = 0; 
	virtual bool SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false) = 0; 
	virtual bool SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch = false) = 0; 
	virtual bool SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch = false) = 0; 

	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ���нӿ��õ���pBuf��Ϊ���������
	virtual bool SendRawData(CBuffer* pBuf) = 0; 

	/* ���еĽӿ�ΪѲ�����ش��� */
	// �����Ե��ã���������Ѳ�������������Ͷ����е����ݰ����ͳ�ȥ
	virtual void DoPatrol() = 0;
	
	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ����־�д�ӡ��������
	virtual string GetDumpParamStr() = 0;
	virtual void DumpParam() = 0;
	virtual void DumpAllocator() = 0;

	// ��ȡ��ʽ���ķ������Ϣ����Ҫ������־���
	virtual string GetTagSvrInfo(char cNetNo = 1) = 0;	
	virtual string GetTagConnInfo(char cNetNo = 1) = 0;	
};

//==============================================================================
//                 
//                    CNetClientSingle
//                      �����ͻ�����
//
//==============================================================================
class MY_EXT_CLASS CNetClientSingle : public CNetClientInterface, public CObjectBase
{
public: 
	CNetClientSingle();
	virtual ~CNetClientSingle();

public:
	// �������нӿھ�ΪCNetClientInterface�ĺ������ؽӿ�
	// �������нӿ��õ���cNetNo�����������ţ�1�����һ������2����ڶ�������-1 �����ܵ����

	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ�����������
	bool InitCommParam(TiXmlHandle& txhClient);
	
	// ��ʼ�����������Ϣ��ע�ᣩ
	bool InitInCondition(TiXmlHandle& txhClient);
	
	// ��ʼ������������Ϣ
	bool InitOutCondition(TiXmlHandle& txhClient);
	
	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	int GetType(char cNetNo = 1);
	const char* GetUnixPath(char cNetNo = 1);
	string GetSvrIp(char cNetNo = 1);
	int GetSvrPort(char cNetNo = 1);
	string GetMyIp(char cNetNo = 1);
	int GetMyPort(char cNetNo = 1);
	WORD GetClientId(char cNetNo = 1);
	
	/* ���еĽӿ�Ϊ�������Ϣ����ش��� */
	CCommAddr& GetClientAddr();
	CForwardCondition* GetOutCondition();
	CForwardCondition* GetInCondition();
	bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead);
	void SetAppClient(bool bIsAppClient = true); 
	
	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	bool Connect(); 
	void Close(); 
	bool IsConnect(char cNetNo = -1);
	bool IsReady(char cNetNo = -1);	
	
	void OnConnect(CClientOneNet* pClient); 
	void OnClose(CClientOneNet* pClient); 
	void OnSessionReady(CClientOneNet* pClient); 
	void OnSessionClose(CClientOneNet* pClient); 
	
	/* ���еĽӿ�Ϊͨ��select���ƻ�ȡ������Ϣ����ش��� */
	void SetFdset(CFdset& objFs); 
	void HandleNetEvent(CFdset& objFs);  
	
	/* ���еĽӿ�Ϊ�������ݵ���ش��� */
	// �������ݰ������¼���
	// �����ͻ��˲���Ҫʵ�֣����ݰ����Է���m_pClient�Ľ��ն�����
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ
	void OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf);
	
	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	bool IsHasData(); 
	
	// ��ȡ���յ���ת�������ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���Ĳ�ȥ���˻Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���봫��������
	bool RecvData(char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(char* szBuf, int& nBufLen); 
	bool RecvData(CBuffer* pBuf); 
	
	// ��ȡ���յ���ԭʼ���ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���İ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ���ݰ���
	// ���нӿ��õ���pBuf��Ϊ���봫��������
	bool RecvRawData(CBuffer* pBuf); 
	
	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���������
	bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch = false); 
	
	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ���нӿ��õ���pBuf��Ϊ���������
	bool SendRawData(CBuffer* pBuf); 
	
	/* ���еĽӿ�ΪѲ�����ش��� */
	// �����Ե��ã���������Ѳ�������������Ͷ����е����ݰ����ͳ�ȥ
	void DoPatrol();
	
	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ����־�д�ӡ��������
	virtual string GetDumpParamStr();
	void DumpParam();
	virtual void DumpAllocator();
	
	// ��ȡ��ʽ������Ϣ����Ҫ������־���
	string GetTagSvrInfo(char cNetNo = 1);	
	string GetTagConnInfo(char cNetNo = 1);

	/* ���еĽӿ�Ϊ��־�ļ���������ش��� */
	// ���ػ��ຯ��
	// ������־�ļ��ı�ʶ
	virtual void SetLogFileTag(const char* szFileTag);

protected:
	CClientOneNet* m_pClient;
};

//==============================================================================
//                 
//                   CNetClientDouble
//                    ˫���ͻ�����
//
//==============================================================================
class MY_EXT_CLASS CNetClientDouble : public CNetClientInterface, public CObjectBase
{ 
public: 
	CNetClientDouble();
	virtual ~CNetClientDouble();

protected:
	bool DealSingleRecvSeq(CClientOneNet* pClient, CBuffer* pBuf);
	bool DealDoubleRecvSeq(CClientOneNet* pClient);
	void AddRecvPacketToQueue(CClientOneNet* pClient, CBuffer* pBuf);

public:
	// �������нӿھ�ΪCNetClientInterface�ĺ������ؽӿ�
	// �������нӿ��õ���cNetNo�����������ţ�1�����һ������2����ڶ�������-1 �����ܵ����
	
	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ�����������
	bool InitCommParam(TiXmlHandle& txhClient);
	
	// ��ʼ�����������Ϣ��ע�ᣩ
	bool InitInCondition(TiXmlHandle& txhClient);
	
	// ��ʼ������������Ϣ
	bool InitOutCondition(TiXmlHandle& txhClient);

	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	int GetType(char cNetNo = 1);
	const char* GetUnixPath(char cNetNo = 1);
	string GetSvrIp(char cNetNo = 1);
	int GetSvrPort(char cNetNo = 1);
	string GetMyIp(char cNetNo = 1);
	int GetMyPort(char cNetNo = 1);
	WORD GetClientId(char cNetNo = 1);
	
	/* ���еĽӿ�Ϊ�������Ϣ����ش��� */
	CCommAddr& GetClientAddr();
	CForwardCondition* GetOutCondition();
	CForwardCondition* GetInCondition();
	bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead);
	void SetAppClient(bool bIsAppClient = true); 
	
	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	bool Connect(); 
	void Close(); 
	bool IsConnect(char cNetNo = -1);
	bool IsReady(char cNetNo = -1);	
	
	void OnConnect(CClientOneNet* pClient); 
	void OnClose(CClientOneNet* pClient); 
	void OnSessionReady(CClientOneNet* pClient); 
	void OnSessionClose(CClientOneNet* pClient); 
	
	/* ���еĽӿ�Ϊͨ��select���ƻ�ȡ������Ϣ����ش��� */
	void SetFdset(CFdset& objFs); 
	void HandleNetEvent(CFdset& objFs);  
	
	/* ���еĽӿ�Ϊ�������ݵ���ش��� */
	// �������ݰ������¼���
	// ����˫����š�ע�����
	// �����buffer�����ݰ��������Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	void OnRecvPacket(CClientOneNet* pClient, CBuffer* pBuf); 
	
	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	bool IsHasData(); 
	
	// ��ȡ���յ���ת�������ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���Ĳ�ȥ���˻Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���봫��������
	bool RecvData(char* szBuf, int& nBufLen, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(CBuffer* pBuf, CProtocolForward::CForwardAddr& objSrcAddr); 
	bool RecvData(char* szBuf, int& nBufLen); 
	bool RecvData(CBuffer* pBuf); 

	// ��ȡ���յ���ԭʼ���ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���İ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ���ݰ���
	// ���нӿ��õ���pBuf��Ϊ���봫��������
	bool RecvRawData(CBuffer* pBuf); 

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���������
	bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr, bool bIsSupportLocalDispatch = false); 
	bool SendData(char* szBuf, int nBufLen, bool bIsSupportLocalDispatch = false); 
	bool SendData(CBuffer* pBuf, bool bIsSupportLocalDispatch = false); 
	
	// ����ԭʼ���ݰ���
	// �����buffer�����ݰ��������������������Ѱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ���뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ��������ݰ����ᱻ�Զ�����˫����Ϣ��
	// ���нӿ��õ���pBuf��Ϊ���������
	bool SendRawData(CBuffer* pBuf); 
	
	/* ���еĽӿ�ΪѲ�����ش��� */
	// �����Ե��ã���������Ѳ�������������Ͷ����е����ݰ����ͳ�ȥ
	virtual void DoPatrol();
	
	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ����־�д�ӡ��������
	virtual string GetDumpParamStr();
	void DumpParam();
	
	// ��ȡ��ʽ������Ϣ����Ҫ������־���
	string GetTagSvrInfo(char cNetNo = 1);
	string GetTagConnInfo(char cNetNo = 1);
	virtual void DumpAllocator();

	/* ���еĽӿ�Ϊ��־�ļ���������ش��� */
	// ���ػ��ຯ��
	// ������־�ļ��ı�ʶ
	virtual void SetLogFileTag(const char* szFileTag);
	
public:
	// �������нӿھ�Ϊ�Ǻ������ؽӿ�

	/* ���еĽӿ�Ϊ���ն��й������ش��� */
	void ClearRecvQueue();

protected:
	CClientOneNet* m_pClient1;
	CClientOneNet* m_pClient2;

	CProtocolForward::CForwardDataSeq m_objRecvSeq1;
	CProtocolForward::CForwardDataSeq m_objRecvSeq2;
	CProtocolForward::CForwardDataSeq m_objRecvSeq;
	CProtocolForward::CForwardDataSeq m_objSendSeq;
	
	CBuffer::CAllocator m_objBufAlloc; 	// ������������
	CQueue<CBuffer* > m_RecvQueue; 		// ���ݰ����ն���
};



#endif 
