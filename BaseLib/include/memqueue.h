/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/memqueue.h $
 *
 *  @brief mem queueͷ�ļ�������ʹ��һ�ι̶��ڴ棬�ڴ��ڶ��д���ʱָ������������ã�
 *	���¶��������ʱ������Ҫ�������µ��ڴ档����Ϊ�ڴ��ʹ��
 *
 *  $Id: memqueue.h 8 2008-06-19 08:58:51Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __MEM_QUEUE_H__
#define __MEM_QUEUE_H__

#include "common.h"

/**
* @brief ����������ṹ��
*/
typedef struct _TQueueItem
{
	char *pDbStart;								/**<��������payload��ʼλ��ָ��*/
	unsigned int nTotalDbSize;					/**<��ǰ�������*/
	struct _TQueueItem *pPrev;		/**<��һ��������ָ��*/
	struct _TQueueItem *pNext;		/**<��һ��������ָ��*/
}TQueueItem;

/**
* @brief �ڴ���нṹ��
*/
typedef struct _TQueue
{
	char *pHead;							/**<����ͷ*/
	char *pTail;							/**<����β*/
	TQueueItem *pReadPtr;			/**<��ָ��*/
	TQueueItem *pWritePtr;		/**<дָ��*/
	unsigned int nCount;						/**<��ռ�ø���*/
	unsigned int nSize;							/**<���ֽڳ���*/
	unsigned int nUsedSize;					/**<��ʹ���ֽڳ���*/
}TQueue;


/**
* @brief ��������
* @param nSize ������ռ���ڴ��ֽڳ���
* @return ���ض���ָ��
*/
extern TQueue *QueueCreate(unsigned int nSize);

/**
* @brief ���ٶ��У��ͷ���Դ
* @param q ��Ҫ���ٵĶ���ָ��
*/
extern void QueueRelease(TQueue *q);

/**
* @brief ��������������
* @param q ����ָ��
* @param pData ��Ҫ��ӵ�����ͷָ��
* @param nSize ��Ҫ��ӵ������ܳ���
* @return �����Ƿ���ӳɹ�
* @retval true ��ӳɹ�
* @retval false ���ʧ�ܣ������п����ڴ治��
*/
extern bool QueuePutData(TQueue *q, char *pData, unsigned int nSize);

/**
* @brief �Ӷ�����ȡ��һ��
* @param q ����ָ��
* @return ����һ�������е�������
*/
extern TQueueItem *QueueGetData(TQueue *q);

/**
* @brief �Ӷ����е���һ�������������ٱ����ڶ�����
* @param q ����ָ��
* @return �����Ƿ�ɹ�
* @retval true �ɹ�
* @retval false ʧ��
*/
extern bool QueuePopData(TQueue *q);


#endif
