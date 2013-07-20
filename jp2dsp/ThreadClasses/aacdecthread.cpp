#include "aacdecthread.h"

#include "../commonclasses/qmultimapts.h"

void AACDecThread::run()
{
//	decoder.OpenStream();
	while(!terminateFlag)
	{
		while(inQueue.empty() && !terminateFlag) msleep(10);
		if(terminateFlag) break;
		CodedBuff buff = Dequeue();//������� inQueue.dequeue();, ���� �������� �� �����
		msvFrameHeader *frameHeader = (msvFrameHeader *)buff.data1;
		outMapFree->acquire();//������� ������ ������
		//�������� ����� ������ � map. ����� ����� ������� ����� ����, ��� ���������� ����
		//QMap<int, DecodedBuff>::iterator i = outMap->insertMulti(frameHeader->time_ms, DecodedBuff(0, 0, stream));
		//MY_DEBUG printf("\ndecThread %d: decoding frame for time %d", stream, frameHeader->time_ms);
		//msleep(50);
		short *dstSamples = (short *)malloc(2*48000*sizeof(short));//TODO: �� �������� ������ ���
		unsigned int dstSize = decoder.DecodeSamples(buff.data1+sizeof(msvFrameHeader), frameHeader->frameSize, dstSamples);
		outMap->insertMulti(frameHeader->time_ms, DecodedBuff(dstSamples, dstSize, stream));
		//i.value().frame = decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize);
		//MY_DEBUG printf("\ndecThread %d: decoded frame for time %d", stream, frameHeader->time_ms);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		if(outMap->count() >= minOutMapSize)
		{
			outMapUsed->release();
//			MY_DEBUG printf("\ndecThread %d: outMapUsed has %d", stream, outMapUsed->available());
		}
		emit BufferReleased(buff.buffId);
		//outMap->insertMulti(frameHeader->time_ms, RenderedBuff(decoder.DecodeFrame(buff+sizeof(msvFrameHeader), frameHeader->frameSize), stream));
		//MY_DEBUG printf("\ndecThread %d: frame for time %d added", stream, frameHeader->time_ms);
		//�������, ��� ��������� ����� ����������� ������ ������ ���� ����� ������ � ������� ������ ��������
	}
//	decoder.CloseStream();
}
