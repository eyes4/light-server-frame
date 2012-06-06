/**
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/win32_thread.h $
 *
 *  @brief win32 �߳�ͷ�ļ�
 *
 *  $Id: win32_thread.h 4 2008-06-17 11:55:23Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 */
#ifndef __WIN32_THREAD_H__
#define __WIN32_THREAD_H__

#define   WIN32_LEAN_AND_MEAN   /**<��Windowsͷ���ų�����ʹ�õ�����*/
#include <windows.h>
#include <process.h> 

/*************  mutex (use CRITICAL_SECTION in windows) ***************/
#define	ThreadMutex			CRITICAL_SECTION						/**<�߳��ٽ�����������ThreadMutex Mutex*/
#define	InitMutex(mutex)  	InitializeCriticalSection(mutex)		/**<��ʼ���ٽ��������÷�����InitMutex(&Mutex);*/
#define	DestroyMutex(mutex)	DeleteCriticalSection(mutex)			/**<�����ٽ��������÷�����DestroyMutex(&Mutex);*/
#define	EnterMutex(mutex)	EnterCriticalSection(mutex)				/**<�����ٽ��������÷�����EnterMutex(&Mutex);*/
#define	LeaveMutex(mutex)	LeaveCriticalSection(mutex)				/**<�뿪�ٽ��������÷�����LeaveMutex(&Mutex);*/
#define	ThreadHandle        HANDLE									/**<�߳̾������*/
#define	THREADFUNC_TYPE		THREAD_FUN								/**<�̹߳����������ͺ궨��*/

typedef unsigned int (__stdcall * THREAD_FUN)(void *);				/**<�̹߳�����������*/


#endif 