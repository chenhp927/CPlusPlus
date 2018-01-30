//==============================================================================
//                      GetEthif.cpp
//
//begin   : 2016-08-23
//describe:
//==============================================================================

#include "GetEthif.h"

#define getif_trace  1


#define REALDEV  0
#define ALIASDEV 1
#define ALLDEV 2

//extern int getethif(struct ethif *pei, int type, int size);
//extern int getifparam(struct ethif *pei);
//extern bool IsLocalIp(const char* pchIp);


#define getif_exit(x) do{free(ifc.ifc_buf); close(sockfd); return (x);} while(0)
#define c_return(x, y) do{close(x); return (y);}while(0)

int getethif(struct ethif *pei, int type, int size)
{
/*	struct ifconf ifc;
	struct ifreq  ifr, *pifr;
	struct sockaddr_in *psin;
	int  sockfd, len;
	int nif, i, curif;

	bzero(pei, sizeof(struct ethif) * size);

	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0){
		perror("socket");
		CTraceLog::Trace(TLL_ERROR, "socket create err");
		return -1;
	}
	len = size * sizeof(struct ifreq);
	ifc.ifc_buf = (char*)malloc(len);
	if(ifc.ifc_buf == NULL){
		CTraceLog::Trace(TLL_ERROR, "memory not enough when malloc");
		close(sockfd);
		return -1;
	}
	ifc.ifc_len = len;
	if(ioctl(sockfd, SIOCGIFCONF, &ifc) < 0){
		perror("ioctl get conf");
		CTraceLog::Trace(TLL_ERROR, "ioctl get conf err");
		getif_exit(-1);
	}
	nif = ifc.ifc_len / sizeof(struct ifreq);
	curif = 0;
	for(i = 0; i < nif; i++){
		if(curif >= size){
			CTraceLog::Trace(TLL_ERROR, "ethif buf isn't enough(%d)", size);
			break;
		}
		pifr = &ifc.ifc_req[i];
		if(type != ALLDEV)
		{
			if( (strchr(pifr->ifr_name, ':') && type != ALIASDEV) ||
			    (!strchr(pifr->ifr_name, ':') && type != REALDEV) )
				continue;
		}

		memcpy(&ifr, pifr, sizeof(struct ifreq));
		if(ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0){
			perror("ioctl get flags");
			CTraceLog::Trace(TLL_ERROR, "ioctl get flags err");
			getif_exit(-1);
		}
		if(type != ALLDEV)
		{
			if( (ifr.ifr_flags & IFF_POINTOPOINT) ||
			    (ifr.ifr_flags & IFF_LOOPBACK))
				continue;
		}

		strcpy((char*)pei[curif].ifname, pifr->ifr_name);
		psin = (struct sockaddr_in*)&pifr->ifr_addr;
		strcpy((char*)pei[curif].ip, (char*)inet_ntoa(psin->sin_addr));

		memcpy(&ifr, pifr, sizeof(struct ifreq));
		if(ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0){
			perror("ioctl get netmask");
			CTraceLog::Trace(TLL_ERROR, "ioctl get netmask err");
			getif_exit(-1);
		}
		psin = (struct sockaddr_in*)&ifr.ifr_addr;
		strcpy(pei[curif].netmask, (char*)inet_ntoa(psin->sin_addr));

		memcpy(&ifr, pifr, sizeof(struct ifreq));
		if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0){
			perror("ioctl get hwaddr");
			CTraceLog::Trace(TLL_ERROR, "ioctl get hwaddr err");
			getif_exit(-1);
		}
		memcpy((char*)pei[curif].mac, (char*)ifr.ifr_hwaddr.sa_data, 6);

		memcpy(&ifr, pifr, sizeof(struct ifreq));
		if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0){
			perror("ioctl get ifindex");
			CTraceLog::Trace(TLL_ERROR, "ioctl get ifindex err");
			getif_exit(-1);
		}
		pei[curif].ifindex = ifr.ifr_ifindex;
		if(getif_trace){
			CTraceLog::Trace(TLL_IMPORTANT, "getethif  %s: %s %s ", pei[curif].ifname, 
				pei[curif].ip, pei[curif].netmask);
			CTraceLog::Trace(TLL_IMPORTANT, " hw: %x:%x:%x:%x:%x:%x",
				pei[curif].mac[0], pei[curif].mac[1],
				pei[curif].mac[2], pei[curif].mac[3],
				pei[curif].mac[4], pei[curif].mac[5]);
		}
		curif++;
	}
	getif_exit(curif);*/
}

