//==============================================================================
//                      GetEthif.h
//
//begin   : 2016-08-23
//describe:
//==============================================================================
#ifndef SELFDEF_GETIFLIST_H 
#define SELFDEF_GETIFLIST_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <arpa/inet.h>

#include "ObjectBase.h"

#define _ETH_NAME_LEN  16
#define _IP_LEN        16
#define _MAC_LEN       6 

#define MAX_INTERFACE_COUNT 16

struct ethif
{
	char ifname[_ETH_NAME_LEN];
	char ip[_IP_LEN];
	unsigned char mac[_MAC_LEN];
	char netmask[_IP_LEN];
	int ifindex;
};

//==============================================================================
//                 
//                    CEthIfs
//                    ��ϵͳ��ȡ������Ϣ��
//
//==============================================================================
class CEthIfs
{
public:
	CEthIfs();
	
	//��ϵͳ��ȡ����������Ϣ
	bool InitAllEthIfs();

	//��ϵͳ��ȡ��������������Ϣ����eth0��eth1�ȣ�
	bool InitHwEthIfs();

	//��ϵͳ��ȡ����Ư��������Ϣ����eth0:0��eth0:1�ȣ�
	bool InitAliasEthIfs();

	//��ȡ������Ϣ�ɹ�����øú���������ĳ��ip���ڵ�������Ϣ
	ethif* Find(const char* pchIp);
	
	void Dump();

private:
	bool InitEthIfs(int type);
	
private:
	ethif eifs[MAX_INTERFACE_COUNT];
	int eifs_size; //ʵ����Ч����������
};

#endif
