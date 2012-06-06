/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/util/memqueue.cpp $
 *
 *  @mem queueÏà¹Øº¯Êý
 *
 *  $Id: memqueue.cpp 8 2008-06-19 08:58:51Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#include "../include/memqueue.h"



TQueue *QueueCreate(unsigned int nSize)
{
	TQueue *q;
	if (nSize < sizeof(TQueue)) return NULL;
	q = (TQueue *)malloc(sizeof(TQueue));
	if (NULL == q) return NULL;
	q->pHead = (char *)malloc(nSize);
	if (NULL == q->pHead)
	{
		free(q);
		return NULL;
	}
	q->pTail = q->pHead + nSize;
	q->pReadPtr = q->pWritePtr = (TQueueItem *)q->pHead;
	//q->pWritePtr->pDbStart = q->pReadPtr + sizeof(TQueueItem);
	//q->pReadPtr->pDbStart = q->pWritePtr->pDbStart;
	q->pReadPtr->pPrev = q->pReadPtr->pNext = q->pWritePtr->pPrev = q->pReadPtr->pNext = NULL;
	//q->pReadPtr->bLoop = q->pWritePtr->bLoop = false;
	q->nCount = 0;
	q->nSize = nSize;
	q->nUsedSize = 0;/**/
	//DEBUGMSG("q head:%x q tail:%x\n", q->pHead, q->pTail);
	return q;
}

void QueueRelease(TQueue *q)
{
	if (NULL == q) return;
	if (q->pHead != NULL)
	{
		free((char *)q->pHead);
		q->pHead = NULL;
	}
	free(q);
	q = NULL;
}

