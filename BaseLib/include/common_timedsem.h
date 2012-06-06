/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/common_timedsem.h $
 *
 *  @brief ��ʱ�ź���, ����ͨ���ź����������������ʱ�ȴ����ܡ�
 *		   ��ͨ�ź�����waitʱ�ǳ�ʱ��wait����ʱ�ź��������趨
 *		   �ȴ�ʱ�䣬��ʱʱ��ʹû���źŷ�����Ҳ�������������
 *		   ����ִ�С�
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
 *@brief ����һ����ʱ�ź���
 *
 *@param ���ص��ź������
 *@param nInitVal �ź�����ʼֵ
 */
extern void TimedSemCreate(TimedSemHandle *pHandle, int nInitVal);

/**
 *@brief ����һ����ʱ�ź���
 *
 *@param hSem �ź������
 */
extern void TimedSemDestroy(TimedSemHandle hSem);

/**
 *@brief �����źţ��ź���ֵ��1
 *
 *@param hSem �ź������
 */
extern void TimedSemPost(TimedSemHandle hSem);

/**
 *@brief �ȴ��źţ��ź���ֵ��1
 *
 *@param hSem �ź������
 *@param nTimeOut ��ʱʱ�䣬����(ms)Ϊ��λ
 *@return �Ƿ�ȴ����ź���
 *@retval true ���ź�������
 *@retval false ��ʱ�˳�
 */
extern bool TimedSemWait(TimedSemHandle hSem, int nTimeOut);

/*#ifdef __cplusplus
}
#endif
*/
#endif

