/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_thread.h $
* @brief �߳�ͷ�ļ������ݺ궨��Ĳ�ͬ��������ͬƽ̨��ʵ��ͷ�ļ�
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
 * @brief ����һ�����߳�
 *
 * @param �̵߳Ĺ�������
 * @param �������߳�ʱ������Զ��������ͨ���ǵ������ʵ��ָ��
 * @return �������߳̾��
 */
extern ThreadHandle ThreadCreate(THREAD_FUN (* pThreadFun)( void * ),  void * pParam);
 
/**
 * @brief �˳��̣߳����̵߳Ĺ�������δβ������
 * 
 */
extern void ThreadExit();

 
/**
 * @brief �ر��̣߳�ͨ�����߳��˳������
 * @param hThread �߳̾��
 */
extern void CloseThreadHandle(ThreadHandle hThread);

/**
 * @brief �ȴ��߳��˳�
 * @param hThread �߳̾��
 */
extern void WaitForThread(ThreadHandle hThread);



#endif

