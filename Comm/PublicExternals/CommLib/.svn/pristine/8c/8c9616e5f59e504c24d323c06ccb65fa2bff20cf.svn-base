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
//                    一个网服务端类
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
	/* 下列的实现为服务器地址初始化的相关处理 */
	// 重载基类函数
	virtual string GetServerIdValidRangeStr();
	
	// 重载基类函数
	virtual bool IsServerIdValid();
	
	/* 下列的实现为接入客户端连接建立、关闭的相关处理 */
	// 重载基类函数
	virtual void OnSessionConnect(CSid objSid); 

	// 重载基类函数
	virtual void OnSessionReady(CSid objSid); 

	// 重载基类函数
	virtual void OnSessionClose(CSid objSid); 

	/* 下列的实现为接收数据的相关处理 */
	// 重载基类函数
	// 处理接收的用户数据包。
	// 回传给父对象进行处理。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息
	virtual void HandleUserData(CServerBaseCltObj* pCltObj, CBuffer* pBuf);

	/* 下列的实现为接入客户端对象的相关处理 */
	// 重载基类函数
	virtual CServerBaseCltObj* CreateClientObject();
	
	// 重载基类函数
	virtual string GetCliendIdValidRangeStr();

	// 重载基类函数
	virtual bool IsClientIdValid(CServerSessionBaseCltObj* pCltObj);
	
protected:
	CNetServerDouble* m_pParent;
};

//==============================================================================
//                 
//                    CServerOneNetCltObj
//                  一个网服务端的客户端对象类
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
//                    双网服务端类
//
//==============================================================================
class CNetServerDoubleCltObj;
class MY_EXT_CLASS CNetServerDouble : public CObjectBase
{ 
public: 
	CNetServerDouble();
	virtual ~CNetServerDouble();

protected:
	// 该函数在该类构造后要马上手动调用一次，可以看做构造函数的一部分，
	// 目的是为了在构造过程中执行虚函数（因为构造函数中要避免调用虚函数）
	bool Create();
	
protected:
	/* 下列的实现为接入客户端对象的相关处理 */
	void ConvertCltObjs(vector<int>& vecCltIndex, vector<CServerBaseCltObj*>& vecCltObj1, vector<CServerBaseCltObj*>& vecCltObj2);
	CNetServerDoubleCltObj* GetNetServerDoubleCltObj(CServerOneNetCltObj* pCltObj);

	/* 下列的实现为接收用户数据包的相关处理 */
	// 派生类必须实现。
	// 处理接收的用户数据包。
	// 以下接口传入的buffer（数据包）包含会话层协议头、转发层协议头信息
	virtual void OnDealRecvData(CServerOneNetCltObj* pCltObj, CBuffer* pBuf) = 0;
	
public:
	/* 下列的接口为网络参数初始化的相关处理 */
	// 初始化网络参数。
	bool InitCommParam(TiXmlHandle& txhServer);
	virtual string GetServerIdValidRangeStr(bool bIsAppSvr);
	virtual bool IsServerIdValid(bool bIsAppSvr, WORD wSvrId);
	bool IsRunAsSingleNet(); 
	int GetType();
	
	/* 下列的接口为网络连接启动、关闭、判定的相关处理 */
	bool Startup();
	virtual void OnSessionConnect(CServerOneNetCltObj* pCltObj); 
	virtual void OnSessionReady(CServerOneNetCltObj* pCltObj); 
	virtual void OnSessionClose(CServerOneNetCltObj* pCltObj); 

	/* 下列的接口为通过select机制获取网络消息的相关处理 */
	void SetFdset(CFdset& objFs);
	void SetListenFdset(CFdset& objFs);
	void SetClientFdset(CFdset& objFs);
	void SetClientFdset(CFdset& objFs, vector<int>& vecCltIndex);
	void HandleNetEvent(CFdset& objFs);
	void HandleListenNetEvent(CFdset& objFs);
	void HandleClientNetEvent(CFdset& objFs);
	void HandleClientNetEvent(CFdset& objFs, vector<int>& vecCltIndex);

