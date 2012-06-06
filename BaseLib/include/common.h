/**
 * @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common.h $
 *
 * @brief common head file,常用宏定义及函数头文件
 *
 * $Id: common.h 16 2008-11-18 11:36:40Z gang.chen $
 *
 * @author gang chen <eyesfour@gmail.com>
 */

#ifndef __CG_COMMON_H__
#define __CG_COMMON_H__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#define true 1		/**< C语言中无true关键词，为c/c++兼容用*/
#define false 0		/**< C语言中无false关键词，为c/c++兼容用*/
#define inline		/**<防止编译器无inline关键词时编译失败*/
#endif

#ifdef WIN32
#pragma   warning(disable:4996)
#define   WIN32_LEAN_AND_MEAN   
#include <windows.h>
#include <Winsock2.h>
#include <shlwapi.h>
//#include <ws2tcpip.h>
#include <objbase.h>
#ifndef __cplusplus
typedef unsigned char		bool;
#endif
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;
typedef float				FP32, *PFP32;
typedef double				FP64, *PFP64;
typedef struct { unsigned char octet[16]; }  UINT128;

#pragma   comment   (   lib,   "Shlwapi.lib"   ) 
#ifdef __DEBUG__
	static void DEBUGMSGOUT(const char* pszFormat, ...)
	{
		char buf[40960]={0};
		va_list arglist;
		va_start(arglist, pszFormat);
		vsprintf(&buf[strlen(buf)], pszFormat, arglist);
		va_end(arglist);
		OutputDebugStringA(buf);
		printf("%s", buf);
	}
	#define DEBUGMSG DEBUGMSGOUT("[%s %d %s] ", __FILE__, __LINE__, __FUNCTION__); DEBUGMSGOUT
#else
	#define DEBUGMSG
#endif
#endif

#ifdef _LINUX
#include <unistd.h>
#include <sys/time.h>
#include <uuid/uuid.h>
#ifndef __cplusplus
typedef unsigned char		bool;
#endif
#define TRUE 1
#define FALSE 0 
typedef unsigned char boolean;
typedef signed char         INT8, *PINT8, CHAR;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed long long    INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8, UCHAR;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32, DWORD;
typedef unsigned long long  UINT64, *PUINT64;
typedef float				FP32, *PFP32;
typedef double				FP64, *PFP64;
typedef struct { unsigned char octet[16]; }  UINT128;

#ifdef __DEBUG__
	#define DEBUGMSG(x...) do{printf("[%s %d %s] ", __FILE__, __LINE__, __FUNCTION__);printf(x); }while(0)
#else
	#define DEBUGMSG(x...) 
#endif

//handle type
typedef UINT32 HWND;
typedef void* HANDLE;
#define __stdcall
#endif


/**
* @brief 初始化随机函数种子
*/
extern void SRand(void);

/**
* @brief 返回随机数
*
* @return 随机数，（0～65535）
*/
extern int Rand(void);

/**
* @brief 休眠指定毫秒数
*
* @param mSec 需要休眠的毫秒数
*/
extern void SleepMs(int mSec);

/**
* @brief 取当前时间
* @param tp 存放当前时间的结构体，用于返回结果
* @return 执行结果
* @retval 0 成功执行
* @retval -1 执行失败
*/
extern int GetTimeofDay(struct timeval* tp);

/**
* @brief 获取应用程序当前文件夹
*
* @return 文件夹字符串，最后带"\"或"/"
*/
extern char *GetCurrPath(void);

/**
* @brief 获取系统临时文件夹
*
* @return 临时文件夹字符串，最后带"\"或"/"
*/
extern char *GetTmpPath(void);

/**
* @brief 取GUID，linux下依赖于uuid库
*
* @return 字符串方式的guid
*/
extern const char* newGUID();

/**
* @brief 判断文件夹是否存在
* @param szPathName 文件夹名
* @return 文件夹是否存在
*/
extern bool DirExists(char *szPathName);

/**
* @brief 创建文件夹
* @param szPathName 文件夹名
* @return 是否创建成功
*/
extern bool CreateDir(char *szPathName);

#endif
