//=========================================================================================
//
//							 ProtocolMaintenContent.h
// begin    : 2016-10-10
// describe : ����ά��Э�鼯
//
//=========================================================================================
#ifndef _PROTOCOLMAINTENCONTENT_H_2016_10_10
#define _PROTOCOLMAINTENCONTENT_H_2016_10_10

#include <vector>
#include "TypeDef.h"
#include "ProtocolBase.h"
#include "ProtocolHead.h"
#include "ProtocolDevState.h"	
#include "ProtocolMaintenComm.h"

namespace ProtocolMaintenContent
{
	using namespace std;
	using namespace ProtocolBase;
	using namespace ProtocolIndication;
	
	//==============================================================================
	//
	//             CReqLocalCfgItoAPkt
	//             ���󱾻�����������Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CReqLocalCfgItoAPkt : public CProtocolHead
	{
	public:
		CReqLocalCfgItoAPkt();
		~CReqLocalCfgItoAPkt();
		
	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;
		
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������
		string m_strHostName;
	};

	//==============================================================================
	//
	//             CMoniInfoBase
	//          ������Ϣ���࣬���������������������
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoBase : public CObjectBase
	{
	public:
		CMoniInfoBase();
		virtual ~CMoniInfoBase();

	public:
		void SetMoniType(BYTE byMoniType);
		BYTE GetMoniType() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	protected:
		BYTE m_byMoniType;		// �������
	};
	
	//==============================================================================
	//
	//             CLocalProcItoA
	//             ������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CLocalProcItoA : public CObjectBase
	{
	public:
		CLocalProcItoA();
		~CLocalProcItoA();
		
	public:
		// ���ý�����
		void SetProcName(string strProcName);
		// ��ȡ������
		string GetProcName() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������
		string m_strProcName;
	};
	
	//==============================================================================
	//
	//             CSingleHostCfg
	//             ��������������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CSingleHostCfg : public CObjectBase
	{
	public:
		CSingleHostCfg();
		~CSingleHostCfg();
		
	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;
		
		// ��ȡ������͸���
		int GetMoniTypeInfoCount() const;
		// ��ȡָ���±�ļ������
		const CMoniInfoBase* GetMoniTypeInfo(int nIndex) const;
		// ��Ӽ������
		void Add(const CMoniInfoBase& objMoniTypeInfo);
		void Clear();

		// ��ȡ������Ϣ����
		int GetLocalProcItoACount() const;
		// ��ȡָ���±�Ľ�����Ϣ
		const CLocalProcItoA* GetLocalProcItoA(int nIndex) const;
		// ��ӽ�����Ϣ
		void Add(const CLocalProcItoA& objLocalProcItoA);
		void CLocalProcItoAClear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������
		string m_strHostName;
		// �������
		vector<CMoniInfoBase> m_vecMoniTypeInfo;
		// ������Ϣ
		vector<CLocalProcItoA> m_vecLocalProcItoA;
	};

	//==============================================================================
	//
	//             CHostsCfgAtoIPkt
	//             ��������������Ϣ��RmAppHostMgr��RmInterHost��
	//
	//==============================================================================
	class MY_EXT_CLASS CHostsCfgAtoIPkt : public CProtocolHead
	{
	public:
		CHostsCfgAtoIPkt();
		~CHostsCfgAtoIPkt();

	public:
		// ��ȡ��������
		int GetHostsCount() const;
		// ��ȡָ���±������������Ϣ
		const CSingleHostCfg* GetSingleHostCfg(int nIndex) const;
		// ��Ӽ������
		void Add(const CSingleHostCfg& objSingleHostCfg);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ��������������Ϣ
		vector<CSingleHostCfg> m_vecHostsCfg;
	};

	//==============================================================================
	//
	//						CPU��������
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoCpuLoad : public CMoniInfoBase
	{
	public:
		CMoniInfoCpuLoad();
		~CMoniInfoCpuLoad();

	public:
		// ����CPUռ����
		void SetCPULoadPer(float fCPULoadPer);
		// ��ȡCPUռ����
		float GetCPULoadPer() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		float m_fCPULoadPer; // CPUռ����
	};

	//==============================================================================
	//
	//						�ڴ�����
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoMemLoad : public CMoniInfoBase
	{
	public:
		CMoniInfoMemLoad();
		~CMoniInfoMemLoad();

	public:
		// �����ڴ�ռ����
		void SetMemLoadPer(float fMemLoadPer);
		// ��ȡ�ڴ�ռ����
		float GetMemLoadPer() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// �ڴ�ռ����
		float m_fMemLoadPer;
	};