int getifparam(struct ethif *pei)
{
/*	struct ifreq  ifr;
	struct sockaddr_in *psin;
	int  sockfd;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0){
		perror("socket");
		return -1;
	}
	bzero(&ifr, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, pei->ifname);
	if(ioctl(sockfd, SIOCGIFADDR, &ifr) < 0){
		perror("ioctl get addr");
		c_return(sockfd, -1);
	}
	psin = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy((char*)pei->ip, (char*)inet_ntoa(psin->sin_addr));

	if(ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0){
		perror("ioctl get netmask");
		c_return(sockfd, -1);
	}
	psin = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy(pei->netmask, (char*)inet_ntoa(psin->sin_addr));

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0){
		perror("ioctl get hwaddr");
		c_return(sockfd, -1);
	}
	memcpy((char*)pei->mac, (char*)ifr.ifr_hwaddr.sa_data, 6);

	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0){
		perror("ioctl get netmask");
		c_return(sockfd, -1);
	}
	pei->ifindex = ifr.ifr_ifindex;
	if(getif_trace){
		printf("getethif  %s: %s %s ", pei->ifname, 
			pei->ip, pei->netmask);
		printf(" hw: %02X:%02X:%02X:%02X:%02X:%02X\n",
			pei->mac[0], pei->mac[1],
			pei->mac[2], pei->mac[3],
			pei->mac[4], pei->mac[5]);
	}
	c_return(sockfd, 1);
*/}

//==============================================================================
//                 
//                    CEthIfs
//                    从系统获取网卡信息类
//
//==============================================================================
CEthIfs::CEthIfs()
{
	memset(eifs, sizeof(eifs), 0);
	eifs_size = 0;
}

bool CEthIfs::InitEthIfs(int type)
{
	memset(eifs, sizeof(eifs), 0);
	eifs_size = 0;

	int n = getethif(eifs, type, MAX_INTERFACE_COUNT);
	if(n < 0)
	{
		memset(eifs, sizeof(eifs), 0);
		eifs_size = 0;
		return false;
	}
	eifs_size = n;
	return true;
}

bool CEthIfs::InitAllEthIfs()
{
	return InitEthIfs(ALLDEV);
}

bool CEthIfs::InitHwEthIfs()
{
	return InitEthIfs(REALDEV);
}

bool CEthIfs::InitAliasEthIfs()
{
	return InitEthIfs(ALIASDEV);
}

ethif* CEthIfs::Find(const char* pchIp)
{
	if(!pchIp)
		return 0;

	for(int i=0; i<eifs_size; i++)
	{
		if(strcmp(pchIp, eifs[i].ip) == 0)
			return &eifs[i];
	}
	return 0;
}

void CEthIfs::Dump()
{
/*	CTraceLog::Trace(TLL_IMPORTANT, "eth ifs dump:");
	for(int i=0; i<eifs_size; i++)
	{
		CTraceLog::Trace(TLL_IMPORTANT, "ifname(%s): ip=%s, netmask=%s ifindex=%d mac=%x:%x:%x:%x:%x:%x", 
			eifs[i].ifname, eifs[i].ip, eifs[i].netmask, eifs[i].ifindex,
			eifs[i].mac[0], eifs[i].mac[1],
			eifs[i].mac[2], eifs[i].mac[3],
			eifs[i].mac[4], eifs[i].mac[5]);
	}
*/}
