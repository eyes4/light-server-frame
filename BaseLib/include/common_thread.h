/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_thread.h $
* @brief 线程头文件，根据宏定义的不同来包含不同平台的实际头文件
*
* $Id: common_thread.h 13 2008-09-30 11:47:16Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __CCOMMON_THREAD_H__
#define __CCOMMON_THREAD_H__

#include "common.h"

#ifdef WIN32  //define the macro for Win32 thread
#include "win32_thread.h"
#endif

#ifdef _LINUX
#include "linux_thread.h"
#endif



/**
 * @brief 创建一个新线程
 *
 * @param 线程的工作函数
 * @param 创建新线程时传入的自定义参数，通常是调用类的实例指针
 * @return 返回新线程句柄
 */
extern ThreadHandle ThreadCreate(THREAD_FUN (* pThreadFun)( void * ),  void * pParam);
 
/**
 * @brief 退出线程，在线程的工作函数未尾处调用
 * 
 */
extern void ThreadExit();

 
/**
 * @brief 关闭线程，通常在线程退出后调用
 * @param hThread 线程句柄
 */
extern void CloseThreadHandle(ThreadHandle hThread);

/**
 * @brief 等待线程退出
 * @param hThread 线程句柄
 */
extern void WaitForThread(ThreadHandle hThread);



#endif

