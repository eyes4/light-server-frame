/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/network/linux_socket.cpp $
 *
 *  @brief linux socketœ‡πÿ
 *
 *  $Id: linux_socket.cpp 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "common_socket.h"
#include <errno.h>
bool SocketInit(void)
{
  return true;
}

void SocketUninit(void)
{
}

int GetHostName(char *szName, int nSize)
{
  return gethostname(szName, nSize);
}

hostent *GetHostByName(char *szName)
{
  return (hostent *)gethostbyname(szName);
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
} /* SocketSetTypeOfService */

SOCKET SocketBind (int nSockType, char *szIP, unsigned short nPort)
{
    // set up sockaddr_in and try to bind it to the socket
  int s;
  struct sockaddr_in sa;
  int sockoptval = 1;

  s = socket(AF_INET, nSockType, 0);
  if (s < 0)
  return -1;

  //bFlag = true; //…Ë÷√Ã◊Ω”◊÷—°œÓ£¨ πÃ◊Ω”◊÷Œ™ø…÷ÿ”√∂Àø⁄µÿ÷∑
  if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(sockoptval)) != 0)
  {
    close(s);
    return -1;
  }

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(nPort);
  sa.sin_addr.s_addr = inet_addr(szIP);
 
  if (bind(s, (struct sockaddr*)&sa, sizeof(sa)) != 0)
  {
    close(s);
        return -1;
  }

  return s;
}

SOCKET SocketBindAll(int nSockType, unsigned short nPort)
{
    // set up sockaddr_in and try to bind it to the socket
  int s;
  struct sockaddr_in sa;
  int nFlag;

  s = socket(AF_INET, nSockType, 0);
  if (s < 0)
  return -1;

  nFlag = 1; //ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ πÔøΩÔøΩÔøΩÔøΩÔøΩ√∂Àø⁄µÔøΩ
  if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &nFlag, sizeof(nFlag)) == -1)
  {
    close(s);
    return -1;
  }

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(nPort);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
 
  if (bind(s, (struct sockaddr*)&sa, sizeof(sa)) != 0)
  {
    close(s);
        return -1;
  }
  return s;
}

bool SocketListen(SOCKET sSocket)
{
  if (listen(sSocket, SOMAXCONN) != 0)
  {
    close(sSocket);
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

SOCKET SocketAccept(SOCKET sSocket, char *szAddr, unsigned short *pPort)
{
  struct sockaddr_in addrClient;
  SOCKET s;
  socklen_t len = sizeof(addrClient);
  s = accept(sSocket, (struct sockaddr *)&addrClient, &len);
  sprintf(szAddr, "%s", inet_ntoa(addrClient.sin_addr));
  *pPort = ntohs(addrClient.sin_port);
  return s;
}

int SocketRecv(SOCKET sSocket, void *pBuff, int nLen)
{
  return recv(sSocket, pBuff, nLen, 0);
}

int SocketRecvFrom(SOCKET sSocket, void *pBuff, int nLen, char *szAddr, unsigned short *pPort)
{
  struct sockaddr_in server;  
  socklen_t len = sizeof(server);  
  int r = recvfrom(sSocket, pBuff, nLen, 0, (struct sockaddr*)&server, &len);
  if (r >= 0)
  {
    sprintf(szAddr, "%s", inet_ntoa(server.sin_addr));
    *pPort = ntohs(server.sin_port);
  }
  else
  {
    //printf("%d\n", WSAGetLastError());
  }
  return r;
}

int SocketSend(SOCKET sSocket, void *pBuff, int nLen)
{
  return send(sSocket, pBuff, nLen, 0);
}

int SocketSendTo(SOCKET sSocket, void *pBuff, int nLen, char *szAddr, unsigned short nPort)
{
  struct sockaddr_in server;
  int len = sizeof(server);
  memset(&server, 0, len);
  server.sin_family=AF_INET;  
  server.sin_port=htons(nPort);
  server.sin_addr.s_addr=inet_addr(szAddr);  

  return sendto(sSocket, pBuff, nLen, 0,(struct sockaddr*)&server,len);
}

int SocketSendToSvr(SOCKET sSocket, void *pBuff, int nLen, struct sockaddr_in server, int len)
{
  return sendto(sSocket, pBuff, nLen, 0,(struct sockaddr*)&server,len);
}
bool SocketSetBroadcast(SOCKET sSocket)
{
  char cFlag = 1;
  int nRet = setsockopt(sSocket, SOL_SOCKET, SO_BROADCAST, (char *)&cFlag, sizeof(cFlag));
  if(-1 == nRet)
  {
    return false;
  }
  return true;
}

SOCKET SocketConnect(char *szIP, unsigned short nPort, char *szLocalIP, unsigned short nLocalPort)
{
  SOCKET sSocket;
  struct sockaddr_in server;  
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(nPort);
  server.sin_addr.s_addr = inet_addr(szIP);  

    //if ((sSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    //    return -1;
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
    close(sSocket);
    return -1;
  }
    return sSocket;
}

bool SocketClose(SOCKET sSocket)
{
  return close(sSocket) == 0?true:false;
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
          PhysAddrToString(pIpNetTable->table[i].bPhysAddr,   pIpNetTable->table[i].dwPhysAddrLen,  
szPrintablePhysAddr);  
      printf("mac :%s\n" , szPrintablePhysAddr);
      }
  }
  return false;*/
  
  /*unsigned   char   macAddress[6];
  int i;
                  ULONG   macAddLen   =   6;  
                int  iRet=SendARP(inet_addr(szIP),   (unsigned   long)NULL,(PULONG)&macAddress,  
&macAddLen);
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
*/
        return true;

}

int SocketRandPort()
{
  struct sockaddr_in sin;
  int fd;
  int nRetry=0, port;
  socklen_t len;
  int sockoptval=1;
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
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
      continue;
    }
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(sockoptval)) != 0)
    {
      close(fd);
      continue;
    }
    if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) != 0)
    {
      close(fd);
      continue;
    }
    len = sizeof(sin);
    if(getsockname(fd, (struct sockaddr *)&sin, &len) != 0)
    {
      close(fd);
      continue;
    }
    port = sin.sin_port;
    if(fd != -1)
      close(fd);
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
  
  
  register int fd, intrface;
  struct ifreq buf[MAXINTERFACES];
  struct ifconf ifc;

  if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
  {
     return NULL;
  }

  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = (caddr_t) buf;
  if (ioctl (fd, SIOCGIFCONF, (char *) &ifc) < 0)
  {
    return NULL;
  }

  intrface = ifc.ifc_len/sizeof(struct ifreq);
  while (intrface-- > 0)
  {
    if ((ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface])) < 0)
    {
      continue;
    }
    if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
    {
      //DEBUGMSG("local ip:%s\n", inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
      if (szLocalIPList[0]==0)
        sprintf(szLocalIPList, 
                "%s\n",
                inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
      else
        sprintf(szLocalIPList, 
                "%s%s\n", szLocalIPList,
                inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
    }
    else
      continue;
  }
  
  close(fd);
  return szLocalIPList;
}