	//==============================================================================
	//
	//             CCPUAndMEMOfProcItoA
	//             ������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CCPUAndMEMOfProcItoA : public CObjectBase
	{
	public:
		CCPUAndMEMOfProcItoA();
		~CCPUAndMEMOfProcItoA();
		
	public:
		// ���ý�����
		void SetProcName(string strProcName);
		// ��ȡ������
		string GetProcName() const;
		
		// ���ý���ID
		void SetPID(DWORD dwPID);
		// ��ȡ����ID
		DWORD GetPID() const;
		
		// ����CPUռ����
		void SetCPULoadPer(float fCPULoadPer);
		// ��ȡCPUռ����
		float GetCPULoadPer() const;
		
		// �����ڴ�ռ����
		void SetMemLoadPer(float fMemLoadPer);
		// ��ȡ�ڴ�ռ����
		float GetMemLoadPer() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������
		string m_strProcName;
		
		// ����ID
		DWORD m_dwPID;
		
		// CPUռ����
		float m_fCPULoadPer;
		
		// �ڴ�ռ����
		float m_fMemLoadPer;
	};
	
	//==============================================================================
	//
	//					��������
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoProcess : public  CMoniInfoBase
	{
	public:
		CMoniInfoProcess();
		~CMoniInfoProcess();

	public:
		// ��ȡ������Ϣ����
		int GetCPUAndMEMOfProcItoACount() const;
		// ��ȡָ���±�Ľ�����Ϣ
		CCPUAndMEMOfProcItoA* GetCPUAndMEMOfProcItoA(int nIndex);
		// ��ӽ�����Ϣ
		void Add(const CCPUAndMEMOfProcItoA objCPUAndMEMOfProcItoA);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector <CCPUAndMEMOfProcItoA> m_vecMoniInfoProcess;		//������Ϣ
	};

