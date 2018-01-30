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
//                    从系统获取网卡信息类
//
//==============================================================================
class CEthIfs
{
public:
	CEthIfs();
	
	//从系统获取所有网卡信息
	bool InitAllEthIfs();

	//从系统获取所有物理网卡信息（如eth0、eth1等）
	bool InitHwEthIfs();

	//从系统获取所有漂移网卡信息（如eth0:0、eth0:1等）
	bool InitAliasEthIfs();

	//获取网卡信息成功后调用该函数来查找某个ip所在的网卡信息
	ethif* Find(const char* pchIp);
	
	void Dump();

private:
	bool InitEthIfs(int type);
	
private:
	ethif eifs[MAX_INTERFACE_COUNT];
	int eifs_size; //实际有效的网卡个数
};

#endif
