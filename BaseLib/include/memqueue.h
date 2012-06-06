/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/memqueue.h $
 *
 *  @brief mem queue头文件，队列使用一段固定内存，内存在队列创建时指定长度先申请好，
 *	在新队列项加入时，不需要再申请新的内存。可作为内存池使用
 *
 *  $Id: memqueue.h 8 2008-06-19 08:58:51Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __MEM_QUEUE_H__
#define __MEM_QUEUE_H__

#include "common.h"

/**
* @brief 单个队列项结构体
*/
typedef struct _TQueueItem
{
	char *pDbStart;								/**<队列项中payload开始位置指针*/
	unsigned int nTotalDbSize;					/**<当前队列项长度*/
	struct _TQueueItem *pPrev;		/**<上一个队列项指针*/
	struct _TQueueItem *pNext;		/**<下一个队列项指针*/
}TQueueItem;

/**
* @brief 内存队列结构体
*/
typedef struct _TQueue
{
	char *pHead;							/**<队列头*/
	char *pTail;							/**<队列尾*/
	TQueueItem *pReadPtr;			/**<读指针*/
	TQueueItem *pWritePtr;		/**<写指针*/
	unsigned int nCount;						/**<总占用个数*/
	unsigned int nSize;							/**<总字节长度*/
	unsigned int nUsedSize;					/**<已使用字节长度*/
}TQueue;


/**
* @brief 创建队列
* @param nSize 队列总占用内存字节长度
* @return 返回队列指针
*/
extern TQueue *QueueCreate(unsigned int nSize);

/**
* @brief 销毁队列，释放资源
* @param q 所要销毁的队列指针
*/
extern void QueueRelease(TQueue *q);

/**
* @brief 向队列中添加数据
* @param q 队列指针
* @param pData 所要添加的数据头指针
* @param nSize 所要添加的数据总长度
* @return 返回是否添加成功
* @retval true 添加成功
* @retval false 添加失败，队列中空闲内存不足
*/
extern bool QueuePutData(TQueue *q, char *pData, unsigned int nSize);

/**
* @brief 从队列中取出一项
* @param q 队列指针
* @return 返回一个队列中的数据项
*/
extern TQueueItem *QueueGetData(TQueue *q);

/**
* @brief 从队列中弹出一项，弹出的数据项不再保存在队列中
* @param q 队列指针
* @return 返回是否成功
* @retval true 成功
* @retval false 失败
*/
extern bool QueuePopData(TQueue *q);


#endif
