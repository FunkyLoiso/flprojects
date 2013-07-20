#include <QWaitCondition>
#include <QFile>

#include "../commonclasses/qmultimapts.h"
#include "JP2DecThread.h"
#include "../commonclasses/frame.h"

#ifdef USE_J2K_Codec
void JP2DecThread::run()
{
	while(!terminateFlag)
	{
		//printf("\nJP2DecThread %d: inQueue has %d", stream, inQueue.count());
		while(inQueue.isEmpty() && !terminateFlag)
		{
			//printf("\nJP2DecThread %d: waiting for data", stream);
			waitingForData = true;
			msleep(100);
		}
		if(terminateFlag) break;
		waitingForData = false;
		//printf("\nJP2DecThread %d: outMapFree is %d, acquiring..", stream, outMapFree->available());
		if(!outMapFree->tryAcquire(1, 100)) continue;//Получим пустую ячейку
		//outMapFree->acquire();
		//printf("\nJP2DecThread %d: inQueue is %d, dequeing..", stream, inQueue.count());
		CodedBuff buff = Dequeue();
//		if(buff.data1 == 0 || buff.data2 == 0) continue;

		void *image = J2K_Open(buff.data1, buff.size1);
		J2K_Info info;
		J2K_GetInfoEx(image, &info);
		unsigned char *outBuff = (unsigned char *)malloc(info.Width * info.Height * 3 * 2);
		J2K_Close(image);
		oddWorker->SetData(&buff, outBuff);
		evenWorker->SetData(&buff, outBuff);
		oddSem.release();
		evenSem.release();
//		printf("\nJP2DecThread %d: before acquire(2)", stream);
		doneSem.acquire(2);	//Здесь будем ждать, пока обе части картинки раскодируются
		delete buff.data1;
		delete buff.data2;
//		printf("\nJP2DecThread %d: after acquire(2)", stream);
//		printf("\n*** ***");
//
//		AVFrame *frame = avcodec_alloc_frame();
//		int si = avpicture_fill((AVPicture *)frame, outBuff, PIX_FMT_RGB24, info.Width, info.Height * 2);
//		WHStruct *wh = (WHStruct *)&(frame->opaque);
//		wh->width = info.Width;
//		wh->height = info.Height * 2;

//		outMap->insertMulti(buff.time_ms, DecodedBuff(frame, 0, stream));
		Frame *f = new Frame(outBuff, PIX_FMT_BGR24, info.Width, info.Height * 2);
		outMap->insertMulti(buff.time_ms, DecodedBuff(f, 0, stream));
		//printf("\nJP2DecThread %d: inserted frame ptr: %d", stream, (void *)f);
		//printf("\ndecThread %d: decoded frame for time %d", stream, buff.time_ms);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		if(outMap->count() >= minOutMapSize)
		{
			outMapUsed->release();
//			MY_DEBUG printf("\ndecThread %d: outMapUsed has %d", stream, outMapUsed->available());
		}
		//emit BufferReleased(buff.buffId);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		//MY_DEBUG printf("\ndecThread %d: frame for time %d added", stream, frameHeader->time_ms);
		//Сообщим, что появилась новая заполненная ячейка только если число кадров в очереди больше минимума
	}
}
#endif
#ifdef USE_LuraWave

struct decompressParam
{
	unsigned int width, height;
	unsigned char *outBuff;
	bool isOdd;
};

void * JP2_Callback_Conv callbackMalloc(long size, JP2_Callback_Param lParam){return malloc(size);};
JP2_Error JP2_Callback_Conv callbackFree(void *ptr, JP2_Callback_Param lParam)
{
	free(ptr);
	return cJP2_Error_OK;
};
unsigned long JP2_Callback_Conv callbackRead(unsigned char *pucData, unsigned long ulPos, unsigned long ulSize, JP2_Callback_Param lParam)
{
	char *buff = (char *)lParam;
	memcpy(pucData, buff + ulPos, ulSize);
	return ulSize;
};

