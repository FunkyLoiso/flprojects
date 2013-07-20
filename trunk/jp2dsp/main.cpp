#define NORMAL

#ifdef NORMAL
#include <QtGui>
#include <QApplication>

#include "mainwnd.h"
extern "C"
{
#include "frogavcodec/include/ffmpeg/avcodec.h"
}

#include "converters/spoi_to_avi.h"
#include "io/avireader.h"

//#include <QLibrary>
//
//#include "264codec/x264encoder.h"

int main(int argc, char *argv[])
{
	//FILE *out = freopen("log_out.txt", "w+", stdout);
	//FILE *err = freopen("log_err.txt", "w+", stderr);

	avcodec_init();
	avcodec_register_all();

	//SPOI_to_AVI *conv = new SPOI_to_AVI();
	//conv->OpenFolder("E:\\SPOI_video\\2008-08-27-15-21-22-484");
	////conv->OpenFolder("E:\\SPOI_video\\3");
	//conv->SetDestenation("D:\\AVI\\temp\\");
	//conv->Convert();
	//
	//getchar();

	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

//	fclose(out), fclose(err);

	MainWnd w;
	w.show();
	w.thread()->setPriority(QThread::TimeCriticalPriority);
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();

}

#else

extern "C"
{
#include "frogavcodec/include/ffmpeg/avcodec.h"
#include "frogavcodec/include/ffmpeg/avformat.h"
}

#include "264codec/x264encoder.h"
#include "aaccodec/aacencoder.h"
#include "io/msvwriter.h"

