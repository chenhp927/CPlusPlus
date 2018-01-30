//==============================================================================
//                      ClientSessionBase.h
//
//begin   : 2016-08-29
//describe:
//==============================================================================
#ifndef SELFDEF_CLIENTSESSIONBASE_H 
#define SELFDEF_CLIENTSESSIONBASE_H 
 
#include "ClientBase.h" 
#include "ProtocolSession.h"
#include "CommAddr.h"

//==============================================================================
//                 
//                    CClientSessionBase
//           �Ự��ͻ��˻��ࣨ��Э�̡�����������ȷ�ϵĻ��ƣ�
//
//==============================================================================
class MY_EXT_CLASS CClientSessionBase : public CClientBase  
{ 
public: 
	CClientSessionBase(); 
	virtual ~CClientSessionBase(); 
 
protected: 
	/* ���е�ʵ��Ϊ��������ַ��ʼ������ش��� */
	bool LoadClientAddr();
	
	/* ���е�ʵ��Ϊ����ͻ������ӽ������رյ���ش��� */
	// ���ػ��ຯ��
	virtual void OnConnect(); 

	// ���ػ��ຯ��
	virtual void OnClose(); 
	virtual void OnSessionReady(); 
	virtual void OnSessionClose(); 

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ���ػ��ຯ��
	// ����Э��������ݰ��ĳ��ȣ��Ա��ڱ��ͻ��˻�����ݽ��յ����ݽ������ݰ���
	// ȡ���������ѽ��յ����ݻ��޷��������ݰ�����ʱ������-1�����ٴν��ղ�����
	// �ݺ��ٽ��м��㡣
	virtual int GetRecvPacketSize(CBuffer* pBuf);
	
	// ���ػ��ຯ��
	// ������յ����������ݰ����ж����ݰ����Ͳ�������ش���
	virtual void ProcessStreamPacket(CBuffer* pBuf); 

	bool AuthLevel(CBuffer* pBuf); 
	virtual void OnRecvHeart(CBuffer* pBuf); 
	virtual void OnRecvData(CBuffer* pBuf); 
	virtual void OnRecvDataAck(CBuffer* pBuff); 
	virtual void OnRecvNegAck(CBuffer* pBuf); 
	virtual void OnRecvOtherPacket(CBuffer* pBuf); 
	void PushRecvSeq(DWORD dwSeq); 
 	DWORD GetLastRecvSeq() const; 
	virtual void HeartDead(); 
	DWORD GetRecvWindowFirstTime() const;
	
	// ������յ��û����ݰ������û����ݰ�������ն��С�
	// ����������ر��������û����ݰ�������
	virtual void HandleUserData(CBuffer* pBuf);

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// ���ػ��ຯ��
	// �����緢������֮ǰ�������ݱ��
	virtual bool Write();

	virtual void SendHeart(); 
	virtual void SendNegReq(); 
	bool IsNeedToAck() const; 
	virtual void AckData(); 
	virtual void AckFail();
	virtual bool WriteSp(); 
	virtual void OnWriteSp(CBuffer* pBuf); 
	virtual void OnWriteCompletedSp(CBuffer* pBuf); 
	virtual void OnWritePartSp(CBuffer* pBuf, int nSend); 
	virtual void OnWriteErrorSp(CBuffer* pBuf); 
	void PushSendSeq(); 
	bool IsSendWindowFull() const;
	DWORD GetSendWindowFullTime() const;
	void ClearSpSendQueue(); 
	
public: 
	/* ���еĽӿ�Ϊ�����������úͻ�ȡ����ش��� */
	void SetPairNetIp(const char* szIp);
	string GetPairNetIp();
	void SetClientId(short sClientId);
	short GetClientId();
	CCommAddr& GetClientAddr();
	void SetAppClient(bool bIsAppClient = true);
	
	/* ���е�ʵ��Ϊ����ͻ������ӽ������رյ���ش��� */
	bool IsReady(); 

	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	// ��ȡ���յ��ĻỰ�����ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���Ĳ�ȥ���˻Ự��Э��ͷ��Ϣ�����ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��Ϊ���봫��������
	// ע��Ҫ��ȡ���Ự��Э��ͷ��Ϣ�����ݰ�������û���ġ�RecvData()���ӿ�
	virtual bool RecvSessionData(char* szBuf, int& nBufLen); 
	virtual bool RecvSessionData(CBuffer* pBuf); 

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ���ͻỰ�����ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��Ϊ���������
	// ע��Ҫ���ͺ��Ự��Э��ͷ��Ϣ�����ݰ�������û���ġ�SendData()���ӿ�
	virtual bool SendSessionData(char* szBuf, int nBufLen); 
	virtual bool SendSessionData(CBuffer* pBuf); 
	
	/* ���е�ʵ��ΪѲ�����ش��� */
	// ���ػ��ຯ��
	// �����Ե��ã���������Ѳ�������������Ͷ����е����ݰ����ͳ�ȥ
	virtual void DoPatrol(); 
	
	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ���ػ��ຯ��
	// ����־�д�ӡ��������
	virtual string GetDumpParamStr();
	virtual void DumpParam();

	// ���ػ��ຯ��
	// ��ȡ��ʽ���Ŀͻ��˱�ʶ��Ϣ����Ҫ������־���
	virtual string GetTagMyInfo();
	virtual string GetTagMyAddrInfo();

protected: 
	// ���Ͷ��������ݰ�CBuffer��m_nWParam��m_nFlag����������Ӧ�á�
	// ��ͨ��send()���������ݰ����͵�������ʱ������ÿ�ε��ò���һ���ܹ����������ݰ�����������������Ҫ���б�ǡ�
	// m_nFlag���ڱ�Ǳ����ݰ��Ƿ�Ϊ���������ݰ���0-�������ݰ���1-���������ݰ�
	// m_nWParam���ڼ�¼�����ݰ�Ϊ�������ݰ�ʱ�ĳ���
	CQueue<CBuffer* > m_SpSendQueue;		// �Ự��Э�鷢�Ͷ��У����Է���Э�̡�������ȷ�ϰ�
 	
 	/* ����������� */
	int 	m_nHeartInterval; 				// ���������ͼ��ʱ��
	int 	m_nHeartTimeout; 				// ���������ճ�ʱʱ��
	
 	/* ȷ�ϻ������ */
	int		m_nAckInterval;  				// ȷ�ϰ����ͼ��ʱ��
	int		m_nAckTimeout; 					// ȷ�ϰ����ճ�ʱʱ��
	BYTE	m_byWindowSize;					// ���ڴ�С

	DWORD	m_dwSendWinFullTime;			// ���ʹ���������������һ���ķ���ʱ��				 
	DWORD	m_dwSendSeq; 					// �������
	DWORD	m_SendWindow[MAX_COMMWINSIZE];	// ���ʹ���
	BYTE	m_bySendCount; 					// ���ʹ������ѷ�������

	DWORD	m_dwRecvWinFirstTime;			// ���մ����е�һ�����հ�ʱ��
	DWORD	m_dwRecvSeq; 					// �������
	DWORD	m_RecvWindow[MAX_COMMWINSIZE];  // ���մ���
	BYTE	m_byRecvCount;					// ���մ��������հ�����
	
	CCommAddr m_objClientAddr;				// �ͻ��˵�ַ����ΪӦ�����ݰ�����Դ��ַ
	bool	m_bIsAppClient;					// ���ͻ����Ƿ�Ϊҵ�����ͻ���
	char	m_szPairNetIp[IP_LEN];			// ˫�����IP������Ϊ�գ�m_nType = AF_INETʱʹ��
}; 
 
#endif 
