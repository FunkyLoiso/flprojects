#include "dispatcherthread.h"
#include "../commonclasses/qmultimapts.h"
#include "../commonclasses/frame.h"

//#define UNLIMITED_MODE

void DispatcherThread::run()
{
	minFramesSinceDrop = 0x1;
	framesDropped = 0;
	static int framesSinceDrop = minFramesSinceDrop;
	clock.Set(-outMapFillWait);
	while(!terminateFlag)
	{
		//pauseMutex.lock();
//		printf("\ndsp: after pauseMutex.lock()");
		//if(terminateFlag) break;
		//printf("\noutMapUsed is %d", outMapUsed->available());
		//printf("\noutMap->count() is %d", outMap->count());
begin:
		while(!outMap->count() || paused)
		{
			//pauseMutex.unlock();
			waitingForData = true;
			msleep(100);
			//continue;
		}
		waitingForData = false;
		//printf("\nDisaptcherThread: tryAcquire used");
		if(!outMapUsed->tryAcquire(1, 100)) continue;//Здесь будем ждать, пока в очереди не будет минимального количества кадров
		int frameTime_ms = outMap->first().first;

		int timeToSleep = frameTime_ms - clock.Get_ms();
#ifndef UNLIMITED_MODE
		if(timeToSleep > 1000)
		{//Если время слишком большое, то оставляем кадр в очереди, т.к. скорее всего он там по ошибке. Вернёмся к нему позже..
			msleep(20);
			outMapUsed->release();
			//pauseMutex.unlock();
			continue;
		}
#endif
	
		if((timeToSleep <= -1000 && framesSinceDrop >= minFramesSinceDrop)/* || (timeToSleep > 1000)*/)//Если не успеваем, то не отображаем кадр
		{
			framesSinceDrop = 0;
			QList<DecodedBuff> renderList(outMap->values(frameTime_ms));
			framesDropped += renderList.count();

			DecodedBuff decBuff;
			foreach(decBuff, renderList)
			{
				if(renderers->contains(decBuff.stream))(*outMapFree)[decBuff.stream]->release();

				if(decBuff.stream == SoundDataID) free(decBuff.frame);
				else delete (Frame *)decBuff.frame;
			}
			renderList.clear();
			outMapUsed->tryAcquire(outMap->remove(frameTime_ms) - 1);//Удаляем все кадры с таким временем
//			printf("\ndsp: before pauseMutex.unlock()");
			//pauseMutex.unlock();
			continue;
		}
		QList<DecodedBuff> renderList(outMap->values(frameTime_ms));
		int q = outMap->remove(frameTime_ms) - 1;
		//outMapUsed->acquire(q);//Может здесь проблема...
		//if(!outMapUsed->tryAcquire(q, 100))
		//{
		//	//очередь очистили, пока мы что-то тут получали
		//	continue;
		//}
		while(!outMapUsed->tryAcquire(q, 1000))
		{
			if(paused) goto begin;
		}
		printf("\noutMap now contains: %d ", outMap->count());
		framesSinceDrop += renderList.count();
		printf("dropped: %d ", framesDropped);
#ifndef UNLIMITED_MODE
		if(timeToSleep > 0)
		{
			printf("\nDispatcherThread: sleeping for %d", timeToSleep);
			msleep(timeToSleep);//TODO: Здесь выключается ограничение по скорости
		}
#endif

		DecodedBuff tmpBuff;
		foreach(tmpBuff, renderList)
		{
			printf("\nDispatcherThread: releasing outMapFree for stream %d", tmpBuff.stream);
			(*outMapFree)[tmpBuff.stream]->release();
			if(renderers->contains(tmpBuff.stream))
			{
				if(tmpBuff.stream == SoundDataID)
				{
//					unsigned long PlaybackRate = 0x10000 * frameTime_ms / clock.Get_ms();
					//reinterpret_cast<ISoundRenderer *>(renderers->value(tmpBuff.stream).renderer)->Render((char *)tmpBuff.frame, tmpBuff.frameSize, 0/*PlaybackRate*/);
					ISoundRenderer *rnd = (ISoundRenderer *)renderers->value(tmpBuff.stream).renderer;
					rnd->Render((char *)tmpBuff.frame, tmpBuff.frameSize, 0);
					//Освобождаться буфер будет уже там, когда закончится его воспроизведение
				}
				else
				{
					static_cast<IAVFrameRenderer *>(renderers->value(tmpBuff.stream).renderer)->Render((Frame *)tmpBuff.frame);
				}
			}
			else
			{
				if(tmpBuff.stream == SoundDataID) free(tmpBuff.frame);
				else delete (Frame *)tmpBuff.frame;
			}
		}
		unsigned int timeNow = clock.Get_ms();

		renderingSpeed = (timeNow > 0 ? (float)frameTime_ms/timeNow : 1.0f);
		printf("speed: %.3fX ", renderingSpeed);
		emit SetProgressVal(frameTime_ms);
//		printf("\ndsp: before pauseMutex.unlock()");
		//pauseMutex.unlock();
	}
}

void DispatcherThread::Pause(bool pause)
{
	if(paused == pause) return;
	paused = pause;
	if(paused)
	{
		//pauseMutex.lock();
		clock.Pause();
	}
	else
	{
		//pauseMutex.unlock();
		clock.Go();
	}
}