JP2_Error JP2_Callback_Conv callbackWrite(
												unsigned char*		pucData,
												short				sComponent,
												unsigned long		ulRow,
												unsigned long		ulStart,
												unsigned long		ulNum,
												JP2_Callback_Param	lParam)
{
	decompressParam *param = (decompressParam *)lParam;
	//Вывод в RGB24
//	int ml = 1;
//	if(sComponent > 0) ml = 2;
//	unsigned char *beg = param->outBuff + (param->width * 6 * ulRow) + ulStart + sComponent;
//	for(unsigned int i = 0; i < ulNum; ++i)
//	{
//		if(!sComponent)beg[i * 3] = *(pucData+i);
//		else beg[i * 6] = beg[i * 6 + 3] = *(pucData+i);
//	}
//	return cJP2_Error_OK;
	unsigned char *beg;
	switch(sComponent)
	{
	case 0: beg = param->outBuff + (param->width * 2 * ulRow + (param->isOdd ? param->width : 0)) + ulStart; break;
	case 2: beg = param->outBuff + (param->width * 2 * param->height) + (param->width * ulRow + (param->isOdd ? param->width/2 : 0)) + ulStart; break;
	case 1: beg = param->outBuff + (param->width * 2 * param->height) + (param->width * param->height) + (param->width * ulRow + (param->isOdd ? param->width/2 : 0)) + ulStart; break;
	}
	memcpy(beg, pucData, ulNum);
	return cJP2_Error_OK;
};

void JP2DecThread::run()
{
	while(!terminateFlag)
	{
		//printf("\nJP2DecThread %d: outMapFree is %d, acquiring..", stream, outMapFree->available());
		outMapFree->acquire();//Получим пустую ячейку
		//printf("\nJP2DecThread %d: asquired! available: %d", stream, outMapFree->available());
		while(inQueue.isEmpty() && !terminateFlag)
		{
			msleep(100);
		}
		if(terminateFlag) break;
		//printf("\nJP2DecThread %d: inQueue is %d, dequeing..", stream, inQueue.count());
		CodedBuff buff = Dequeue();
		printf("\nJP2DecThread %d: got frame for time %d", stream, buff.time_ms);
//		if(buff.data1 == 0 || buff.data2 == 0) continue;
		JP2_Decomp_Handle handle;
		decompressParam param;
		JP2_Property_Value value;
		JP2_Error error;
		error = JP2_Decompress_Start(&handle, callbackMalloc, 0, callbackFree, 0, callbackRead, (JP2_Callback_Param)buff.data1);
		JP2_Decompress_SetLicense(handle, 4267283395, 762824125);
		error = JP2_Decompress_GetProp(handle, cJP2_Prop_Width, &value, 0, 0);
		param.width = (unsigned int) value;
		error = JP2_Decompress_GetProp(handle, cJP2_Prop_Height, &value, 0, 0);
		param.height = (unsigned int) value;
		error = JP2_Decompress_End(handle);

//		char *outBuff = (char *)malloc(/*info.Width * info.Height * */3 * 2);
		unsigned char *outBuff = (unsigned char *)malloc(param.width * param.height * 3 * 2);

		oddWorker->SetData(&buff, outBuff);
		evenWorker->SetData(&buff, outBuff);
		oddSem.release();
		evenSem.release();
//		printf("\nJP2DecThread %d: before acquire(2)", stream);
		doneSem.acquire(2);	//Здесь будем ждать, пока обе части картинки раскодируются
		delete [] buff.data1;
		delete [] buff.data2;
//		printf("\nJP2DecThread %d: after acquire(2)", stream);
//		printf("\n*** ***");
//
//		AVFrame *frame = avcodec_alloc_frame();
//		int si = avpicture_fill((AVPicture *)frame, outBuff, PIX_FMT_RGB24, param.width, param.height * 2);
//		WHStruct *wh = (WHStruct *)&(frame->opaque);
//		wh->width = info.Width;
//		wh->height = info.Height * 2;

//		outMap->insertMulti(buff.time_ms, DecodedBuff(frame, 0, stream));
//		Frame *f = new Frame(outBuff, PIX_FMT_RGB24, info.Width, info.Height * 2);
//		outMap->insertMulti(buff.time_ms, DecodedBuff(f, 0, stream));
		Frame *f = new Frame(outBuff, PIX_FMT_YUVJ440P , param.width, param.height * 2);
		outMap->insertMulti(buff.time_ms, DecodedBuff(f, 0, stream));
//		printf("\nJP2DecThread %d: inserted frame ptr: %d", stream, (void *)f);
//		printf("\ndecThread %d: decoded frame for time %d", stream, buff.time_ms);
//		outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		if(outMap->count() >= minOutMapSize)
		{
			outMapUsed->release();
//			MY_DEBUG printf("\ndecThread %d: outMapUsed has %d", stream, outMapUsed->available());
		}
		//emit BufferReleased(buff.buffId);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		//MY_DEBUG printf("\ndecThread %d: frame for time %d added", stream, frameHeader->time_ms);
		//Сообщим, что появилась новая заполненная ячейка только если число кадров в очереди больше минимума
	}
}
#endif

