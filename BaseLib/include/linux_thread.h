/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/linux_thread.h $
 *
 *  @brieff  linux posix 线程头文件
 *
 *  $Id: linux_thread.h 4 2008-06-17 11:55:23Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __LINUX_THREAD_H__
#define __LINUX_THREAD_H__

#include <pthread.h>

#define	ThreadMutex							pthread_mutex_t											/**<线程临界区，方法：ThreadMutex Mutex*/
#define	InitMutex(mutex)				pthread_mutex_init(mutex, NULL)			/**<初始化临界区，调用方法：InitMutex(&Mutex);*/
#define	DestroyMutex(mutex)			pthread_mutex_destroy(mutex)				/**<销毁临界区，调用方法：DestroyMutex(&Mutex);*/
#define	EnterMutex(mutex)				pthread_mutex_lock(mutex)						/**<进入临界区，调用方法：EnterMutex(&Mutex);*/
#define	LeaveMutex(mutex)				pthread_mutex_unlock(mutex)					/**<离开临界区，调用方法：LeaveMutex(&Mutex);*/
#define	ThreadHandle						pthread_t														/**<线程句柄类型*/
#define	THREADFUNC_TYPE					void *															/**<线程工作函数类型宏定义*/
typedef void * (* THREAD_FUN)(void *);															/**<线程工作函数类型宏定义*/


#endif 
