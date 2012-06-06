/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/linux_timedsem.h $
 *
 *  linux timedsem head file, adds timed waiting feature to the simple semaphore
 *
 *  $Id: linux_timedsem.h 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __CG_LINUX_TIMEDSEM_H__
#define __CG_LINUX_TIMEDSEM_H__

#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>

#include "common.h"
typedef struct __TIMEDSEM
{
    int nCount;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
}
TimedSemHandle;

#endif
