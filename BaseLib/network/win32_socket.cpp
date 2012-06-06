/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/network/win32_socket.cpp $
 *
 *  @brief win32 socket相关
 *
 *  $Id: win32_socket.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "../include/common_socket.h"

bool SocketInit(void)
{
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 0);

    err = WSAStartup(wVersionRequested, &wsaData);
    if ( err != 0 )
        return false;

    if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0 )
        return false;
	return true;
}

void SocketUninit(void)
{
	WSACleanup();
}

int GetHostName(char *szName, int nSize)
{
	return gethostname(szName, nSize);
}

hostent *GetHostByName(char *szName)
{
	return gethostbyname(szName);
}

char * SocketGetIPByName(char *szAddr)
{
	//static szIP[64];
	hostent* h;
	h = GetHostByName(szAddr);
	if (h != NULL)
		return inet_ntoa(*((struct   in_addr   *)h->h_addr));
	else
		return NULL;
}

int SocketSetTypeOfService(const SOCKET sSocket, const int tosType)
{
	int opt;

	opt = tosType;
	return (setsockopt(sSocket, IPPROTO_IP, IP_TOS, (char *) &opt,  sizeof(opt)));
}	/* SocketSetTypeOfService */


/*#define SOCK_STREAM     1               // stream socket 
#define SOCK_DGRAM      2                //datagram socket 
#define SOCK_RAW        3                //raw-protocol interface 
#define SOCK_RDM        4                //reliably-delivered message 
#define SOCK_SEQPACKET  5                //sequenced packet stream
*/
SOCKET SocketBind (int nSockType, char *szIP, unsigned short nPort)
{
    // set up sockaddr_in and try to bind it to the socket
	SOCKET s;
    struct sockaddr_in sa;
	bool bFlag;

	s = socket(AF_INET, nSockType, IPPROTO_IP);
    if (s < 0)
		return -1;

	bFlag = true; //设置套接字选项，使套接字为可重用端口地址
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&bFlag, sizeof(bFlag)) == SOCKET_ERROR)
	{
		closesocket(s);
		return -1;
	}

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(nPort);
    sa.sin_addr.s_addr = inet_addr(szIP);
    
    if (bind(s, (struct sockaddr*)&sa, sizeof(sa)) != 0)
	{
		closesocket(s);
        return -1;
	}

	return s;
}

SOCKET SocketBindAll(int nSockType, unsigned short nPort)//SOCK_STREAM SOCK_DGRAM
{
    // set up sockaddr_in and try to bind it to the socket
	SOCKET s;
    struct sockaddr_in sa;
	bool bFlag;

	s = socket(AF_INET, nSockType, 0);
    if (s < 0)
		return -1;

	bFlag = true; //设置套接字选项，使套接字为可重用端口地址
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&bFlag, sizeof(bFlag)) == SOCKET_ERROR)
	{
		closesocket(s);
		return -1;
	}

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(nPort);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(s, (struct sockaddr*)&sa, sizeof(sa)) != 0)
	{
		closesocket(s);
        return -1;
	}
	return s;
}

bool SocketListen(SOCKET sSocket)
{
	if (listen(sSocket, SOMAXCONN) != 0)
	{
		closesocket(sSocket);
		return false;
	}
	return true;
}

void SocketFD_CLR(SOCKET sSocket, fd_set *psFdSet)
{
	FD_CLR(sSocket, psFdSet);
}

void SocketFD_ZERO(fd_set *psFdSet)
{
	FD_ZERO(psFdSet);
}

int SocketFD_ISSET(SOCKET sSocket, fd_set *psFdSet)
{
	return FD_ISSET(sSocket, psFdSet);
}

void SocketFD_SET(SOCKET sSocket, fd_set *psFdSet)
{
	FD_SET(sSocket, psFdSet);
}

int SocketSelect(SOCKET sSocket, fd_set *psFdSet, int nTimeOut)
{
	struct timeval t;
    
    t.tv_sec = nTimeOut / 1000;
    t.tv_usec = (nTimeOut % 1000) * 1000;
    return select((int)sSocket, psFdSet, NULL, NULL, (nTimeOut < 0) ? NULL : &t);
}

