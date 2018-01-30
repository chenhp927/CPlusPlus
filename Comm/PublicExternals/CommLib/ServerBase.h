//==============================================================================
//                      ServerBase.h
//
//begin   : 2016-05-13
//describe:
//==============================================================================
#ifndef SELFDEF_SERVER_BASE_H 
#define SELFDEF_SERVER_BASE_H 
 
#include <vector>

#include "TypeDef.h"
#include "SockMacro.h" 
#include "Buffer.h" 
#include "Queue.h" 
#include "Fdset.h" 
#include "Sid.h"
#include "ObjectBase.h"
#include "DataStreamStat.h"

using namespace std;

//==============================================================================
//                 
//                    CServerBase
//                    ����˻���
//
//==============================================================================
class CServerBaseCltObj;
class MY_EXT_CLASS CServerBase : public CObjectBase
{ 
public: 
	CServerBase(); 
	virtual ~CServerBase(); 

	// �ú����ڸ��๹���Ҫ�����ֶ�����һ�Σ����Կ������캯����һ���֣�Ŀ����
	// Ϊ���ڹ��������ִ���麯��CreateObject����Ϊ���캯����Ҫ��������麯����
	// �ú����ڵ��óɹ�����Ե��ö�Σ������سɹ�
	bool Create();

protected:  
	/* ���е�ʵ��Ϊ��������������������رյ���ش��� */
	virtual bool OnStart(); 
	virtual bool MakeInetSocket();  
	virtual bool MakeUnixSocket();

	/* ���е�ʵ��Ϊ����ͻ������ӽ������رյ���ش��� */
	virtual void Accept(); 
	virtual void OnInetAccept(int nSockfd, struct sockaddr_in& stuSockAddr); 
	virtual void OnUnixAccept(int nSockfd); 
	virtual void DenyClient(int nSockfd, struct sockaddr_in* stuSockAddr, int nReason);
	virtual void OnFdExcept();  
	virtual void OnSessionConnect(CSid objSid); 
	virtual void OnSessionClose(CSid objSid); 
	virtual bool IsClientIpValid(struct sockaddr_in& stuSockAddr);  
	virtual bool IsTooMuchSameClient(struct sockaddr_in& stuSockAddr);  

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	virtual int  Read(CServerBaseCltObj* pCltObj); 
	virtual void OnRead(CServerBaseCltObj* pCltObj); 
	virtual void OnReadCompleted(CServerBaseCltObj* pCltObj); 
	virtual void OnReadClientClose(CServerBaseCltObj* pCltObj); 
	virtual void OnReadError(CServerBaseCltObj* pCltObj); 
	void ClearRecvQueue();
	
	// ����Э��������ݰ��ĳ��ȣ��Ա��ڱ��ͻ��˻�����ݽ��յ����ݽ������ݰ���
	// ȡ���������ѽ��յ����ݻ��޷��������ݰ�����ʱ������-1�����ٴν��ղ�����
	// �ݺ��ٽ��м��㡣
	// ���������ʵ�ֱ�������
	virtual int GetRecvPacketSize(CBuffer* pBuf) = 0;
	
	// ������յ����������ݰ��������ݰ�ֱ�ӷ�����ն��С�
	virtual void ProcessStreamPacket(CServerBaseCltObj* pCltObj, CBuffer* pBuf);
	
	// �����յ������ݰ�������ն��С�
	void AddRecvPacketToQueue(CServerBaseCltObj* pCltObj, CBuffer* pBuf);

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	virtual bool Write(CServerBaseCltObj* pCltObj); 
	virtual void OnWrite(CServerBaseCltObj* pCltObj, CBuffer* pBuf) ; 
	virtual void OnWriteCompleted(CServerBaseCltObj* pCltObj, CBuffer* pBuf); 
	virtual void OnWritePart(CServerBaseCltObj* pCltObj, CBuffer* pBuf, int nSend); 
	virtual void OnWriteError(CServerBaseCltObj* pCltObj, CBuffer* pBuf); 

	/* ���е�ʵ��ΪѲ�����ش��� */
	virtual void OnPatrol();  
	virtual void PatrolClientObject(CServerBaseCltObj* pCltObj);

