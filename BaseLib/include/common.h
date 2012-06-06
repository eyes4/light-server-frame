/**
 * @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common.h $
 *
 * @brief common head file,���ú궨�弰����ͷ�ļ�
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
#define true 1		/**< C��������true�ؼ��ʣ�Ϊc/c++������*/
#define false 0		/**< C��������false�ؼ��ʣ�Ϊc/c++������*/
#define inline		/**<��ֹ��������inline�ؼ���ʱ����ʧ��*/
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
* @brief ��ʼ�������������
*/
extern void SRand(void);

/**
* @brief ���������
*
* @return ���������0��65535��
*/
extern int Rand(void);

/**
* @brief ����ָ��������
*
* @param mSec ��Ҫ���ߵĺ�����
*/
extern void SleepMs(int mSec);

/**
* @brief ȡ��ǰʱ��
* @param tp ��ŵ�ǰʱ��Ľṹ�壬���ڷ��ؽ��
* @return ִ�н��
* @retval 0 �ɹ�ִ��
* @retval -1 ִ��ʧ��
*/
extern int GetTimeofDay(struct timeval* tp);

/**
* @brief ��ȡӦ�ó���ǰ�ļ���
*
* @return �ļ����ַ���������"\"��"/"
*/
extern char *GetCurrPath(void);

/**
* @brief ��ȡϵͳ��ʱ�ļ���
*
* @return ��ʱ�ļ����ַ���������"\"��"/"
*/
extern char *GetTmpPath(void);

/**
* @brief ȡGUID��linux��������uuid��
*
* @return �ַ�����ʽ��guid
*/
extern const char* newGUID();

/**
* @brief �ж��ļ����Ƿ����
* @param szPathName �ļ�����
* @return �ļ����Ƿ����
*/
extern bool DirExists(char *szPathName);

/**
* @brief �����ļ���
* @param szPathName �ļ�����
* @return �Ƿ񴴽��ɹ�
*/
extern bool CreateDir(char *szPathName);

#endif
