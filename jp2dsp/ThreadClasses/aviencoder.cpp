#include "aviencoder.h"

AVIEncoder::AVIEncoder(char *fileName, int width, int height, int videoBitrate, int channels, int samplerate, int audioBitrate) : restSize(0), waitingForData(false), mapSem(maxOutMapSizePerStream), close(false)
{
	av_register_all();
	fmt = guess_format("avi", NULL, NULL);

	int res = -18;
	ctxt = av_alloc_format_context();
	ctxt->oformat = fmt;
	sprintf_s(ctxt->filename, sizeof(ctxt->filename), fileName);
	videoStream = av_new_stream(ctxt, 0);
	audioStream = av_new_stream(ctxt, 1);

	AVCodecContext *codecCtxt;
	AVCodec *codec;
	//video stream
	codecCtxt = videoStream->codec;
	codecCtxt->codec_id = videoCodecID;
	codecCtxt->codec_type = CODEC_TYPE_VIDEO;
	codecCtxt->gop_size = 12;
	codecCtxt->bit_rate = videoBitrate;
	codecCtxt->width = width;
	codecCtxt->height = height;
	codecCtxt->pix_fmt = PIX_FMT_YUV420P;
	codecCtxt->time_base.num = fpsNum;
	codecCtxt->time_base.den = fpsDen;

	codec = avcodec_find_encoder(videoCodecID);
	res = avcodec_open(codecCtxt, codec);
	//audio stream
	codecCtxt = audioStream->codec;
	codecCtxt->codec_id = audioCodecID;
	codecCtxt->codec_type = CODEC_TYPE_AUDIO;
	codecCtxt->channels = channels;
	codecCtxt->sample_rate = samplerate;
	codecCtxt->bit_rate = audioBitrate;

	codec = avcodec_find_encoder(audioCodecID);
	res = avcodec_open(codecCtxt, codec);

	res = av_set_parameters(ctxt, NULL);
	fprintf(stderr, "\n");	
	dump_format(ctxt, 0, ctxt->filename, 1);

	res = url_fopen(&ctxt->pb, ctxt->filename, URL_WRONLY);
	res = av_write_header(ctxt);

	temp = (unsigned char *)malloc(1600 * 1200 * 4);
}

bool AVIEncoder::Enqueue(int time_ms, DecodedBuff buff)
{
	mapSem.acquire();
	mapMutex.lock();
	inMap.insert(time_ms, buff);
	mapMutex.unlock();
	return true;
}
void AVIEncoder::Close()
{
	while(!waitingForData) msleep(100);
	close = true;
}
void AVIEncoder::run()
{
	int spf = audioStream->codec->frame_size * audioStream->codec->channels;//samples per frame
	int spms = audioStream->codec->sample_rate / 1000;//samples per milisecond
	short *soundBuff = new short[8000 + spf];
	for(;;)
	{
		while(!inMap.count() && !close)
		{
			waitingForData = true;
			msleep(100);
		}
		if(close) break;
		waitingForData = false;
		mapMutex.lock();
		QMap<int, DecodedBuff>::iterator i = inMap.begin();
		int time_ms = i.key();
		DecodedBuff buff = i.value();
		inMap.erase(i);
		mapSem.release();
		mapMutex.unlock();

		if(buff.stream == SoundDataID)
		{//Если звук
			buff.frameSize = buff.frameSize * 24 / 25;

			memcpy(soundBuff + restSize, buff.frame, buff.frameSize);
			free(buff.frame);
			//for(int i = 0; i < buff.frameSize/2; ++i)buffBeg[i] = i;
			short *ptr = soundBuff;
			for(;ptr - soundBuff <= (int)buff.frameSize/2 + restSize - spf; ptr += spf)
			{
				AVPacket pkt;
				av_init_packet(&pkt);

				pkt.size = avcodec_encode_audio(audioStream->codec, temp, 1600 * 1200 * 4, ptr);
				pkt.data = temp;
				pkt.stream_index = audioStream->index;
				//if (av_interleaved_write_frame(i.value(), &pkt) != 0) {fprintf(stderr, "Error while writing audio frame\n");}
				if (av_write_frame(ctxt, &pkt) != 0) {fprintf(stderr, "Error while writing audio frame\n");}
				av_free_packet(&pkt);
			}
			//Проверим, сколько осталось в буффере и скопируем это в restBuff
			restSize = ((int)buff.frameSize/2 + restSize) - ((int)(ptr - soundBuff));
			memcpy(soundBuff, ptr, restSize);
		}
		else if(buff.stream != UndefinedDataID)
		{//Если видео
			Frame *f = (Frame *)buff.frame;

			SwsContext *swsCtxt = sws_getContext(f->width, f->height, f->pixFmt, f->width, f->height, PIX_FMT_YUV420P, /*SWS_PARAM_DEFAULT | */SWS_FAST_BILINEAR | SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2, NULL, NULL, NULL);
			AVFrame *trueSource = avcodec_alloc_frame();
			unsigned char *trueBuff = (unsigned char *)malloc(avpicture_get_size(PIX_FMT_YUV420P, f->width, f->height));
			avpicture_fill((AVPicture *)trueSource, trueBuff, PIX_FMT_YUV420P, f->width, f->height);
			sws_scale(swsCtxt, f->data, f->linesize, 0, f->height, trueSource->data, trueSource->linesize);
			sws_freeContext(swsCtxt);

			int out_size = avcodec_encode_video(videoStream->codec, temp, 1600 * 1200 * 4, trueSource);

			free(trueBuff);
			av_free(trueSource);
			delete f;

			/* if zero size, it means the image was buffered */
			if (out_size > 0) {
				AVPacket pkt;
				av_init_packet(&pkt);
				pkt.pts = /*time_ms/40;*/av_rescale(time_ms, fpsDen, fpsNum * 1000);
				if(videoStream->codec->coded_frame->key_frame) pkt.flags |= PKT_FLAG_KEY;
				pkt.stream_index= videoStream->index;
				pkt.data= temp;
				pkt.size= out_size;

				/* write the compressed frame in the media file */
				//int ret = av_interleaved_write_frame(formatContexts[buff.stream], &pkt);
				if (av_write_frame(ctxt, &pkt) != 0) {fprintf(stderr, "Error while writing video frame\n");}
				av_free_packet(&pkt);
			}
		}
	}
	delete soundBuff;
	//avcodec_close(videoStream->codec);
	//printf("\nSPOI_to_AVI: videoStream->codec closed");
	avcodec_close(audioStream->codec);
	printf("\nSPOI_to_AVI: audioStream->codec closed");
	av_write_trailer(ctxt);
	printf("\nSPOI_to_AVI: trailer written");
	av_free(videoStream->codec);
	av_free(audioStream->codec);
	printf("\nSPOI_to_AVI: codecs freed");
	av_free(videoStream);
	av_free(audioStream);
	printf("\nSPOI_to_AVI: streams freed");
	url_fclose(ctxt->pb);
	printf("\nSPOI_to_AVI: url fcloed");
	av_free(ctxt);
	printf("\nSPOI_to_AVI: format context freed");
}