int SocketWaitForRead(SOCKET sSocket, int nTimeOut) //-1:error 0-timeout 1-ok
{

    fd_set fdSet;
	int err;
	
	SocketFD_ZERO(&fdSet);
	SocketFD_SET(sSocket, &fdSet);
	err = SocketSelect(sSocket + 1, &fdSet, nTimeOut);
	if (-1 == err) return -1;
	else if (0 == err) return 0;
	else 
	{
		if (SocketFD_ISSET(sSocket, &fdSet))
			return 1;
		else
			return 0;
	}
}

SOCKET SocketAccept(SOCKET sSocket, char *szAddr, unsigned short *pPort)
{
	struct sockaddr_in addrClient;
	SOCKET s;
	int len = sizeof(addrClient);
	s = accept(sSocket, (struct sockaddr *)&addrClient, &len);
	sprintf(szAddr, "%s", inet_ntoa(addrClient.sin_addr));
	*pPort = ntohs(addrClient.sin_port);
	return s;
}

int SocketRecv(SOCKET sSocket, void *pBuff, int nLen)
{
	return recv(sSocket, (char *)pBuff, nLen, 0);
}

int SocketRecvFrom(SOCKET sSocket, void *pBuff, int nLen, char *szAddr, unsigned short *pPort)
{
	struct sockaddr_in server;   
	int len = sizeof(server);   
	int r = recvfrom(sSocket, (char *)pBuff, nLen, 0, (struct sockaddr*)&server, &len);
	if (r >= 0)
	{
		sprintf(szAddr, "%s", inet_ntoa(server.sin_addr));
		*pPort = ntohs(server.sin_port);
	}
	else
	{
		printf("%d\n", WSAGetLastError());
	}
	return r;
}

int SocketSend(SOCKET sSocket, void *pBuff, int nLen)
{
	return send(sSocket, (const char *)pBuff, nLen, 0);
}

int SocketSendTo(SOCKET sSocket, void *pBuff, int nLen, char *szAddr, unsigned short nPort)
{
	struct sockaddr_in server;
	int len = sizeof(server);
	memset(&server, 0, len);
	server.sin_family=AF_INET;   
	server.sin_port=htons(nPort); 
	server.sin_addr.s_addr=inet_addr(szAddr);   

	return sendto(sSocket, (const char *)pBuff, nLen, 0,(struct sockaddr*)&server,len);
}

int SocketSendToSvr(SOCKET sSocket, void *pBuff, int nLen, struct sockaddr_in server, int len)
{
	return sendto(sSocket, (const char *)pBuff, nLen, 0,(struct sockaddr*)&server,len);
}

bool SocketSetBroadcast(SOCKET sSocket)
{
	char cFlag = 1;
		int nRet = setsockopt(sSocket, SOL_SOCKET, SO_BROADCAST, (char *)&cFlag, sizeof(cFlag));
		if(SOCKET_ERROR == nRet)
		{
			//sprintf( m_pszError, "Winsock error : %s (Error Code %d)\n", "Can't allow transmission of broadcast messages on the socket.", WSAGetLastError());
			return false;
		}
		return true;
}

