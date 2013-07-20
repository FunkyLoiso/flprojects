#include "common.h"
#include "qmultimapts.h"

bool DecThread::Enqueue(CodedBuff buff)
{
	if(!inQueueSem.tryAcquire(1, 10000)) return false;
	printf("\nreader got the sem, now the mutex..");
	localMutex.lock();
	printf("\nreader got the  mutex!");
	//printf("\nDecThread %d: frame for %d enqueued. sem is %d", stream, buff.time_ms, inQueueSem.available());
	inQueue.insert(buff.time_ms, buff);
	localMutex.unlock();
//	printf("\nDecThread: frame for %d enqueued", buff.time_ms);
	return true;
}

CodedBuff DecThread::Dequeue()
{
	localMutex.lock();
	if(!inQueue.empty())
	{
		CodedBuff tmp = inQueue.begin().value();
		inQueue.erase(inQueue.begin());
		inQueueSem.release();
		//printf("\nDecThread %d: frame for %d dequeued, sem is %d", stream, tmp.time_ms, inQueueSem.available());
//		printf("\nDecThread: frame for %d dequeued", tmp.time_ms);
		localMutex.unlock();
		return tmp; 
	}
	localMutex.unlock();
	return CodedBuff();
}

void DecThread::CleanQueue()
{
	localMutex.lock();
	inQueueSem.release(inQueue.count());
	CodedBuff codedBuff;
	foreach(codedBuff, inQueue)
	{
		if(codedBuff.data1) delete codedBuff.data1;
		if(codedBuff.data2) delete codedBuff.data2;
		codedBuff = CodedBuff();
	}
	inQueue.clear();
	//printf("\nDecThread %d: inQueue cleaned, sem is %d", stream, inQueueSem.available());
	localMutex.unlock();
}
