/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_timedsem.h $
 *
 *  @brief 延时信号量, 在普通的信号量基础上添加了延时等待功能。
 *		   普通信号量在wait时是长时间wait，延时信号量可以设定
 *		   等待时间，超时时即使没有信号发生，也会打破阻塞继续
 *		   向下执行。
 *
 *  $Id: common_timedsem.h 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/

#ifndef __TIMEDSEM_H__
#define __TIMEDSEM_H__

#include "common.h"

#ifdef WIN32  
#include "win32_timedsem.h"
#endif

#ifdef _LINUX
#include "linux_timedsem.h"
#endif

/*#ifdef __cplusplus
extern "C"
{
#endif
*/

/**
 *@brief 创建一个延时信号量
 *
 *@param 返回的信号量句柄
 *@param nInitVal 信号量初始值
 */
extern void TimedSemCreate(TimedSemHandle *pHandle, int nInitVal);

/**
 *@brief 销毁一个延时信号量
 *
 *@param hSem 信号量句柄
 */
extern void TimedSemDestroy(TimedSemHandle hSem);

/**
 *@brief 发送信号，信号量值加1
 *
 *@param hSem 信号量句柄
 */
extern void TimedSemPost(TimedSemHandle hSem);

/**
 *@brief 等待信号，信号量值减1
 *
 *@param hSem 信号量句柄
 *@param nTimeOut 超时时间，毫秒(ms)为单位
 *@return 是否等待到信号量
 *@retval true 有信号量发生
 *@retval false 超时退出
 */
extern bool TimedSemWait(TimedSemHandle hSem, int nTimeOut);

/*#ifdef __cplusplus
}
#endif
*/
#endif