int SocketConnect(char *szIP, unsigned short nPort, char *szLocalIP, unsigned short nLocalPort)
{
	SOCKET sSocket;
	struct sockaddr_in server;   
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
    server.sin_port = htons(nPort);
    server.sin_addr.s_addr = inet_addr(szIP);   

	//注意，socket重复创建
    if ((sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return -1;
	if (strcmp(szLocalIP, "0.0.0.0") == 0)
	{
		sSocket = SocketBindAll(SOCK_STREAM, nLocalPort);
	}
	else
	{
		sSocket = SocketBind(SOCK_STREAM, szLocalIP, nLocalPort);
	}
	if (sSocket < 0) return -1;
    if (connect(sSocket, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
		closesocket(sSocket);
        return -1;
    }
    return sSocket;
}

int SocketConnectTimer(char *szIP, unsigned short nPort, char *szLocalIP, unsigned short nLocalPort, int nTimeOut)
{
	SOCKET sSocket = INVALID_SOCKET;
	int nRet = -1;
	BOOL bConnected = FALSE;
	struct sockaddr_in server;   
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
    server.sin_port = htons(nPort);
    server.sin_addr.s_addr = inet_addr(szIP);   

	//注意socket重复创建
    if ((sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return -1;

	int nFlag = 0;
	//设置接收超时  
	if(::setsockopt(sSocket,SOL_SOCKET,SO_SNDTIMEO,(char   *)&nTimeOut,sizeof(nTimeOut))==SOCKET_ERROR)
	{   
		return   0;   
	}     
 
	unsigned long uFlag = 1;
	nRet = ioctlsocket(sSocket,FIONBIO,(unsigned long*)&uFlag);
	if(nRet == SOCKET_ERROR)
	{
	   return -1;
	}
	DEBUGMSG("to connect:%d\n", sSocket);
	if(connect(sSocket,(SOCKADDR*)&server,sizeof(server)) != 0)
	{
		struct timeval timeout = {0};
		timeout.tv_sec = nTimeOut/1000;
		timeout.tv_usec = nTimeOut % 1000;

		fd_set fdWrite;
		FD_ZERO(&fdWrite);
		FD_SET(sSocket,&fdWrite);

		int nError = -1;
		int nLen = sizeof(int);

		nRet = select(sSocket,0,&fdWrite,0,&timeout);
		if (nRet > 0)
		{
			DEBUGMSG("nRet > 0\n");
			getsockopt(sSocket,SOL_SOCKET,SO_ERROR,(char*)&nError,&nLen);
			DEBUGMSG("nError:%d\n", nError);
			if (nError != 0)
			{
				bConnected = FALSE;
			}
			bConnected = TRUE;
		}
		else
		{
			bConnected = FALSE;
		} 
	}
	else
		bConnected = TRUE;
	nFlag = 0;
	ioctlsocket(sSocket,FIONBIO,(unsigned long*)&nFlag);
	if (bConnected)
		return sSocket;
	else
		return -2;
}

bool SocketClose(SOCKET sSocket)
{
	return closesocket(sSocket) == 0?true:false;
}

int Socketfprintf(SOCKET sSocket, const char *fmt, ...)   
{   
	va_list args;   
	char temp[SOCKETMAXBUFSIZE];   
	int ret;   
	va_start(args, fmt);   
	ret = vsprintf(temp, fmt, args);   
	va_end(args);   
	if(send(sSocket, temp, strlen(temp), 0) == SOCKET_ERROR)   
		return -1;   
	return ret;   
}   
int Socketfgets(char *pBuff, int nMaxLen, SOCKET sSocket)   
{   
	int ret, count = 0;   
	char *tempbuf = pBuff;   
	while(1)   
	{   
		++count;   
		if(count > nMaxLen)   
			return count-1;   
		ret = recv(sSocket, tempbuf, 1, 0);   
		if((ret == SOCKET_ERROR) || (ret <= 0))   
			return 0;   
		if(*tempbuf == '\n')   
		{   
			++tempbuf;   
			*tempbuf = 0;   
			return count;   
		}   
		++tempbuf;   
	}   
}  


  bool   InterfaceIdxToInterfaceIp(PMIB_IPADDRTABLE   pIpAddrTable,   DWORD   dwIndex,   char   str[])   
  {   
          struct   in_addr   inadTmp;   
          char*   szIpAddr;   
			DWORD   dwIdx;
          if   (pIpAddrTable   ==   NULL   ||     str   ==   NULL)   
                  return   FALSE;   
          str[0]   =   '\0';   
          for   ( dwIdx   =   0;   dwIdx   <   pIpAddrTable->dwNumEntries;   dwIdx++)   
          {   
                  if   (dwIndex   ==   pIpAddrTable->table[dwIdx].dwIndex)   
                  {   
                          inadTmp.s_addr   =   pIpAddrTable->table[dwIdx].dwAddr;   
                          szIpAddr   =   inet_ntoa(inadTmp);   
                          if   (szIpAddr)   
                          {   
                                  strcpy(str,   szIpAddr);   
                                  return   TRUE;   
                          }   
                          else   
                                  return   FALSE;   
                  }   
          }   
          return   FALSE;   
    
  } 

   bool   PhysAddrToString(BYTE   PhysAddr[],   DWORD   PhysAddrLen,   char   str[])   
  {   
	  DWORD   dwIdx;
          if   (PhysAddr   ==   NULL   ||   PhysAddrLen   ==   0   ||   str   ==   NULL)   
                  return   FALSE;   
          str[0]   =   '\0';   
          for   (  dwIdx   =   0;   dwIdx   <   PhysAddrLen;   dwIdx++)   
          {   
                  if   (dwIdx   ==   PhysAddrLen-1)   
                          sprintf(str+(dwIdx*3),   "%02X",   ((int)PhysAddr[dwIdx])&0xff);   
                  else   
                          sprintf(str+(dwIdx*3),   "%02X-",   ((int)PhysAddr[dwIdx])&0xff);   
                    
          }   
          return   TRUE;   
  }
bool SocketGetMacByIP(char *szIP, char *szMac)
{
	/*MIB_IPNETTABLE  IpNetTable;
	MIB_IPNETTABLE  *pIpNetTable = (MIB_IPNETTABLE *)malloc(246);
	DWORD   status   =   NO_ERROR;   
	//DWORD   statusRetry   =   NO_ERROR;   
	DWORD   dwActualSize   =   0;   
	          DWORD   i,   dwStatus,   dwCurrIndex;   
          struct   in_addr   inadTmp;   
          char   szPrintablePhysAddr[256];   
          char   szType[128];   
          char   szIpAddr[128];   
          PMIB_IPADDRTABLE   pIpAddrTable   =   NULL;   

	//   query   for   buffer   size   needed   
	dwActualSize   =   0;   
	status   =   GetIpNetTable(&IpNetTable,   &dwActualSize,  true);  
	if (status   ==   ERROR_INSUFFICIENT_BUFFER)   
	{   
		pIpNetTable   =   (PMIB_IPNETTABLE)   malloc(dwActualSize);   

		status   =   GetIpNetTable(pIpNetTable,   &dwActualSize,   true);   

		if   (status   !=   NO_ERROR)   
		{   
			return false; 
		}   
	}   
	if (status == NO_ERROR)
	{
		for   (i   =   0;   i   <   pIpNetTable->dwNumEntries;   ++i)   
          {    
          PhysAddrToString(pIpNetTable->table[i].bPhysAddr,   pIpNetTable->table[i].dwPhysAddrLen,   szPrintablePhysAddr);   
		  printf("mac :%s\n" , szPrintablePhysAddr);
		  }
	}
	return false;*/
	
	unsigned   char   macAddress[6]; 
	int i;
                  ULONG   macAddLen   =   6;   
                int  iRet=SendARP(inet_addr(szIP),   (unsigned   long)NULL,(PULONG)&macAddress,   &macAddLen); 
				if   (  iRet   ==   NO_ERROR   )   
                  {   
                          printf(   "MacAddress:   "   );   
                          for(     i   =0;   i<6;   i++   )   
                          {   
                                  printf(   "%.2x",   macAddress[i]   );   
                                  if   (   i<5   )   printf(   "-"   );   
                          }   
                          printf(   "\n"   );   
                  }   
else   
                          printf(   "SendARP   Error:%d\n",   GetLastError());   
                          //nRemoteAddr   =   htonl(   ntohl(   nRemoteAddr   )   +   1   );   

				return true;

}

int SocketRandPort()
{
	struct sockaddr_in sin;
	SOCKET fd;
	int nRetry=0, port, len;
	char nDontLinger = 0;
	//int min = 0x1000;
	//int max = 0x9FFF;

	//int ret = Rand();
	//ret = ret | min;
	//ret = ret & max;
	//return ret;

	while(nRetry < 3)
	{
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(0);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		nRetry ++;
		fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(fd < 0)
		{
			continue;
		}
		if(setsockopt(fd, SOL_SOCKET, SO_DONTLINGER, (const char*)&nDontLinger, sizeof(nDontLinger)) != 0)
		{
			closesocket(fd);
			continue;
		}
		if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) != 0)
		{
			closesocket(fd);
			continue;
		}
		len = sizeof(sin);
		if(getsockname(fd, (struct sockaddr *)&sin, &len) != 0)
		{
			closesocket(fd);
			continue;
		}
		port = sin.sin_port;
		if(fd != -1)
			closesocket(fd);
		return port;
	}
	return -1;
}

char *GetLocalIp()
{
	struct hostent * phost;
	char ip[20];
	char hostname[50];
	char **names;
	char **iplist;
	static char szLocalIPList[1024];
	memset(szLocalIPList, 0, 1024);
	gethostname(hostname, 50);
	phost = gethostbyname(hostname);
	
	names = phost->h_aliases;
	
	iplist = phost->h_addr_list;
	while (* iplist)
	{
		if (szLocalIPList[0]==0)
			sprintf(szLocalIPList, "%s\n", inet_ntoa(* (struct in_addr *) * iplist));
		else
			sprintf(szLocalIPList, "%s%s\n", szLocalIPList, inet_ntoa(* (struct in_addr *) * iplist));
		iplist++;
	}
	
	return szLocalIPList;
}