bool QueuePutData(TQueue *q, char *pData, unsigned int nSize)
{
	TQueueItem *pTemp;
	//CHAR *pTail;
	//UINT32 nFirstSize;
	unsigned int nTotalSize = sizeof(TQueueItem) + nSize;
	//DEBUGMSG("QueuePutData: queue size:%d, queue usedsize:%d DataTotalSize:%d count:%d\n", q->nSize , q->nUsedSize ,nTotalSize, q->nCount);
	if (q->nSize - q->nUsedSize < nTotalSize + sizeof(TQueueItem)) return false;
	//DEBUGMSG("Position: Read:%x write %x\n", q->pReadPtr, q->pWritePtr);
	if (q->pWritePtr >= q->pReadPtr) 
	{
		//DEBUGMSG("writeprt > readptr\n");
		if (((char *)q->pWritePtr + nTotalSize + sizeof(TQueueItem)) > q->pTail)
		{
			//DEBUGMSG("writeprt + totalsize + TQueueItem > pTail\n");
			if ((char *)q->pWritePtr + nTotalSize > q->pTail)
			{
				//DEBUGMSG("writeprt + totalsize > tail\n");
				//DEBUGMSG("(CHAR *)q->pHead + nTotalSize + sizeof(TQueueItem):%x, pReadptr:%x\n", (CHAR *)q->pHead + nTotalSize + sizeof(TQueueItem), (CHAR *)q->pReadPtr);
				if ((char *)q->pHead + nTotalSize + sizeof(TQueueItem) >= (char *)q->pReadPtr) return false;
				pTemp = q->pWritePtr->pPrev;
				pTemp->pNext = (TQueueItem *)q->pHead;
				if (q->pWritePtr == q->pReadPtr) q->pReadPtr = (TQueueItem *)q->pHead;
				q->pWritePtr = (TQueueItem *)q->pHead;
				q->pWritePtr->pDbStart = (char *)q->pWritePtr + sizeof(TQueueItem);
				memcpy(q->pWritePtr->pDbStart, pData, nSize);
				q->pWritePtr->nTotalDbSize = nSize;
				q->pWritePtr->pNext = (TQueueItem *)(q->pWritePtr->pDbStart + nSize);
				pTemp = q->pWritePtr;
				q->pWritePtr = q->pWritePtr->pNext;
				q->pWritePtr->pPrev = pTemp;
				q->pWritePtr->pNext = NULL;
				
				q->nCount++;
				q->nUsedSize += nTotalSize;
			}
			else
			{
				//DEBUGMSG("writeprt + totalsize <= tail\n");
				//DEBUGMSG("q->pHead + sizeof(TQueueItem):%x readptr:%x\n", q->pHead + sizeof(TQueueItem), (CHAR *)q->pReadPtr);
				if (q->pHead + sizeof(TQueueItem) >= (char *)q->pReadPtr) return false;
				//if (q->pHead + sizeof(TQueueItem) <= (CHAR *)q->pReadPtr) return false;

				q->pWritePtr->pDbStart = (char *)q->pWritePtr + sizeof(TQueueItem);
				memcpy(q->pWritePtr->pDbStart, pData, nSize);
				q->pWritePtr->nTotalDbSize = nSize;
				q->pWritePtr->pNext = (TQueueItem *)(q->pHead);
				pTemp = q->pWritePtr;
				q->pWritePtr = q->pWritePtr->pNext;
				q->pWritePtr->pPrev = pTemp;
				q->pWritePtr->pNext = NULL;
				q->nCount++;
				q->nUsedSize += nTotalSize;
			}
		}
		else
		{
			//DEBUGMSG("writeprt + totalsize + TQueueItem < pTail, writeptr:%x nsize:%d\n", (CHAR *)q->pWritePtr, nSize);
			q->pWritePtr->pDbStart = (char *)q->pWritePtr + sizeof(TQueueItem);
			memcpy(q->pWritePtr->pDbStart, pData, nSize);
			q->pWritePtr->nTotalDbSize = nSize;
			q->pWritePtr->pNext = (TQueueItem *)(q->pWritePtr->pDbStart + nSize);
			pTemp = q->pWritePtr;
			q->pWritePtr = q->pWritePtr->pNext;
			q->pWritePtr->pPrev = pTemp;
			q->pWritePtr->pNext = NULL;
			q->nCount++;
			q->nUsedSize += nTotalSize;
		}
	}
	else 
	{
		//DEBUGMSG("writeprt < readptr\n");
		//DEBUGMSG("(CHAR *)q->pWritePtr + nTotalSize + sizeof(TQueueItem):%x readptr:%x\n", (CHAR *)q->pWritePtr + nTotalSize + sizeof(TQueueItem), (CHAR *)q->pReadPtr);
		if ((char *)q->pWritePtr + nTotalSize + sizeof(TQueueItem) >= (char *)q->pReadPtr) return false;
		q->pWritePtr->pDbStart = (char *)q->pWritePtr + sizeof(TQueueItem);
		memcpy(q->pWritePtr->pDbStart, pData, nSize);
		q->pWritePtr->nTotalDbSize = nSize;
		q->pWritePtr->pNext = (TQueueItem *)(q->pWritePtr->pDbStart + nSize);
		pTemp = q->pWritePtr;
		q->pWritePtr = q->pWritePtr->pNext;
		q->pWritePtr->pPrev = pTemp;
		q->pWritePtr->pNext = NULL;
		q->nCount++;
		q->nUsedSize += nTotalSize;

	}

	//DEBUGMSG("head:%x tail:%x write:%x read:%x size:%x\n", q->pHead, q->pTail, (CHAR*)q->pWritePtr, (CHAR *)q->pReadPtr, nSize); 
	return true;
}

TQueueItem *QueueGetData(TQueue *q)
{
	//DEBUGMSG("Get Data:  count:%d readprt:%x writeptr:%x usersize:%d\n", q->nCount , q->pReadPtr , q->pWritePtr, q->nUsedSize);
	if (q->nCount < 1 || q->pReadPtr == q->pWritePtr) return NULL;
	return q->pReadPtr;
}

bool QueuePopData(TQueue *q)
{
	//DEBUGMSG("q->pReadPtr->pNext :%x\n", q->pReadPtr->pNext );
	if (q->pReadPtr->pNext != NULL)
	{
		
		//DEBUGMSG("--pop data, readprt:%x usersize:%d, totalsize:%d\n", q->pReadPtr, q->nUsedSize ,q->pReadPtr->nTotalDbSize);
		q->nUsedSize -= (q->pReadPtr->nTotalDbSize + sizeof(TQueueItem));
		q->pReadPtr = q->pReadPtr->pNext;
		q->nCount--;
		return true;
	}
	else
		return false;
}
