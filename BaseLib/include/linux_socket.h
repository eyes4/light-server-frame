/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/linux_socket.h $
 *
 *  @brief linux socketÍ·ÎÄ¼þ
 *
 *  $Id: linux_socket.h 13 2008-09-30 11:47:16Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef _LINUX_SOCKET_H_
#define _LINUX_SOCKET_H_

#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>  
#include <sys/ioctl.h>  
#include <sys/socket.h>  
#include <net/if.h>  
#include <arpa/inet.h>  
#include <netdb.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>

typedef int SOCKET;

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define MAXINTERFACES 16
#endif