	/* 下列的接口为接收数据的相关处理 */
	// 处理接收的用户数据包。
	// 处理双网序号、注册包。
	// 传入的buffer（数据包）包含会话层协议头、转发层协议头信息。
	void OnRecvPacket(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	void HandleRegisterPkt(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	void HandleForwardPkt(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	
	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、pBuf、szCltIP、nCltPort、objSid、pCltObj、vecDestAddr均为传入参数。
	bool SendData(const char* szCltIP, unsigned short nCltPort, char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr); 
	bool SendData(const char* szCltIP, unsigned short nCltPort, CBuffer* pBuf, vector<CProtocolForward::CForwardAddr>& vecDestAddr); 
	bool SendData(const char* szCltIP, unsigned short nCltPort, char* szBuf, int nBufLen); 
	bool SendData(const char* szCltIP, unsigned short nCltPort, CBuffer* pBuf); 

	/* 下列的接口为巡检的相关处理 */
	// 周期性调用，负责网络巡检重连、将发送队列中的数据包发送出去
	void DoPatrol();
	void DoPatrol(vector<int>& vecCltIndex);

	/* 下列的接口为接入客户端对象信息获取、管理的相关处理 */
	virtual string GetCliendIdValidRangeStr();
	virtual bool IsClientIdValid(bool bIsAppClt, WORD wCltId);

	/* 下列的接口为日志输出的相关处理 */
	// 在日志中打印基本参数
	string GetDumpParamStr();
	void DumpParam();
	virtual void DumpAllocator();
	
	/* 下列的接口为日志文件操作的相关处理 */
	// 重载基类函数
	// 设置日志文件的标识
	virtual void SetLogFileTag(const char* szFileTag);
	
protected:
	// 包装两个服务端
	CServerOneNet* m_pServer1;
	CServerOneNet* m_pServer2;

	// 双网客户端对象集合，一般来说，是个动态的数组集合
	vector<CNetServerDoubleCltObj> m_vecNetServerDoubleCltObj;
};

//==============================================================================
//                 
//                   CNetServerDoubleCltObj
//                 双网服务端的双网客户端对象类
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
	/* 下列的接口为基本属性设置和获取的相关处理 */
	// cNetNo代表网络编号，1代表第一个网，2代表第二个网，-1代表返回总的情况
	virtual int GetClientId(char cNetNo = 1);
	virtual string GetPeerIp(char cNetNo = 1);
	virtual int GetPeerPort(char cNetNo = 1);
	virtual CCommAddr* GetClientAddr(char cNetNo = -1);
	virtual string GetTagCltInfo(char cNetNo = -1);
	virtual string GetTagConnInfo(char cNetNo = -1);
	
	/* 下列的接口为客户端注册信息获取的相关处理 */
	// cNetNo代表网络编号，1代表第一个网，2代表第二个网，-1代表返回总的情况
	virtual CForwardCondition* GetForwardCondition(char cNetNo = -1);
	virtual bool IsRegistered(CProtocolForward::CForwardHead& objForwardHead);

	/* 下列的接口为客户端网络连接启动、关闭、判定的相关处理 */
	// cNetNo代表网络编号，1代表第一个网，2代表第二个网，-1代表返回总的情况
	virtual bool IsReady(char cNetNo = -1);	

	/* 下列的接口为接收数据包的相关处理 */
	// 响应数据包接收事件，处理双网序列号信息。
	bool DealRecvPacketDataSeq(CServerOneNetCltObj* pCltObj, CBuffer* pBuf);
	
	/* 下列的接口为发送数据包的相关处理。*/
	// 发送转发层数据包。
	// 传入的buffer（数据包）须是完整包，包中不应包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被自动设置双网信息。
	// 传入的数据包将会被自动添加会话层协议头、转发层协议头信息后放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 下列接口用到的szBuf、nBufLen、vecDestAddr均为传入参数。
	bool SendData(char* szBuf, int nBufLen, vector<CProtocolForward::CForwardAddr>& vecDestAddr); 

	// 发送原始数据包。
	// 传入的buffer（数据包）须是完整包，包中已包含会话层协议头、转发层协议头信息。
	// 传入的数据包将会被放入发送队列，在DoPatrol()被调用时会将数据包发送到网络上。 
	// 传入的数据包将会被自动设置双网信息。
	// 下列接口用到的pBuf均为传入参数。
	bool SendRawData(CBuffer* pBuf); 

	/* 下列的接口为客户端对象指针管理的相关处理 */
	CServerOneNetCltObj* GetRelatedCltObjPtr1();
	void SetRelatedCltObjPtr1(CServerOneNetCltObj* p);
	CServerOneNetCltObj* GetRelatedCltObjPtr2();
	void SetRelatedCltObjPtr2(CServerOneNetCltObj* p);
	bool IsEmpty();
	bool IsFull();
	bool AddCltObj(CServerOneNetCltObj* pCltObj);
	bool RemoveCltObj(CServerOneNetCltObj* pCltObj);
	
private: 
	// 管理双网的一对客户端对象指针，这一对指针的对象实体本身存在于其各自的服务端对象中
	CServerOneNetCltObj* m_pRelatedCltObjPtr1; 
	CServerOneNetCltObj* m_pRelatedCltObjPtr2;

	CProtocolForward::CForwardDataSeq m_objRecvSeq1;
	CProtocolForward::CForwardDataSeq m_objRecvSeq2;
	CProtocolForward::CForwardDataSeq m_objRecvSeq;
	
	CProtocolForward::CForwardDataSeq m_objSendSeq;
};


#endif 
