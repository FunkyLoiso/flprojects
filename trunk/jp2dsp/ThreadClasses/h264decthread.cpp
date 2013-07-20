#include "h264decthread.h"
#include "../commonclasses/qmultimapts.h"

void H264DecThread::run()
{
//	decoder.OpenStream();
	while(!terminateFlag)
	{
		printf("\nh264DecThread %d: inQueue has %d", stream, inQueue.count());
		while(inQueue.empty() && !terminateFlag)
		{
			waitingForData = true;
			msleep(100);
		}
		if(terminateFlag) break;
		waitingForData = false;
		printf("\nh264DecThread %d: outMapFree is %d, acquiring..", stream, outMapFree->available());
		outMapFree->acquire();
		//if(!outMapFree->tryAcquire(1, 100))
		//{
		//	printf("\nh264DecThread %d: !cannot acquire, continue!", stream);
		//	continue;//ѕолучим пустую €чейку
		//}
		printf("\nh264DecThread %d: acquired! Before Dequeue..", stream);
		CodedBuff buff = Dequeue();
		printf("\nh264DecThread %d: Dequeued! time: %d..", stream, buff.time_ms);
		if(buff.data1 == 0) continue;
		printf("\nh264DecThread %d: data is %x%x%x%x%x%x%x%x size is %d", stream, buff.data1[0], buff.data1[1], buff.data1[2], buff.data1[3], buff.data1[4], buff.data1[5], buff.data1[6], buff.data1[7], buff.size1);
		//msvFrameHeader *frameHeader = (msvFrameHeader *)buff.data1;
		//«аполним новую €чейку в map. јдрес кадра запишем после того, как декодируем кадр
//		QMap<int, RenderedBuff>::iterator i = outMap->insertMulti(frameHeader->time_ms, RenderedBuff(0, 0xfefefefe, stream));
		//MY_DEBUG printf("\ndecThread %d: decoding frame for time %d", stream, frameHeader->time_ms);
		//msleep(50);
//		i.value().frame = decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize);
//		if(frameHeader->time_ms > 600000 || frameHeader->time_ms < 0)
//		{
//			printf("\ndecThread:Strange frame! Time: %d, size: %d, ptr: %d", frameHeader->time_ms, frameHeader->frameSize, buff.data1);
////			emit BufferReleased(buff.buffId);
//			continue;
//		}
//		printf("\ndecThread: decoding frame for time %d, ptr: %d", frameHeader->time_ms, buff.data);
		outMap->insertMulti(buff.time_ms, DecodedBuff(decoder.DecodeFrame(buff.data1, buff.size1), 0, stream));
		printf("\nh264DecThread %d: inserted", stream);

		delete buff.data1;
		//MY_DEBUG printf("\ndecThread %d: decoded frame for time %d", stream, frameHeader->time_ms);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		if(outMap->count() >= minOutMapSize)
		{
			outMapUsed->release();
			printf("\nh264DecThread %d: outMapUsed released", stream);
//			MY_DEBUG printf("\ndecThread %d: outMapUsed has %d", stream, outMapUsed->available());
		}
		//emit BufferReleased(buff.buffId);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		//MY_DEBUG printf("\ndecThread %d: frame for time %d added", stream, frameHeader->time_ms);
		//—ообщим, что по€вилась нова€ заполненна€ €чейка только если число кадров в очереди больше минимума
	}
//	decoder.CloseStream();
}