	/* ���е�ʵ��Ϊ����ͻ��˶������ش��� */
	bool AllocateClientPool();
	virtual CServerBaseCltObj* AssignClientObject(); 
	virtual CServerBaseCltObj* CreateClientObject(); 
	void UpdateMaxClientBoundOnConnect(CServerBaseCltObj* pCltObj);
	void UpdateMaxClientBoundOnClose(CServerBaseCltObj* pCltObj);

public: 
	/* ���еĽӿ�Ϊ���������ʼ������ش��� */
	// ���������ļ����л��������ĳ�ʼ����������δʵ�֣���Ҫ������������ʵ��
	virtual bool Init(const char* szConfigFile);

	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	void SetType(int nType);
	int GetType();
	bool SetUnixPath(const char* szPath);
	const char* GetUnixPath();
	bool SetSvrIp(const char* szSvrIp);
	string GetSvrIp();
	bool SetSvrPort(unsigned short sPort);
	int GetSvrPort();
	bool SetMaxClients(int nMax);
	int GetMaxClients();
	int GetMaxClientBound();
	void SetMaxPacketSize(int nSize);
	int GetMaxPacketSize();
	void SetSidHigh(DWORD dw);
	DWORD GetSidHigh();

	// ��������Ƿ�û������
	bool IsNotCfged();

	/* ���еĽӿ�Ϊ���������������رա��ж�����ش��� */
	virtual bool Startup(unsigned short nPort, const char* szIP = "0.0.0.0");  
	virtual bool Startup(const char* szPath); 
	virtual bool Startup();

	// �رռ���
	virtual void CloseSvr();

	/* ���еĽӿ�Ϊͨ��select���ƻ�ȡ������Ϣ����ش��� */
	void SetFdset(CFdset& objFs); 
	virtual void HandleNetEvent(CFdset& objFs);  
	void SetListenFdset(CFdset& objFs); 
	virtual void HandleListenNetEvent(CFdset& objFsfs);  
	void SetClientFdset(CFdset& objFs); 
	virtual void HandleClientNetEvent(CFdset& objFs);  
	void SetClientFdset(CFdset& objFs, vector<CServerBaseCltObj*>& vecCltObj); 
	virtual void HandleClientNetEvent(CFdset& objFs, vector<CServerBaseCltObj*>& vecCltObj);  

	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	bool IsHasData(); 
	
