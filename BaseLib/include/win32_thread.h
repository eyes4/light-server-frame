/**
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/win32_thread.h $
 *
 *  @brief win32 线程头文件
 *
 *  $Id: win32_thread.h 4 2008-06-17 11:55:23Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 */
#ifndef __WIN32_THREAD_H__
#define __WIN32_THREAD_H__

#define   WIN32_LEAN_AND_MEAN   /**<从Windows头中排除极少使用的资料*/
#include <windows.h>
#include <process.h> 

/*************  mutex (use CRITICAL_SECTION in windows) ***************/
#define	ThreadMutex			CRITICAL_SECTION						/**<线程临界区，方法：ThreadMutex Mutex*/
#define	InitMutex(mutex)  	InitializeCriticalSection(mutex)		/**<初始化临界区，调用方法：InitMutex(&Mutex);*/
#define	DestroyMutex(mutex)	DeleteCriticalSection(mutex)			/**<销毁临界区，调用方法：DestroyMutex(&Mutex);*/
#define	EnterMutex(mutex)	EnterCriticalSection(mutex)				/**<进入临界区，调用方法：EnterMutex(&Mutex);*/
#define	LeaveMutex(mutex)	LeaveCriticalSection(mutex)				/**<离开临界区，调用方法：LeaveMutex(&Mutex);*/
#define	ThreadHandle        HANDLE									/**<线程句柄类型*/
#define	THREADFUNC_TYPE		THREAD_FUN								/**<线程工作函数类型宏定义*/

typedef unsigned int (__stdcall * THREAD_FUN)(void *);				/**<线程工作函数类型*/


#endif 