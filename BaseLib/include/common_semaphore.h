/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_semaphore.h $
 *
 *  @brief �ź�����ض���ͺ���ͷ�ļ�
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
 *@brief ����һ�����ź���
 *
 *@param nInitVal �ź�����ʼֵ
 *@return ���ش������ź������
 */
extern  int SemCreate(INT32 nInitVal, SemHandle *pHandle);

/**
 *@brief ����һ���ź���
 *
 *@param hSem �ź������
 */
extern void SemDestroy(SemHandle *pSem);

/**
 *@brief �����źţ��ź���ֵ��1
 *
 *@param hSem �ź������
 */
extern void SemPost(SemHandle *pSem);

/**
 *@brief �ȴ��źţ��ź���ֵ��1
 *
 *@param hSem the handle of semaphore
 */
extern void SemWait(SemHandle *pSem);

#endif
