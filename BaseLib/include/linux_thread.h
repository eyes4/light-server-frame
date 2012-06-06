/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/linux_thread.h $
 *
 *  @brieff  linux posix �߳�ͷ�ļ�
 *
 *  $Id: linux_thread.h 4 2008-06-17 11:55:23Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __LINUX_THREAD_H__
#define __LINUX_THREAD_H__

#include <pthread.h>

#define	ThreadMutex							pthread_mutex_t											/**<�߳��ٽ�����������ThreadMutex Mutex*/
#define	InitMutex(mutex)				pthread_mutex_init(mutex, NULL)			/**<��ʼ���ٽ��������÷�����InitMutex(&Mutex);*/
#define	DestroyMutex(mutex)			pthread_mutex_destroy(mutex)				/**<�����ٽ��������÷�����DestroyMutex(&Mutex);*/
#define	EnterMutex(mutex)				pthread_mutex_lock(mutex)						/**<�����ٽ��������÷�����EnterMutex(&Mutex);*/
#define	LeaveMutex(mutex)				pthread_mutex_unlock(mutex)					/**<�뿪�ٽ��������÷�����LeaveMutex(&Mutex);*/
#define	ThreadHandle						pthread_t														/**<�߳̾������*/
#define	THREADFUNC_TYPE					void *															/**<�̹߳����������ͺ궨��*/
typedef void * (* THREAD_FUN)(void *);															/**<�̹߳����������ͺ궨��*/


#endif 
