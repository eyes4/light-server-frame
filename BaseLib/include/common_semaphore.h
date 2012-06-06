/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_semaphore.h $
 *
 *  @brief 信号量相关定义和函数头文件
 *
 *  $Id: common_semaphore.h 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "common.h"

#ifdef WIN32  
#include "win32_semaphore.h"
#endif

#ifdef _LINUX
#include "linux_semaphore.h"
#endif

/**
 *@brief 创建一个新信号量
 *
 *@param nInitVal 信号量初始值
 *@return 返回创建的信号量句柄
 */
extern  int SemCreate(INT32 nInitVal, SemHandle *pHandle);

/**
 *@brief 销毁一个信号量
 *
 *@param hSem 信号量句柄
 */
extern void SemDestroy(SemHandle *pSem);

/**
 *@brief 发送信号，信号量值加1
 *
 *@param hSem 信号量句柄
 */
extern void SemPost(SemHandle *pSem);

/**
 *@brief 等待信号，信号量值减1
 *
 *@param hSem the handle of semaphore
 */
extern void SemWait(SemHandle *pSem);

#endif