	// ��ȡ���յ������ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ�������ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��Ϊ���봫��������
	// ���нӿ��õ���szCltIP��nCltPort��objSidΪ����������
	virtual bool RecvData(char* szBuf, int& nBufLen, char* szCltIP, unsigned short& nCltPort);
	virtual bool RecvData(CBuffer* pBuf, char* szCltIP, unsigned short& nCltPort); 
 	virtual bool RecvData(char* szBuf, int& nBufLen, CSid& objSid); 
	virtual bool RecvData(CBuffer* pBuf, CSid& objSid); 

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// �������ݰ���
	// �����buffer�����ݰ�����������������������ݰ�����ֱ�ӷ��뷢�Ͷ��У�
	// ��DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��szCltIP��nCltPort��objSid��pCltObj��Ϊ���������
	virtual bool SendData(char* szBuf, int nBufLen, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendData(CBuffer* pBuf, const char* szCltIP, unsigned short nCltPort);
	virtual bool SendData(char* szBuf, int nBufLen, CSid objSid);
	virtual bool SendData(CBuffer* pBuf, CSid objSid);
	virtual bool SendData(char* szBuf, int nBufLen, CServerBaseCltObj* pCltObj);

	/* ���еĽӿ�ΪѲ�����ش��� */
	// �����Ե��ã���������Ѳ�������������Ͷ����е����ݰ����ͳ�ȥ
	virtual void DoPatrol(); 
	virtual void DoPatrol(vector<CServerBaseCltObj*>& vecCltObj);  

	/* ���еĽӿ�Ϊ����ͻ��˶�����Ϣ��ȡ���������ش��� */
	CServerBaseCltObj* GetClientObjByIndex(int nIndex);
	CServerBaseCltObj* GetClientObj(const char* szCltIP,unsigned short nCltPort); 
	CServerBaseCltObj* GetClientObj(CSid& objSid); 
	void CloseClientObj(CSid objSid);
	void CloseClientObj(const char* szCltIP, unsigned short nCltPort); 
	void CloseClientObj(CServerBaseCltObj* pCltObj); 
	std::string GetClientObjIp(CSid objSid);
	bool IsMyClientObj(CServerBaseCltObj* pCltObj);

	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ����־�д�ӡ��������
	virtual string GetDumpParamStr();
	virtual void DumpParam();
	virtual void DumpAllocator();
	
	// ��ȡ��ʽ���ķ���˱�ʶ��Ϣ����Ҫ������־���
	virtual string GetTagMyInfo();
	
protected: 
	int		m_nSockfd; 					// socket�ļ�������
	int		m_nType;					// ͨ��ʹ�õĵ�Э�飬AF_INET��ʾIPV4��AF_UNIX��ʾ����ͨ�� 
	char 	m_szUnixPath[UNIX_PATH_LEN];// ����˼����ĵ�ַ��m_nType = AF_UNIXʱʹ��
	char	m_szSvrIp[IP_LEN]; 			// ����˼�����ip��m_nType = AF_INETʱʹ�� 
	unsigned short	m_sSvrPort; 				// ����˼����Ķ˿ڣ�m_nType = AF_INETʱʹ��

	DWORD	m_dwSidHigh;				// ��������ÿ���Ự���ӵĸ�λ��Ϣ
	CServerBaseCltObj **m_ClientsPtr; 	// ���뱾����˵����пͻ�����Ϣ
	int		m_nMaxClients;  			// ���뱾����˵�����пͻ�������
	int		m_nCurMaxClientBound; 		// ��ǰ����������ڵ�����+1
   
  	int		m_nMaxPacketSize;			// ���ݰ���󳤶�

	// ���ն��������ݰ�CBuffer��m_nWParam��m_nLParam����������Ӧ�á�
	// �Ӹ����ͻ��˶�����յ����ݰ��������뱾���У�������Ҫ������ݰ�����Դ��
	// m_nWParam��m_nLParam���ڼ�¼�����ݰ���Դ��CSid
 	CQueue<CBuffer *>	m_RecvQueue;  	// ���ݰ����ն���

}; 

//==============================================================================
//                 
//                    CServerBaseCltObj
//              ����˻���Ŀͻ��˶�����
//
//==============================================================================
class MY_EXT_CLASS CServerBaseCltObj : public CObjectBase
{ 
public: 
	CServerBaseCltObj(CServerBase* pServer); 
	virtual ~CServerBaseCltObj(); 
 
protected: 
	/* ���е�ʵ��Ϊ�ͻ������������������رա��ж�����ش��� */
	virtual void OnConnect(); 
	virtual void OnClose(); 
 
	/* ���е�ʵ��Ϊ���ݰ����Ͷ��в�������ش��� */
	void ClearSendQueue(); 

	/* ���е�ʵ��Ϊ�ͻ��˶����������ش��� */
	virtual void ClearCltInfo();

public: 
	/* ���еĽӿ�Ϊ���ػ���ӿ� */
	// ������־�ļ��ı�ʶ
	virtual void SetLogFileTag(const char* szFileTag);

	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	CSid GetSid();
	int GetSocket();
	bool IsSocketValid();
	void SetPeerName(const char* szPeerName);
	string GetPeerName();
	void SetPeerIP(const char* szPeerIp);
	string GetPeerIp();
	void SetPeerPort(unsigned short sPeerPort);
	int GetPeerPort();
	void SetIndex(int nIndex);
	int GetIndex();
	void SetConnectStatus(int nStatus); 
	int GetConnectStatus();
	void SetSidHigh(DWORD dwSidHigh);
	DWORD GetSidHigh();
 	void SetSendQueueDepth(int nSendQueueDepth);
	int GetSendQueueDepth();
	void  SetRecvTime(); 
	DWORD GetRecvTime();
	void  SetSendTime(); 
	DWORD GetSendTime(); 
	
