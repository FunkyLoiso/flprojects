#ifndef MSV_COMMON_H
#define MSV_COMMON_H

#include <QSemaphore>
#include <QMap>
#include <QQueue>
#include <QMutex>
#include <QThread>

#include "../io/SpoiInterface35.h"

extern "C"
{
#include "../frogavcodec/include/ffmpeg/avcodec.h"
}

class Frame;
class H264DecThread;

#define MY_DEBUG if(0)

#define maxOutMapSizePerStream 10	// аждый кадр 1408x1050 занимает 2 217 600 байт (1408 * 1050 * 12bpp / 8)
#define minOutMapSize 5				//кадр 704x396 занимает 418 176 байт. Ёто в 5,3 раза меньше)
#define outMapFillWait 500			//¬рем€ ожидани€ наполнени€ очереди готовых кадром после старта/перемотки

const unsigned int msvMagic = 0x76736D00;// "\0msv"

static const CodecID audioCodecID = CODEC_ID_AC3;
static const CodecID videoCodecID = CODEC_ID_H264;

//struct __attribute__ ((__packed__)) msvFileHeader
//{
//	unsigned int msvMagic;
//	unsigned int totalPlaytime_ms;
//	struct __attribute__ ((__packed__))
//	{
//		CodecID codecID;
//		unsigned int imageH;
//		unsigned int imageW;
//	}videoProps;
//	struct __attribute__ ((__packed__))
//	{
//		CodecID codecID;
//		SampleFormat sampleFormat;
//		unsigned int samplerate;
//		unsigned int channels;
//	}audioProps;
//};
//
//struct __attribute__ ((__packed__)) msvFrameHeader
//{
//	unsigned int frameSize;//размер файла, не включа€ этот заголовок
//	unsigned int time_ms;
//	StreamType stream;
//	unsigned int keyFrame;//ћожно ли сделать bool? ќдинаков ли его размер в разных системах?
//};
//
//struct __attribute__ ((__packed__)) WHStruct
//{
//	unsigned short width;
//	unsigned short height;
//};
#pragma pack(push, 1)
struct msvFileHeader
{
	unsigned int msvMagic;
	unsigned int totalPlaytime_ms;
	struct
	{
		CodecID codecID;
		unsigned int imageH;
		unsigned int imageW;
	}videoProps;
	struct
	{
		CodecID codecID;
		SampleFormat sampleFormat;
		unsigned int samplerate;
		unsigned int channels;
	}audioProps;
};

struct msvFrameHeader
{
	unsigned int frameSize;//размер файла, не включа€ этот заголовок
	unsigned int time_ms;
	StreamType stream;
	unsigned int keyFrame;//ћожно ли сделать bool? ќдинаков ли его размер в разных системах?
};
#pragma pack(pop)

struct WHStruct
{
	unsigned short width;
	unsigned short height;
};


struct CodedBuff
{
	unsigned char *data1;
	unsigned char *data2;
	unsigned int size1;
	unsigned int size2;
	unsigned int time_ms;
	unsigned int buffId;
	CodedBuff(unsigned char *data1 = 0, unsigned char *data2 = 0, unsigned int size1 = 0, unsigned int size2 = 0, unsigned int time_ms = 0, unsigned int buffId = 0)
	{
		this->data1 = data1;
		this->data2 = data2;
		this->size1 = size1;
		this->size2 = size2;
		this->time_ms = time_ms;
		this->buffId = buffId;
	};
};

struct DecodedBuff
{
	void *frame;
	unsigned int frameSize;
	StreamType stream;
	DecodedBuff(void *frame = 0, unsigned int frameSize = 0, StreamType stream = UndefinedDataID)
	{
		this->frame = frame;
		this->frameSize = frameSize;
		this->stream = stream;
	};
};

class IRenderer{};//ѕустой базовый класс, чтобы приводить к нему и хранить все рендереры в одном списке.. ћожет это как-то по-другому делаетс€?
/* ¬ AVFrame::opaque должна лежать высота кадра просто как (void *)height. Ўирина считаетс€ из linesize[0] */
class IAVFrameRenderer : public IRenderer
{
public:
	virtual void Render(Frame *frame) = 0;
};
/* playbackRate - скорость воспроизведени€. ¬ старшей части слова содержитс€ цела€ часть скорости, в младшей - дробна€.
 * ѕримеры:
 * 	0x00010000 - 1x
 * 	0x00008000 - 0.5x
 * 	0x00014000 - 1.25x*/
class ISoundRenderer : public IRenderer
{
public:
	virtual void Render(char *samples, unsigned int size, unsigned long playbackRate) = 0;
};

template <typename keyT, typename valT>
class QMultiMapTS;

class DecThread : public QThread
{
	Q_OBJECT
public:
	DecThread(QSemaphore *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, StreamType stream)
		:terminateFlag(false), stream(stream), outMapFree(outMapFree), outMapUsed(outMapUsed), outMap(outMap), inQueueSem(maxOutMapSizePerStream), waitingForData(false){};
	virtual void OpenStream() = 0;
	virtual void CloseStream() = 0;
	bool Enqueue(CodedBuff buff);
	CodedBuff Dequeue();
	void CleanQueue();
	bool terminateFlag;
	StreamType stream;

	bool waitingForData;

signals:
	void BufferReleased(unsigned int bufferId);

protected:
	QSemaphore *outMapFree, *outMapUsed;
	QMutex localMutex;
	QMultiMapTS<int, DecodedBuff> *outMap;//врем€, данные
	QMap<unsigned int, CodedBuff> inQueue;//врем€, данные

private:
	QSemaphore inQueueSem;
};

struct DecoderPair
{
	DecThread *decoder;
	IRenderer *renderer;
	DecoderPair(DecThread *decoder = 0, IRenderer *renderer = 0)
	{
		this->decoder = decoder;
		this->renderer = renderer;
	}
};

#endif /*MSV_COMMON_H*/