	//==============================================================================
	//
	//             CCpuAndMemItoAPkt
	//             ����������̵�CPU���ڴ�ʵʱ״̬��Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CCpuAndMemItoAPkt : public CProtocolHead
	{
	public:
		CCpuAndMemItoAPkt();
		~CCpuAndMemItoAPkt();

		//���������=�����
		CCpuAndMemItoAPkt& operator=(const CCpuAndMemItoAPkt& objCpuAndMemItoAPkt);
		//�������캯�������
		CCpuAndMemItoAPkt(const CCpuAndMemItoAPkt& objCpuAndMemItoAPkt);

	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;
		
		// ��ȡ�����Ϣ����
		int GetMoniInfoBaseCount() const;
		// ��ȡָ���±�ļ����Ϣ
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ��Ӽ����Ϣ
		// ���CPU������Ϣ
		void AddCPULoad(const float fCPULoad);

		//����ڴ���Ϣ
		void AddMEMLoad(const float fMEMLoad);

		//��ӽ�����Ϣ
		void AddProcess(const CMoniInfoProcess& objProcess);
		
	private:
		// ������
		string m_strHostName;
		//�����Ϣ
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//				 CMoniInfoOS
	//				 ����ϵͳ
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoOS : public CMoniInfoBase
	{
	public:
		CMoniInfoOS();
		~CMoniInfoOS();

	public:
		// ���ò���ϵͳ����
		void SetOSName(string strOSName);
		// ��ȡ����ϵͳ����
		string GetOSName() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strOSName;	//����ϵͳ����
	};

	//==============================================================================
	//
	//						CPU��������
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoCpuNum : public CMoniInfoBase
	{
	public:
		CMoniInfoCpuNum();
		~CMoniInfoCpuNum();

	public:
		// ����CPU����
		void SetCPUNum(BYTE byCPUNum);
		// ��ȡCPU����
		BYTE GetCPUNum() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		BYTE m_byCPUNum; // CPU����
	};

	//==============================================================================
	//
	//						�ڴ���������
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoMemCap : public CMoniInfoBase
	{
	public:
		CMoniInfoMemCap();
		~CMoniInfoMemCap();

	public:
		// �����ڴ�ռ����
		void SetMemCap(float fMemCap);
		// ��ȡ�ڴ�ռ����
		float GetMemCap() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// �ڴ�ռ����
		float m_fMemCap;
	};

	//==============================================================================
	//
	//             CDisk
	//             ������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CDisk : public CObjectBase
	{
	public:
		CDisk();
		~CDisk();
		
	public:
		// ���ô�������
		void SetDiskName(string strDiskName);
		// ��ȡ��������
		string GetDiskName() const;
		
		// ���ô�������
		void SetDiskCap(float fDiskCap);
		// ��ȡ��������
		float GetDiskCap() const;
		
		// ���ô���ʹ����
		void SetDiskLoadPer(float fDiskLoadPer);
		// ��ȡ����ʹ����
		float GetDiskLoadPer() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ��������
		string m_strDiskName;
		// ��������
		float m_fDiskCap;
		// ����ʹ����
		float m_fDiskLoadPer;
	};
	
	//==============================================================================
	//
	//             CMoniInfoDisk
	//             �̳м�����Ϣ����
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoDisk : public CMoniInfoBase
	{
	public:
		CMoniInfoDisk();
		~CMoniInfoDisk();

	public:
		// ��ȡ������Ϣ����
		int GetDiskCount() const;
		// ��ȡָ���±�Ĵ�����Ϣ
		CDisk* GetDisk(int nIndex);
		// ��Ӵ�����Ϣ
		void Add(const CDisk& objDisk);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CDisk> m_vecDisk;	//������Ϣ
	};

	//==============================================================================
	//
	//             CIPInfo
	//             IP��ַ��Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CIPInfo : public CObjectBase
	{
	public:
		CIPInfo();
		~CIPInfo();
		
	public:
		// ����IP��ַ
		void SetIP(string strIP);
		// ��ȡIP��ַ
		string GetIP() const;
		
		// ������������
		void SetMask(string strMask);
		// ��ȡ��������
		string GetMask() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// IP��ַ
		string m_strIP;
		// ��������
		string m_strMask;
	};
	
	//==============================================================================
	//
	//             CNetCard
	//             ������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CNetCard : public CObjectBase
	{
	public:
		CNetCard();
		~CNetCard();
		
	public:
		// ������������
		void SetCardName(string strCardName);
		// ��ȡ��������
		string GetCardName() const;
		
		// ��������״̬
		void SetCardState(BYTE byCardState);
		// ��ȡ����״̬
		BYTE GetCardState() const;
		
		// ��ȡIP��ַ��Ϣ����
		int GetIPInfoCount() const;
		// ��ȡָ���±��IP��ַ��Ϣ
		const CIPInfo* GetIPInfo(int nIndex) const;
		// ���IP��ַ��Ϣ
		void Add(const CIPInfo& objIPInfo);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ��������
		string m_strCardName;
		// ����״̬
		BYTE m_byCardState;
		// IP��ַ��Ϣ
		vector<CIPInfo> m_vecIPInfo;
	};

	//==============================================================================
	//
	//             CMoniInfoNetCard
	//             �̳м�����Ϣ����
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoNetCard : public CMoniInfoBase
	{
	public:
		CMoniInfoNetCard();
		~CMoniInfoNetCard();

	public:
		// ��ȡ������Ϣ����
		int GetMoniInfoNetCardCount() const;
		// ��ȡָ���±��������Ϣ
		const CNetCard* GetMoniInfoNetCard(int nIndex) const;
		// ���������Ϣ
		void Add(const CNetCard& objNetCard);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CNetCard> m_vecMoniInfoNetCard;	//������Ϣ
	};

	//==============================================================================
	//
	//             CAllHostProcItoA
	//            
	//
	//==============================================================================
	class MY_EXT_CLASS CAllHostProcItoA : public CObjectBase
	{
	public:
		CAllHostProcItoA();
		~CAllHostProcItoA();
		
	public:
		// ���ý�������
		void SetProcName(const string& strProcName);
		// ��ȡ��������
		string GetProcName() const;
		
		// ���ý���ID
		void SetPID(DWORD dwPID);
		// ��ȡ����ID
		DWORD GetPID() const;
		
		// ���ý���·��
		void SetProcPath(const string& strProcPath);
		// ��ȡ����·��
		string GetProcPath() const;
		
		// ���ý��̰汾
		void SetProcVer(const string& strProcVer);
		// ��ȡ���̰汾
		string GetProcVer() const;
		
		// ���ý����ļ���С
		void SetProcFileSize(float fProcFileSize);
		// ��ȡ�����ļ���С
		float GetProcFileSize() const;
		
		// �����ļ�����ʱ��
		void SetCreatTime(time_t& tm, WORD wMs = 0);	//	wMs�����루0~999��
		CProtocolTime GetCreatProtocolTime() const;

		void SetCreatTime(timeval tTime);
		timeval GetCreatTime() const;

		// �����ļ��޸�ʱ��
		void SetModifyTime(time_t& tm, WORD wMs = 0);	//	wMs�����루0~999��
		CProtocolTime GetModifyProtocolTime() const;

		void SetModifyTime(timeval tTime);
		timeval GetModifyTime() const;

		// ��������ʱ��
		void SetStartTime(time_t& tm, WORD wMs = 0);	//	wMs�����루0~999��
		CProtocolTime GetStartProtocolTime() const;
		void SetStartTime(timeval tTime);
		timeval GetStartTime() const;

		// ���ý���״̬
		void SetProcState(BYTE byProcState);
		// ��ȡ����״̬
		BYTE GetProcState() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ��������
		string m_strProcName;
		// ����ID
		DWORD m_dwPID;
		// ����·��
		string m_strProcPath;
		// ���̰汾
		string m_strProcVer;
		// �����ļ���С
		float m_fProcFileSize;
		//�����ļ�����ʱ��
		CProtocolTime	m_CreatTime;
		//�����ļ��޸�ʱ��
		CProtocolTime	m_ModifyTime;
		// ����״̬
		BYTE m_byProcState;
		//��������ʱ��
		CProtocolTime	m_StartTime;
	};

	//==============================================================================
	//
	//             CMoniInfoAllHostProc
	//            �̳м�����Ϣ����
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoAllHostProc : public CMoniInfoBase
	{
	public:
		CMoniInfoAllHostProc();
		~CMoniInfoAllHostProc();

	public:
		// ��ȡ������Ϣ����
		int GetAllHostProcItoACount() const;
		// ��ȡָ���±�Ľ�����Ϣ
		CAllHostProcItoA* GetAllHostProcItoA(int nIndex);
		// ��ӽ�����Ϣ
		void Add(const CAllHostProcItoA& objNetCard);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CAllHostProcItoA> m_vecAllHostProcItoA;	//������Ϣ
	};

	//==============================================================================
	//
	//             CHostAllItoAPkt
	//             ȫ������״̬��Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CHostAllItoAPkt : public CProtocolHead
	{
	public:
		CHostAllItoAPkt();
		~CHostAllItoAPkt();

		//���������=�����
		CHostAllItoAPkt& operator=(const CHostAllItoAPkt& objHostAllItoAPkt);
		//�������캯�������
		CHostAllItoAPkt(const CHostAllItoAPkt& objHostAllItoAPkt);

	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;	

		// ��ȡ�����Ϣ����
		int GetMoniInfoBaseCount() const;
		// ��ȡָ���±�ļ����Ϣ
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;
		// ��Ӽ����Ϣ
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//��Ӳ���ϵͳ��Ϣ
		void AddOS(string sOSName);

		// ���CPU������Ϣ
		void AddCPUNum(BYTE byCPUNum);

		//����ڴ�����
		void AddMEMCap(float fMEMCap);

		//��Ӵ�����Ϣ
		void AddDisk(CMoniInfoDisk* pMoniInfoDisk);

		//���������Ϣ
		void AddNetCard(CMoniInfoNetCard* pMoniInfoNetCard);

		//��ӽ�����Ϣ
		void AddProcess(CMoniInfoAllHostProc* pProcess);

	private:
		// ������
		string m_strHostName;
		//�����Ϣ
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};
	
	//==============================================================================
	//
	//             CHostCHGItoAPkt
	//             �仯����״̬��Ϣ��RmInterHost��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CHostCHGItoAPkt : public CHostAllItoAPkt
	{
	public:
		CHostCHGItoAPkt();
		~CHostCHGItoAPkt();
	};

	//==============================================================================
	//
	//             CReqGrpAndHostAllCtoAPkt
	//             ����ȫ���鼰����״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CReqGrpAndHostAllCtoAPkt : public CProtocolHead
	{
	public:
		CReqGrpAndHostAllCtoAPkt();
		~CReqGrpAndHostAllCtoAPkt();
	};

	//==============================================================================
	//
	//             CHostAllAtoC
	//             ����״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CHostAllAtoC : public CObjectBase
	{
	public:
		CHostAllAtoC();
		~CHostAllAtoC();

	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;

		// ��������״̬
		void SetHostState(BYTE byHostState);
		// ��ȡ����״̬
		BYTE GetHostState() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ������
		string m_strHostName;
		// ����״̬
		BYTE m_byHostState;
	};

	//==============================================================================
	//
	//             CGroupAndHostsAtoC
	//             �����鼰��������״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CGroupAndHostsAtoC : public CObjectBase
	{
	public:
		CGroupAndHostsAtoC();
		~CGroupAndHostsAtoC();
		
	public:
		// ������������
		void SetGroupName(string strGroupName);
		// ��ȡ��������
		string GetGroupName() const;
		
		// ������״̬
		void SetGrpState(BYTE byGrpState);
		// ��ȡ��״̬
		BYTE GetGrpState() const;

		// ��ȡ������Ϣ����
		int GetHostAllAtoCCount() const;
		// ��ȡָ���±��������Ϣ
		CHostAllAtoC* GetHostAllAtoC(int nIndex);
		// ������������ȡ�±�
		int GetHostIndexByName(string strName);
		// ���������Ϣ
		void Add(const CHostAllAtoC& objHostAllAtoC);
		void Clear();
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ��������
		string m_strGroupName;
		// ��״̬
		BYTE m_byGrpState;
		// ��������
		vector<CHostAllAtoC> m_vecHostAllAtoC;
	};
	
	//==============================================================================
	//
	//             CGrpAndHostAllAtoCPkt
	//             ȫ���鼰����״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CGrpAndHostAllAtoCPkt : public CProtocolHead
	{
	public:
		CGrpAndHostAllAtoCPkt();
		~CGrpAndHostAllAtoCPkt();
		
	public:
		// ��ȡ��������Ϣ����
		int GetGroupAndHostsAtoCCount() const;
		// ��ȡָ���±����������Ϣ
		CGroupAndHostsAtoC* GetGroupAndHostsAtoC(int nIndex);
		// ��������������ȡ�±�
		int GetHostGrpIndexByName(string strName);
		// �����������Ϣ
		void Add(const CGroupAndHostsAtoC& objGroupAndHostsAtoC);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// �����鼰��������״̬
		vector<CGroupAndHostsAtoC> m_vecGroupAndHostsAtoC;
	};

	//==============================================================================
	//
	//             CGroupAtoC
	//             ������״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CGroupAtoC : public CObjectBase
	{
	public:
		CGroupAtoC();
		~CGroupAtoC();

	public:
		// ������������
		void SetGroupName(string strGroupName);
		// ��ȡ��������
		string GetGroupName() const;

		// ������״̬
		void SetGrpState(BYTE byGrpState);
		// ��ȡ��״̬
		BYTE GetGrpState() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ��������
		string m_strGroupName;
		// ��״̬
		BYTE m_byGrpState;
	};

	//==============================================================================
	//
	//             CGrpCHGAtoCPkt
	//             �仯��״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CGrpCHGAtoCPkt : public CProtocolHead
	{
	public:
		CGrpCHGAtoCPkt();
		~CGrpCHGAtoCPkt();

	public:
		// ��ȡ��������Ϣ����
		int GetGroupAtoCCount() const;
		// ��ȡָ���±����������Ϣ
		CGroupAtoC* GetGroupAtoC(int nIndex);
		// �����������Ϣ
		void Add(const CGroupAtoC& objGroupAtoC);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ������״̬
		vector<CGroupAtoC> m_vecGroupAtoC;
	};

	//==============================================================================
	//
	//             CGroupAllAtoC
	//             ��������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CGroupAllAtoC : public CObjectBase
	{
	public:
		CGroupAllAtoC();
		~CGroupAllAtoC();
		
	public:
		// ������������
		void SetGroupName(string strGroupName);
		// ��ȡ��������
		string GetGroupName() const;
		
		// ��ȡ������Ϣ����
		int GetHostAllAtoCCount() const;
		// ��ȡָ���±��������Ϣ
		CHostAllAtoC* GetHostAllAtoC(int nIndex);
		// ���������Ϣ
		void Add(const CHostAllAtoC& objHostAllAtoC);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ��������
		string m_strGroupName;
		// ������Ϣ
		vector<CHostAllAtoC> m_vecHostAllAtoC;
	};
	
	//==============================================================================
	//
	//             CHostInGrpCHGAtoCPkt
	//             �仯��������״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CHostInGrpCHGAtoCPkt : public CProtocolHead
	{
	public:
		CHostInGrpCHGAtoCPkt();
		~CHostInGrpCHGAtoCPkt();

	public:
		// ��ȡ��������Ϣ����
		int GetGroupAllAtoCCount() const;
		// ��ȡָ���±����������Ϣ
		CGroupAllAtoC* GetGroupAllAtoC(int nIndex);
		// �����������Ϣ
		void Add(const CGroupAllAtoC& objGroupAllAtoC);
		void Clear();
		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ��������Ϣ
		vector<CGroupAllAtoC> m_vecGroupAllAtoC;
	};
	
	//==============================================================================
	//
	//             CSglHost
	//             ������Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHost : public CObjectBase
	{
	public:
		CSglHost();
		~CSglHost();
		
	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;
		
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������
		string m_strHostName;
	};
	
	//==============================================================================
	//
	//             CReqSglHostAllCtoAPkt
	//             ����ȫ�嵥������״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CReqSglHostAllCtoAPkt : public CProtocolHead
	{
	public:
		CReqSglHostAllCtoAPkt();
		~CReqSglHostAllCtoAPkt();
		
	public:
		// ��ȡ������Ϣ����
		int GetSglHostCount() const;
		// ��ȡָ���±��������Ϣ
		CSglHost* GetSglHost(int nIndex);
		// ���������Ϣ
		void Add(const CSglHost& objSglHost);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������Ϣ
		vector<CSglHost> m_vecSglHost;
	};

	//==============================================================================
	//
	//             CSglCpuAndMem
	//             ���������ļ���������̵�CPU���ڴ�ʵʱ״̬��Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CSglCpuAndMem : public CObjectBase
	{
	public:
		CSglCpuAndMem();
		~CSglCpuAndMem();

		//���������=�����
		CSglCpuAndMem& operator=(const CSglCpuAndMem& objSglCpuAndMem);
		//�������캯�������
		CSglCpuAndMem(const CSglCpuAndMem& objSglCpuAndMem);

	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;

		// ������Ϣ��Ч��
		void SetValidity(BYTE byValidity);
		// ��ȡ��Ϣ��Ч��
		BYTE GetValidity() const;

		// ��ȡ�����Ϣ����
		int GetMoniInfoBaseCount() const;
		// ��ȡָ���±�ļ����Ϣ
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ��Ӽ����Ϣ
		// ���CPU������Ϣ
		void AddCPULoad(const float fCPULoad);

		//����ڴ���Ϣ
		void AddMEMLoad(const float fMEMLoad);

		//��ӽ�����Ϣ
		void AddProcess(const CMoniInfoProcess& objProcess);

	private:
		// ������
		string m_strHostName;
		// ��Ϣ��Ч��
		BYTE m_byValidity;
		//�����Ϣ
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//             CCpuAndMemAtoCPkt
	//             ����������̵�CPU���ڴ�ʵʱ״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CCpuAndMemAtoCPkt : public CProtocolHead
	{
	public:
		CCpuAndMemAtoCPkt();
		~CCpuAndMemAtoCPkt();
		
	public:
		// ��ȡ������Ϣ����
		int GetHostAtoCCount() const;
		// ��ȡָ���±��������Ϣ
		CSglCpuAndMem* GetHostAtoC(int nIndex);
		// ���������Ϣ
		void Add(const CSglCpuAndMem& objHostAtoC);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������Ϣ
		vector<CSglCpuAndMem> m_vecHostAtoC; //ʹ��5.1.3���������
	};
	
	//==============================================================================
	//
	//                        CSglHostState
	//       ����״̬��Ϣ������״̬��Ϣ����������ϵͳ��CPU���ڴ桢���̡����硢������Ϣ��
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHostState : public CObjectBase
	{
	public:
		CSglHostState();
		~CSglHostState();

		//���������=�����
		CSglHostState& operator=(const CSglHostState& objSglHostState);
		//�������캯�������
		CSglHostState(const CSglHostState& objSglHostAndService);

	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;

		// ������������
		void SetHostDesc(string strHostDesc);
		// ��ȡ��������
		string GetHostDesc() const;

		// ������Ϣ��Ч��
		void SetValidity(BYTE byValidity);
		// ��ȡ��Ϣ��Ч��
		BYTE GetValidity() const;
		
		// ��ȡ�����Ϣ����
		int GetMoniInfoBaseCount() const;
		// ��ȡָ���±�ļ����Ϣ
		CMoniInfoBase* GetMoniInfoBase(int nIndex) const;

		// ��Ӽ����Ϣ
		void Add(const CMoniInfoBase* pMoniInfoBase);
		void Clear(); 

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//��Ӳ���ϵͳ��Ϣ
		void AddOS(const string sOSName);
		// ���CPU������Ϣ
		void AddCPUNum(const BYTE byCPUNum);
		//����ڴ�����
		void AddMEMCap(const float fMEMCap);
		//��Ӵ�����Ϣ
		void AddDisk(const CMoniInfoDisk* pMoniInfoDisk);
		//���������Ϣ
		void AddNetCard(const CMoniInfoNetCard* pMoniInfoNetCard);
		//��ӽ�����Ϣ
		void AddProcess(const CMoniInfoAllHostProc* pProcess);
		
	private:
		// ������
		string m_strHostName;
		// ��������
		string m_strHostDesc;
		// ��Ϣ��Ч��
		BYTE m_byValidity;	
		//�����Ϣ
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//             CSglHostAllAtoCPkt
	//             ȫ�嵥������״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHostAllCtoAPkt : public CProtocolHead
	{
	public:
		CSglHostAllCtoAPkt();
		~CSglHostAllCtoAPkt();
		
	public:
		// ��ȡ������Ϣ����
		int GetSglHostStateCount() const;
		// ��ȡָ���±��������Ϣ
		CSglHostState* GetSglHostState(int nIndex);
		// ���������Ϣ
		void Add(const CSglHostState& objSglHostState);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);
		
	private:
		// ������Ϣ
		vector<CSglHostState> m_vecSglHostState;
	};
	
	//==============================================================================
	//
	//             CSglHostCHGCtoAPkt
	//            �仯��������״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CSglHostCHGCtoAPkt : public CSglHostAllCtoAPkt
	{
	public:
		CSglHostCHGCtoAPkt();
		~CSglHostCHGCtoAPkt();

	public:
		virtual CProtocolHead* Clone();
	};

	//==============================================================================
	//
	//             CDeviceInfoEx
	//             ���豸�������豸��Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CDeviceInfoEx : public CDeviceInfo
	{
	public:
		CDeviceInfoEx();
		~CDeviceInfoEx();

		bool operator == (CDeviceInfoEx& objOther);

	public:
		// �����豸��ַ����
		void SetDevName(const string& strName);
		// ��ȡ�豸��ַ����
		string GetDevName() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strDevName;			// �豸��ַ����
	};

	//==============================================================================
	//
	//             CProgramClient
	//             �ͻ�����Ϣ��
	//
	//==============================================================================
	class MY_EXT_CLASS CProgramClient : public CForwardAddr, public CObjectBase
	{
	public:
		CProgramClient();
		~CProgramClient();

	public:
		// ���ÿͻ�������
		void SetCltName(const string& strName);
		// ��ȡ�ͻ�������
		string GetCltName() const;

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strCltName;				// �ͻ�������
	};

	//==============================================================================
	//
	//             CClientConnMainCommtoSysEx
	//             �ͻ�����Ϣ��
	//
	//==============================================================================
	class MY_EXT_CLASS CClientConnMainCommtoSysEx : public CObjectBase
	{
	public:
		CClientConnMainCommtoSysEx();
		~CClientConnMainCommtoSysEx();

	public:
		// ���ÿͻ��˶���
		void SetProgramClient(CProgramClient& objClt);
		// ��ȡ�ͻ��˶���ָ��
		CProgramClient* GetProgramClient();

		// ���ÿͻ�������״̬
		void SetConnState(CConnState& objConnState);
		// ��ȡ�ͻ�������״ָ̬��
		CConnState* GetConnState();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		CProgramClient	m_objClt;				// �ͻ��˶���
		CConnState		m_objConnState;			// �ͻ�������״̬
	};

	//==============================================================================
	//
	//             CProgramServerEx
	//             ���������Ϣ��
	//
	//==============================================================================
	class MY_EXT_CLASS CProgramServerEx : public CProgramServer
	{
	public:
		CProgramServerEx();
		~CProgramServerEx();

	public:
		// ��ȡ���������
		string GetSvcName() const;
		// ���÷��������
		void SetSvcName(const string& strName);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		string m_strSvcName;				// ���������
	};

	//==============================================================================
	//
	//			CSvrConnStateEx
	//			������̵�����״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CSvrConnStateEx : public CObjectBase
	{
	public:
		CSvrConnStateEx();
		~CSvrConnStateEx();

	public:
		// ���÷���˳�����Ϣ
		void SetProgramServer(CProgramServerEx& objSvc);
		// ��ȡ����˳�����Ϣ
		CProgramServerEx* GetProgramServer();

		// ���÷����������Ϣ
		void SetConnInfo(CConnInfo& objConnInfo);
		// ��ȡ�����������Ϣ
		CConnInfo* GetConnInfo();

		// ��տͻ�����Ϣ
		void Clear();
		// ��ӿͻ�����Ϣ
		void AddClient(const CClientConnMainCommtoSysEx& objClient);
		// ��ÿͻ�������
		WORD GetClientCount() const;
		// ���ָ���±�Ŀͻ�����Ϣ
		CClientConnMainCommtoSysEx* GetClient(int nIndex);
		// ���ݿͻ��˵�ַ�ͻ�����Ϣ
		CClientConnMainCommtoSysEx* GetClientByAddr(CForwardAddr& objAddr);

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		CProgramServerEx m_objSvc;
		CConnInfo m_objConnInfo;
		vector<CClientConnMainCommtoSysEx> m_vecClientConnMainCommtoSys;
	};

	//==============================================================================
	//
	//			CReqSglSysAllCtoAPkt
	//			����ȫ�嵥��������ϵͳ״̬��Ϣ��RmControlMonitor��RmAppHostMgr��
	//
	//==============================================================================
	class MY_EXT_CLASS CReqSglSysAllCtoAPkt : public CReqSglHostAllCtoAPkt
	{
	public:
		CReqSglSysAllCtoAPkt();
		~CReqSglSysAllCtoAPkt();
	};

	//==============================================================================
	//
	//			CMoniInfoStandbyState
	//			��������״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoStandbyState : public CMoniInfoBase
	{
	public:
		CMoniInfoStandbyState();
		~CMoniInfoStandbyState();

	public:
		// ���÷�������״̬
		void SetState(BYTE byActStdbySta);
		// ��ȡ��������״̬
		BYTE GetState() const;

		// ����״̬�ı�ʱ��
		void SetChangeTime(time_t tm, WORD wMs = 0);	//	wMs�����루0~999��

		void SetChangeProtocolTime(CProtocolTime* tTime);
		CProtocolTime* GetChangeProtocolTime();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		BYTE m_byState;						// ��������״̬
		CProtocolTime m_ChangeTime;			// ����״̬�ı�ʱ��
	};

	//==============================================================================
	//
	//			CMoniInfoSvrConnState
	//			��������״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoSvrConnState : public CMoniInfoBase
	{
	public:
		CMoniInfoSvrConnState();
		~CMoniInfoSvrConnState();

	public:
		// ��ȡ����˸���
		int GetSvrConnStateExCount() const;
		// ��ȡָ���±�ķ������Ϣ
		CSvrConnStateEx* GetSvrConnStateEx(int nIndex);
		// ��ӷ������Ϣ
		void Add(const CSvrConnStateEx& objSvrConnStateEx);
		// ���ݷ����ID��ȡ��������״̬
		CSvrConnStateEx* GetSvrConnStateBySvrId(WORD wSvrId);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//��������״̬
		vector<CSvrConnStateEx> m_vecSvrConnStateEx;
	};

	//==============================================================================
	//
	//             CMoniInfoStaChannelState
	//                  վͨ��״̬
	//
	//==============================================================================
	class MY_EXT_CLASS CMoniInfoStaChannelState : public CMoniInfoBase
	{
	public:
		CMoniInfoStaChannelState();
		~CMoniInfoStaChannelState();

		// ���������=�����
		CMoniInfoStaChannelState& operator=(const CMoniInfoStaChannelState& DeviceSysState);
		// �������캯�������
		CMoniInfoStaChannelState(const CMoniInfoStaChannelState& objDeviceSysState);

	public:
		// ��ȡ��վ����
		int GetStaCommDevCount() const;
		// ��ȡָ���±��վͨ����Ϣ
		CStaCommDev* GetStaCommDev(int nIndex);
		// ���վͨ����Ϣ
		void Add(const CStaCommDev& objStaCommDev);
		vector<CStaCommDev*>& GetAllStaCommDev();
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//��������״̬
		vector<CStaCommDev*> m_vecStaCommDev;
	};
	
	//==============================================================================
	//
	//								CDeviceSysState
	//             �豸��ϵͳ״̬��Ϣ������״̬����������״̬��վͨ��״̬��
	//
	//==============================================================================
	class MY_EXT_CLASS CDeviceSysState : public CObjectBase
	{
	public:
		CDeviceSysState();
		~CDeviceSysState();

		// ���������=�����
		CDeviceSysState& operator=(const CDeviceSysState& DeviceSysState);
		// �������캯�������
		CDeviceSysState(const CDeviceSysState& objDeviceSysState);

	public:
		// �����豸��ַ��Ϣ
		void SetDeviceInfo(CDeviceInfoEx& objDevInfo);
		CDeviceInfoEx* GetDeviceInfo();
		CDeviceInfoEx const* GetDeviceInfo() const;

		// ������Ϣ��Ч��
		void SetValidity(BYTE byValidity);
		// ��ȡ��Ϣ��Ч��
		BYTE GetValidity() const;

		// ��ȡ�����Ϣ����
		int GetMoniInfoCount() const;
		// ��ȡָ���±�ļ����Ϣ
		CMoniInfoBase* GetMoniInfo(int nIndex);
		// ��Ӽ����Ϣ
		void AddMoniInfo(const CMoniInfoBase* pMoniInfoBase);

		void Clear();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		//��ӷ���������״̬
		void AddSvrActStbyState(const CMoniInfoStandbyState* pMoniSvcActStdbySta);
		//��ӷ�������״̬
		void AddSvcConnState(const CMoniInfoSvrConnState* pSvrConnState);
		//���ͨ��״̬
		void AddChanState(const CMoniInfoStaChannelState* pStaCommDev);

	private:
		//�豸��ַ��Ϣ
		CDeviceInfoEx m_objDeviceInfoEx;
		//��Ϣ��Ч��
		BYTE m_byValidity;
		//�����Ϣ
		vector<CMoniInfoBase*> m_vecMoniInfo;
	};

	//==============================================================================
	//
	//             CSglSysAll
	//             ����������ϵͳ��Ϣ
	//
	//==============================================================================
	class MY_EXT_CLASS CSglSysAll : public CObjectBase
	{
	public:
		CSglSysAll();
		~CSglSysAll();

	public:
		// ����������
		void SetHostName(string strHostName);
		// ��ȡ������
		string GetHostName() const;

		// ������������
		void SetHostDesc(string strHostDesc);
		// ��ȡ��������
		string GetHostDesc() const;

		// ��ȡ��ַ����
		int GetDeviceSysStateCount() const;
		// ��ȡָ���±�ĵ�ַ��Ϣ
		CDeviceSysState* GetDeviceSysState(int nIndex);
		// ��ӵ�ַ��Ϣ
		void Add(const CDeviceSysState& objDeviceSysState);
		void Clear();

		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		// ������
		string m_strHostName;
		// ��������
		string m_strHostDesc;

		// �豸��ϵͳ״̬��Ϣ
		vector<CDeviceSysState> m_vecDeviceSysState;
	};

	//==============================================================================
	//
	//             CSglSysAllAtoCPkt
	//             ȫ�嵥��������ϵͳ״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CSglSysAllAtoCPkt : public CProtocolHead
	{
	public:
		CSglSysAllAtoCPkt();
		~CSglSysAllAtoCPkt();

	public:
		// ��ȡ��������
		int GetSglSysAllCount() const;
		// ��ȡָ���±��������Ϣ
		CSglSysAll* GetSglSysAll(int nIndex);
		// ���������Ϣ
		void Add(CSglSysAll& objSglSysAll);
		void Clear();

		virtual CProtocolHead* Clone();
		virtual int StreamSize();
		virtual int ToStreamBuf(char* pBuf, int& nBufLen);
		virtual int FromStreamBuf(const char* pBuf, int nBufLen);

	private:
		vector<CSglSysAll> m_vecSglSysAll;
	};

	//==============================================================================
	//
	//             CSglSysCHGAtoCPkt
	//            �仯����������ϵͳ״̬��Ϣ��RmAppHostMgr��RmControlMonitor��
	//
	//==============================================================================
	class MY_EXT_CLASS CSglSysCHGAtoCPkt : public CSglSysAllAtoCPkt
	{
	public:
		CSglSysCHGAtoCPkt();
		~CSglSysCHGAtoCPkt();
	};
}

#endif
