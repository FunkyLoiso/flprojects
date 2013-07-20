#include <QFile>
#include <QTimer>

#include "player.h"
#include "../ThreadClasses/h264decthread.h"
#include "../ThreadClasses/audiodecthread.h"
#include "../ThreadClasses/dispatcherthread.h"
#include "../ThreadClasses/JP2DecThread.h"
#include "../ThreadClasses/spoisndthread.h"
#include "qmultimapts.h"

Player::Player(PlayerMode mode) : dispatcher(&outMapFree, &outMapUsed, &outMap, &renderers), mode(mode)
{
//	connect(this, SIGNAL(SetProgressMax(int)), &reader, SIGNAL(SetProgressMax(int)));
//	connect(this, SIGNAL(SetProgressVal(int)), &reader, SIGNAL(SetProgressVal(int)));
	connect(&dispatcher, SIGNAL(SetProgressVal(int)), this, SIGNAL(SetProgressVal(int)));
	Pause(true);
	dispatcher.start();
//	dispatcher.setPriority(QThread::HighPriority);
	switch(mode)
	{
	case MSV_Mode:
		reader = new msvReader(&renderers);
		//reader = new aviReader(&renderers);
		break;
	case SPOI_Mode:
		reader = new SPOIReader(&renderers);
		break;
	case AVI_Mode:
		reader = new aviReader(&renderers);
		break;
	}
}

bool Player::OpenFile(char *szFileName)
{
	if(reader->OpenFile(szFileName))
	{
		reader->start();
		return true;
	}
	return false;
}

Player::~Player()
{
//	if(file.isOpen())file.close();
}

void Player::TerminateDispatchers()
{
//	Pause(true);
//	DispatcherThread *dThread;
//	foreach(dThread, renderers)
//	{
//		dThread->terminateFlag = true;
//		dThread->wait();
//		delete dThread;
//	};
//	renderers.clear();
}

void Player::TerminateDispatcher(StreamType stream)
{
	if(!renderers.contains(stream)) return;
	renderers.value(stream).decoder->terminateFlag = true;
//	renderers.value(stream).decoder->wait();
	renderers.remove(stream);
//	delete outMapFree[stream];
//	outMapFree[stream] = 0;

	//if(!renderers.value(stream).decoder->wait(200)) renderers.value(stream).decoder->terminate();
	//delete remPtr;

}

void Player::RegisterRenderer(IRenderer *renderer, StreamType stream)
{
	outMapFree[stream] = new QSemaphore(maxOutMapSizePerStream);
	switch(mode)
	{
	case MSV_Mode:
	case AVI_Mode:
		if(stream == SoundDataID) renderers[stream] = DecoderPair(new AudioDecThread(outMapFree[stream], &outMapUsed, &outMap, stream), renderer);
		else renderers[stream] = DecoderPair(new H264DecThread(outMapFree[stream], &outMapUsed, &outMap, stream), renderer);
		break;
	case SPOI_Mode:
		if(stream == SoundDataID) renderers[stream] = DecoderPair(new SPOISndThread(outMapFree[stream], &outMapUsed, &outMap, stream), renderer);
		else renderers[stream] = DecoderPair(new JP2DecThread(outMapFree[stream], &outMapUsed, &outMap, stream), renderer);
		break;
	}
	connect(renderers[stream].decoder, SIGNAL(BufferReleased(unsigned int)), reader, SLOT(BufferReleased(unsigned int)), Qt::DirectConnection);
	renderers[stream].decoder->OpenStream();
	renderers[stream].decoder->start();//Воспроизведение остаётся на паузе, поток спит, пока в его очереди не появятся данные
//	renderers[stream].decoder->setPriority(QThread::LowPriority);
//	if(stream > type_video4)renderers[stream].decoder->setPriority(QThread::HighestPriority);
//	Seek(150);
}

void Player::Seek(int time_ms)
{
	if(!seekMutex.tryLock()) return;
	reader->Pause(true);
	while(!reader->isPaused) Sleep(100);
	printf("\nseek: reader paused");

	DecoderPair pair;
	foreach(pair, renderers)
	{
		DecThread *dec = pair.decoder;
		dec->CleanQueue();
		printf("\nseek: decoder %d cleared", dec->stream);
		while(!dec->waitingForData) Sleep(100);
		printf("\nseek: decoder %d waiting for data", dec->stream);
	}
	dispatcher.Pause(true);
	while(!dispatcher.waitingForData) Sleep(100);
	printf("\nseek: dispatcher waiting for data");
	outMap.freeContent();
	printf("\nseek: outMap contents deleted");
	outMap.clear();
	printf("\nseek: outMap cleared");
	outMapUsed.acquire(outMapUsed.available());
	printf("\nseek: outMapUsed set to %d", outMapUsed.available());
	//for(int i = 1; i < NumDataTypes; ++i)
	//{
	//	outMapFree[i]->release(maxOutMapSizePerStream - outMapFree[i]->available());
	//	printf("\nseek: outMapFree[%d] set to %d", i, outMapFree[i]->available());
	//}
	QSemaphore *sem;
	foreach(sem, outMapFree) sem->release(maxOutMapSizePerStream - sem->available());
	printf("\nseek: \"outMapFree\"s set to %d", maxOutMapSizePerStream);
	reader->Seek(time_ms);
	printf("\nseek: seek done");
	//dispatcher.SetClock(time_ms - 500 >= 0 ? time_ms - 500 : 0);//на 500 меньше, чтобы успела наполниться очередь
	dispatcher.SetClock(time_ms - outMapFillWait);//на 500 меньше, чтобы успела наполниться очередь
	printf("\nseek: dispatcher clock reset");
	reader->Pause(false);
	printf("\nseek: reader unpaused <^.^>");
	dispatcher.Pause(wasPaused);
	printf("\nseek: dispatcher unpaused)");
	seekMutex.unlock();
}

void Player::Pause(bool pause)
{
	wasPaused = pause;
	dispatcher.Pause(pause);


	printf("\nplayer: pause changed to %d\n", pause);
}

void Player::NextFrame()
{
//	Pause(true);
//	DispatcherThread *dsp;
//	foreach(dsp, renderers)
//	{
//		dsp->NextFrame();
//	}
}

//void msvPlayer::FrameSent(unsigned int time_ms)
//{
//	if(time_ms > currentTime)
//	{
//		emit SetProgressVal(time_ms);
//		currentTime = time_ms;
//	}
//	//MY_DEBUG printf("dThread: buff for %d ms dispatchered (EOBuffers.head = %d)\n", time_ms, EOBuffers_ms.head());
//	if(!EOBuffers_ms.isEmpty() && EOBuffers_ms.head() < time_ms)
//	{
//		MY_DEBUG printf("player: buff being updated..\n");
//		EOBuffers_ms.removeFirst();
///*		if(EOBuffers_ms.isEmpty())Pause(true);*/
//		/*else*/ getBuffer();
//	}
//}
