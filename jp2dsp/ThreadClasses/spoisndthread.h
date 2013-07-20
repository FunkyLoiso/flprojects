#ifndef SPOISNDTHREAD_H
#define SPOISNDTHREAD_H

#include <QSemaphore>

#include "../commonclasses/common.h"
#include "../commonclasses/qmultimapts.h"

class SPOISndThread : public DecThread
{
public:
	SPOISndThread(QSemaphore *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, StreamType stream):DecThread(outMapFree, outMapUsed, outMap, stream){};
	void OpenStream(){};
	void CloseStream(){};
private:
	void run()
	{
		while(!terminateFlag)
		{
			while(inQueue.empty() && !terminateFlag)
			{
				waitingForData = true;
				msleep(100);
			}
			if(terminateFlag) break;
			waitingForData = false;
			if(!outMapFree->tryAcquire(1, 100)) continue;//Получим пустую ячейку
			CodedBuff buff = Dequeue();
			//printf("\nSPOISndThread: got frame for time %d", buff.time_ms);

			outMap->insertMulti(buff.time_ms, DecodedBuff(buff.data1, buff.size1, stream));
			if(outMap->count() >= minOutMapSize)
			{
				outMapUsed->release();
			}
			emit BufferReleased(buff.buffId);
		}
	};
};

#endif /*SPOISNDTHREAD_H*/