int main(int argc, char *argv[])
{
//	freopen("log_out.txt", "w+", stdout);
//	freopen("log_err.txt", "w+", stderr);

	AVFormatContext *pFormatCtx;
	int i, videoStream, audioStream;
	AVCodecContext *VideoCtx, *AudioCtx;
	AVCodec *VideoCodec, *AudioCodec;
	AVFrame *pFrame;
	AVFrame *pFrameRGB;
	AVPacket packet;
	int frameFinished;
	int numBytes;
	uint8_t *buffer;

	X264Encoder x264encoder;
	AACEncoder aacEncoder;
	msvWriter writer;

	avcodec_init();
	// Register all formats and codecs
	av_register_all();

	// Open video file
	if (av_open_input_file(&pFormatCtx, "E:\\12.avi", NULL, 0, NULL)!=0)
		return -1; // Couldn't open file

	// Retrieve stream information
	if (av_find_stream_info(pFormatCtx)<0)
		return -1; // Couldn't find stream information

	// Dump information about file onto standard error
	dump_format(pFormatCtx, 0, argv[1], 0);

	// Find the first video stream
	videoStream=-1;
	for (i=0; i<pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
		{
			videoStream=i;
		}
		if (pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO)
		{
			audioStream=i;
		}
	}
	if (videoStream==-1)
		return -1; // Didn't find a video stream

	// Get a pointer to the codec context for the video stream
	VideoCtx = pFormatCtx->streams[videoStream]->codec;
	AudioCtx = pFormatCtx->streams[audioStream]->codec;

	// Find the decoder for the video stream
	VideoCodec=avcodec_find_decoder(VideoCtx->codec_id);
	AudioCodec = avcodec_find_decoder(AudioCtx->codec_id);
//	pCodec = avcodec_find_encoder(CODEC_ID_H264);
	if (VideoCodec==NULL || AudioCodec == NULL)
	{
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}
	// Open codec
	if (avcodec_open(VideoCtx, VideoCodec)<0 || avcodec_open(AudioCtx, AudioCodec)<0)
		return -1; // Could not open codec

	// Allocate video frame
	pFrame=avcodec_alloc_frame();

	// Allocate an AVFrame structure
//	pFrameRGB=avcodec_alloc_frame();
//	if (pFrameRGB==NULL)
//		return -1;
//
//	// Determine required buffer size and allocate buffer
//	numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
//			pCodecCtx->height);
//	buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
//
//	// Assign appropriate parts of buffer to image planes in pFrameRGB
//	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
//	// of AVPicture
//	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
//			pCodecCtx->width, pCodecCtx->height);


	// Read frames and save first five frames to disk


	aacEncoder.OpenStream(AudioCtx->channels, AudioCtx->sample_rate);
	x264encoder.OpenStream(VideoCtx->width, VideoCtx->height, 21);

	writer.OpenFile("D:\\test.msv");
	writer.FileHeader.audioProps.channels = AudioCtx->channels;
	writer.FileHeader.audioProps.codecID = AudioCtx->codec_id;
	writer.FileHeader.audioProps.sampleFormat = AudioCtx->sample_fmt;
	writer.FileHeader.audioProps.samplerate = AudioCtx->sample_rate;

	writer.FileHeader.videoProps.codecID = VideoCtx->codec_id;
	writer.FileHeader.videoProps.imageH = VideoCtx->height;
	writer.FileHeader.videoProps.imageW = VideoCtx->width;

	static unsigned char *tmpBuff = (unsigned char *)malloc(1408 * 1050 * 3);

	int videoTime, audioTime;
	while (av_read_frame(pFormatCtx, &packet)>=0)
	{
		// Is this a packet from the video stream?
		if (packet.stream_index==videoStream)
		{
			// Decode video frame
			avcodec_decode_video(VideoCtx, pFrame, &frameFinished, packet.data, packet.size);
			// Did we get a video frame?
			if (frameFinished)
			{
//				unsigned int time = packet.pts * VideoCtx->time_base.num * 1000 / VideoCtx->time_base.den;
				videoTime = pFrame->pts * VideoCtx->time_base.num * 1000 / VideoCtx->time_base.den;
				printf("\n time is %d, %.2f%c done", videoTime, (float)videoTime / 91990, '%');
				unsigned int codedSize;
				int keyFrame = x264encoder.EncodeFrame(pFrame, tmpBuff, codedSize);
				writer.WriteFrame(tmpBuff, codedSize, videoTime, VideoDataChannel_0, keyFrame);
				writer.WriteFrame(tmpBuff, codedSize, videoTime, VideoDataChannel_1, keyFrame);
				writer.WriteFrame(tmpBuff, codedSize, videoTime, VideoDataChannel_2, keyFrame);
				writer.WriteFrame(tmpBuff, codedSize, videoTime, VideoDataChannel_3, keyFrame);
//				if(videoTime > 600000)
//				{
//					av_free_packet(&packet);
//					break;
//				}
			}
		}
		else if(packet.stream_index == audioStream)
		{
			int decodedSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
			avcodec_decode_audio2(AudioCtx, (short *)tmpBuff, &decodedSize, packet.data, packet.size);
//			static unsigned int time = -decodedSize /  16 / AudioCtx->channels / (AudioCtx->sample_rate/1000);
//			time += decodedSize /  16 / AudioCtx->channels / (AudioCtx->sample_rate/1000);
//			unsigned int time = packet.pts * AudioCtx->frame_size * 1000 / AudioCtx->sample_rate;
//			static unsigned int samplesPerMs = AudioCtx->sample_rate / 1000;
			audioTime = packet.pts / 40;
			static unsigned char *aacTmp = (unsigned char *)malloc(5000);
			unsigned int codedSize = aacEncoder.EncodeSamples((short *)tmpBuff, decodedSize / 16 / AudioCtx->channels, aacTmp);
			writer.WriteFrame(aacTmp, codedSize, audioTime, SoundDataID, 0);

		}
//		printf("\nv: %d, a: %d, diff: %d", videoTime, audioTime, videoTime - audioTime);

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}
	free(tmpBuff);
	writer.CloseFile();
	x264encoder.CloseStream();
	aacEncoder.CloseStream();

	// Free the RGB image
	av_free(buffer);
	av_free(pFrameRGB);

	// Free the YUV frame
	av_free(pFrame);

	// Close the codec
	avcodec_close(VideoCtx);
	avcodec_close(AudioCtx);

	// Close the video file
	av_close_input_file(pFormatCtx);


	getchar();
	return 0;
}

#endif
