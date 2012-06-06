/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_socket.h $
 *
 *  @brief socket相关函数头文件
 *
 *  $Id: common_socket.h 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include "common.h"

#ifdef WIN32 
#include "win32_socket.h"
#endif

#ifdef _LINUX
#include <sys/select.h>
#include "linux_socket.h"
#endif


#define SOCKETMAXBUFSIZE 1024

#ifndef IPTOS_LOWDELAY
#define IPTOS_LOWDELAY		0x10
#define IPTOS_THROUGHPUT		0x08
#define IPTOS_RELIABILITY	0x04
#endif

extern bool SocketInit(void);
extern void SocketUninit(void);
extern int GetHostName(char *szName, int nSize);
extern hostent *GetHostByName(char *szName);
extern SOCKET SocketBind(int nSockType, char *szIP, unsigned short nPort);
extern SOCKET SocketBindAll(int nSockType, unsigned short nPort);
extern bool SocketListen(SOCKET sSocket);
extern void SocketFD_CLR(SOCKET sSocket, fd_set *psFdSet);
extern void SocketFD_ZERO(fd_set *psFdSet);
extern int SocketFD_ISSET(SOCKET sSocket, fd_set *psFdSet);
extern void SocketFD_SET(SOCKET sSocket, fd_set *psFdSet);
extern int SocketSelect(SOCKET sSocket, fd_set *psFdSet, int nTimeOut);
extern SOCKET SocketAccept(SOCKET sSocket, char *szAddr, unsigned short *pPort);
extern int SocketRecv(SOCKET sSocket, void *pBuff, int nLen);
extern int SocketRecvFrom(SOCKET sSocket, void *pBuff, int nLen, char *szAddr, unsigned short *pPort);
extern int SocketSend(SOCKET sSocket, void *pBuff, int nLen);
extern int SocketSendTo(SOCKET sSocket, void *pBuff, int nLen, char *szAddr, unsigned short nPort);
extern int SocketSendToSvr(SOCKET sSocket, void *pBuff, int nLen, struct sockaddr_in server, int len);
extern int Socketfprintf(SOCKET sSocket, const char *fmt, ...); 
extern int Socketfgets(char *pBuff, int nMaxLen, SOCKET sSocket);
extern int SocketConnect(char *szIP, unsigned short nPort, char *szLocalIP, unsigned short nLocalPort);
extern int SocketConnectTimer(char *szIP, unsigned short nPort, char *szLocalIP, unsigned short nLocalPort, int nTimeOut);
extern int SocketWaitForRead(SOCKET sSocket, int nTimeOut);
extern bool SocketClose(SOCKET sSocket);
extern int SocketRandPort(void);
extern bool SocketSetBroadcast(SOCKET sSocket);
extern bool SocketGetMacByIP(char *szIP, char *szMac);
extern char * SocketGetIPByName(char *szAddr);
extern char *GetLocalIp();

#endif