void JP2DecThread::JP2Worker::run()
{
	for(;;)
	{
//				printf("\nJP2Worker: before sem->acquire()");
		waitSem->acquire();
		if(terminateFlag) return;
//				printf("\nJP2Worker %d: started", isOdd);
#ifdef USE_J2K_Codec
		int reduceVal = 0, err = 0;
		char options[40]; sprintf_s(options, 40, "bpp=3, res_level=%d, video=%d", reduceVal, 0);
//				char options[40]; sprintf(options, "bpp=4, res_level=%d, video=%d, nomct", reduceVal, 0);
		void *image = J2K_Open(isOdd ? data->data2 : data->data1, isOdd ? data->size2 : data->size1);
		J2K_Info info;
		err = J2K_GetInfoEx(image, &info);
//				unsigned char *buffer = (unsigned char *)malloc(outSize);
		int pitch = info.Width * 3 * 2;
		int outSize = pitch * info.Height;
		unsigned char *writePtr = isOdd ? outBuff + info.Width * 3 : outBuff;
		err = J2K_Decode(image, &writePtr, &outSize, options, &pitch);
//				printf("\nJP2Worker %d: decoded", isOdd);
		J2K_Close(image);
//				printf("\nJP2Worker %d:sem before release is %d", isOdd, doneSem->available());
#endif
#ifdef USE_LuraWave
//		if(isOdd) {doneSem->release(); return;}
		JP2_Decomp_Handle handle;
		decompressParam param;
		JP2_Error error;
		JP2_Property_Value value;
		unsigned char *inBuff = isOdd ? data->data2 : data->data1;
		error = JP2_Decompress_Start(&handle, callbackMalloc, 0, callbackFree, 0, callbackRead, (JP2_Callback_Param)inBuff);
		JP2_Decompress_SetLicense(handle, 4267283395, 762824125);
//		error = JP2_Decompress_SetColorSpec(handle, 1);

//		unsigned long nSpecs;
//		JP2_Color_Spec **colorSpec;
//		JP2_Decompress_GetColorSpec(handle, &nSpecs, colorSpec);
//		JP2_Decompress_GetProp(handle, cJP2_Prop_Extern_Colorspace, &value, -1, -1);
		error = JP2_Decompress_SetProp(handle, cJP2_Prop_Output_Function, (JP2_Property_Value)callbackWrite);
		error = JP2_Decompress_SetProp(handle, cJP2_Prop_Output_Parameter, (JP2_Property_Value)&param);
//		JP2_Decompress_SetProp(handle, cJP2_Prop_Scale_Down, 2);
		error = JP2_Decompress_GetProp(handle, cJP2_Prop_Width, &value, 0, 0);
		param.width = (unsigned int)value;
		error = JP2_Decompress_GetProp(handle, cJP2_Prop_Height, &value, 0, 0);
		param.height = (unsigned int)value;
//		param.outBuff = isOdd ? outBuff : outBuff + param.width + 3;
		param.outBuff = outBuff;
		param.isOdd = isOdd;
		error = JP2_Decompress_Image(handle);
		JP2_Decompress_End(handle);
#endif
		doneSem->release();
	}
}

