//==============================================================================
//                      ClientForwardBase.h
//                       
//begin   : 2016-08-30                                                 
//describe:                                              
//==============================================================================
#ifndef SELFDEF_CLIENTFORWARDBASE_H 
#define SELFDEF_CLIENTFORWARDBASE_H 

#include <string>
using namespace std;

#include "ClientSessionBase.h"
#include "ForwardCondition.h"

//==============================================================================
//                 
//                    CClientForwardBase
//           ת����ͻ��˻��ࣨ��˫����ת���������ƣ�
//
//==============================================================================
class TiXmlHandle;
class MY_EXT_CLASS CClientForwardBase : public CClientSessionBase
{ 
public: 
	CClientForwardBase();
	virtual ~CClientForwardBase();

protected:
	/* ���е�ʵ��Ϊ������ʼ������ش��� */
	// ��ʼ�����������cNetNo���������ţ�1�����һ������2����ڶ�����
	bool InitClientId(TiXmlHandle& txhClient);
	bool InitClientType(TiXmlHandle& txhClient);
	bool InitServerPath(TiXmlHandle& txhClient);
	bool InitMyIpPort(TiXmlHandle& txhMyIpPort, char cNetNo = 1);
	bool InitServerIpPort(TiXmlHandle& txhServerIpPort, char cNetNo = 1);
		
	/* ���е�ʵ��Ϊ����ͻ������ӽ������رյ���ش��� */
	// ���ػ��ຯ��
	virtual void OnSessionReady();

	/* ���е�ʵ��Ϊ�������ݵ���ش��� */
	// Ϊҵ��ͻ��˷��͵���������Դ��ַ
	bool SetSrcAddrForSendData(char* szBuf, int nBufLen);

	// Ϊҵ��ͻ��˷��͵��������������롢������
	bool SetTypeFuncForSendData(char* szBuf, int nBufLen);
	bool CheckSendData(char* szBuf, int nBufLen);

public:
	/* ���еĽӿ�Ϊ������ʼ������ش��� */
	// ��ʼ�����������cNetNo���������ţ�1�����һ������2����ڶ�����
	bool InitCommParam(TiXmlHandle& txhClient, char cNetNo = 1);

	// ��ʼ�����������Ϣ��ע�ᣩ
	bool InitInCondition(TiXmlHandle& txhClient);

	// ��ʼ������������Ϣ
	bool InitOutCondition(TiXmlHandle& txhClient);

	/* ���еĽӿ�Ϊע����Ϣ��������������ش��� */
	CForwardCondition* GetInCondition();
	CForwardCondition* GetOutCondition();
	bool CanGoOut(CProtocolForward::CForwardHead& objForwardHead);
	
	/* ���еĽӿ�Ϊ��ȡ�������ݰ�����ش��� */
	// ��ȡ���յ���ת�������ݰ���
	// �������Ͻ��յ�ÿһ��������������������Э�̡�����������ȷ�ϵȴ�����Ƶİ���ת�����ע�����ݰ������������ն��С�
	// ���нӿڷ��ص�buffer�����ݰ�������ָ�ӽ��ն�����ȡ���Ĳ�ȥ���˻Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ���
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���봫��������
	// ע��Ҫ��ȡֻ����ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�RecvSessionData()���ӿ�
	// ע��Ҫ��ȡ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�RecvData()���ӿ�
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int& nBufLen); 
	virtual bool RecvForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf); 
	virtual bool RecvForwardData(char* szBuf, int& nBufLen); 
	virtual bool RecvForwardData(CBuffer* pBuf); 

	/* ���еĽӿ�Ϊ�������ݰ�����ش���*/
	// ����ת�������ݰ���
	// �����buffer�����ݰ������������������в�Ӧ�����Ự��Э��ͷ��ת����Э��ͷ��Ϣ��
	// ��������ݰ����ᱻ�Զ���ӻỰ��Э��ͷ��ת����Э��ͷ��Ϣ����뷢�Ͷ��У���DoPatrol()������ʱ�Ὣ���ݰ����͵������ϡ� 
	// ���нӿ��õ���szBuf��nBufLen��pBuf��objForwardHead��Ϊ���������
	// ע��Ҫ����ֻ����ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�SendSessionData()���ӿ�
	// ע��Ҫ���Ͱ����Ự��Э��ͷ��ת����Э��ͷ��Ϣ�����ݰ�������û���ġ�SendData()���ӿ�
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, char* szBuf, int nBufLen); 
	virtual bool SendForwardData(CProtocolForward::CForwardHead& objForwardHead, CBuffer* pBuf); 
	virtual bool SendForwardData(char* szBuf, int nBufLen); 
	virtual bool SendForwardData(CBuffer* pBuf); 

	/* ���еĽӿ�Ϊ��־�������ش��� */
	// ���ػ��ຯ��
	// ����־�д�ӡ��������
	virtual string GetDumpParamStr();
	virtual void DumpParam();
	virtual void DumpRegPacket();

protected:
	CForwardCondition m_objOutConds;	// ��Ϣ���͵ĳ�������
	CForwardCondition m_objInConds;		// ��Ϣ���յ��������
	CBuffer* m_pBufRegPacket;			// �ͻ���ע����Ϣ��buffer	
}; 

#endif 