	/* ���еĽӿ�Ϊ�ͻ������������������رա��ж�����ش��� */
	virtual void Connect(int nSockfd, const struct sockaddr_in& stuSockAddr);
	virtual void Connect(int nSockfd);
	virtual bool Close(); 
	bool IsConnect(); 
	
	/* ���еĽӿ�Ϊ�������ݵ���ش��� */
	// ��ȡ�������ݵ���ʱ������
	CBuffer* GetRecvTmpBuf();

	/* ���еĽӿ�Ϊ���ݰ����Ͷ��в�������ش��� */
	// ���нӿڴ����buffer�����ݰ�������������
	bool PushSendData(CBuffer* pBuf);
	bool IsHasSendData();
	CBuffer* PopSendData();
	bool RollbackSendData(CBuffer* pBuf);
	int GetSendQueueSize();

	// �û����ݶ�������ǰ��İ��Ƿ��Ѿ�����������
	bool IsSendQueueInPart();

	// ��ȡ������������
	CBuffer* Allocate();

	/* ���еĽӿ�Ϊ��ȡ��������������ش��� */
	CServerBase* GetRelatedServerPtr();
	
	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ��ȡ��ʽ���Ŀͻ�����Ϣ����Ҫ������־���
	virtual string GetTagCltInfo();
	virtual string GetTagConnInfo();
	virtual void DumpAllocator();

	/* ���еĽӿ�Ϊͳ�������շ����ݰ���������ش��� */
	// ͳ�Ʒ��Ͱ�����
	void StreamStatSend(CBuffer* pBuf);
	void StreamStatRecv(CBuffer* pBuf);

protected:  
	CServerBase* m_pRelatedServerPtr;		// �����ķ���������ָ�룬��ֵ�ڳ�ʼ���׶ξ��̶ܹ�����
	int 	m_nCltIndex;					// ��clt�����ڷ���������е���������ֵ�ڳ�ʼ���׶ξ��̶ܹ�����
	DWORD	m_dwSidHigh;					// ���ӵĻỰID�ĸ�λ��ֵ����ֵ�ڳ�ʼ���׶ξ��̶ܹ�����

	CSid 	m_objSid;						// ���ӵĻỰID
	int 	m_nSockfd;						// socket�ļ�������
	char	m_szPeerName[SESSION_NAME_LEN];	// ���뱾����˵Ŀͻ��˵�����
	char	m_szPeerIp[IP_LEN];				// ���뱾����˵Ŀͻ��˵�ip
	unsigned short	m_sPeerPort; 			// ���뱾����˵Ŀͻ��˵Ķ˿�

	int 	m_nConnectStatus; 				// �ÿͻ����뱾����˵�ǰ����״̬

 	CBuffer::CAllocator m_objBufAlloc;		// ������������
	CBuffer* m_pRecvTmpBuf; 				// �������ݵ���ʱ������

	// ���Ͷ��������ݰ�CBuffer��m_nWParam��m_nFlag����������Ӧ�á�
	// ��ͨ��send()���������ݰ����͵�������ʱ������ÿ�ε��ò���һ���ܹ����������ݰ�����������������Ҫ���б�ǡ�
	// m_nFlag���ڱ�Ǳ����ݰ��Ƿ�Ϊ���������ݰ���0-�������ݰ���1-���������ݰ�
	// m_nWParam���ڼ�¼�����ݰ�Ϊ�������ݰ�ʱ�ĳ���
	CQueue<CBuffer *> m_SendQueue; 			// ���ݰ����Ͷ���
	int m_nSendQueueDepth;					// ���Ͷ��е�������

	DWORD	m_dwSendTime; 					// �ϴη���ʱ��
	DWORD	m_dwRecvTime; 					// �ϴ��հ�ʱ��

//	CDataStreamStatMgr	m_objStreamStatSend;// ���ݰ�������ͳ��
//	CDataStreamStatMgr	m_objStreamStatRecv;// ���ݰ�������ͳ��
}; 
   
#endif 
