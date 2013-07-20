#include "msvwriter.h"

bool msvWriter::OpenFile(char *szFileName)
{
	CloseFile();
	file.setFileName(szFileName);
	if(!file.open(QFile::WriteOnly)) return false;
	file.seek(sizeof(msvFileHeader));//Оставим место на заголовок
	FileHeader.msvMagic = msvMagic;
	FileHeader.totalPlaytime_ms = 0;
	buffPtr = buffer;
	return true;
}

bool msvWriter::WriteFrame(unsigned char *frame, unsigned int frameSize, unsigned int frameTime_ms, StreamType stream, unsigned int keyFrame)
{
//	printf("\nmsvWriter: writing frame for %d ms, size: %d", frameTime_ms, frameSize);
 	if(!file.isWritable()) return false;
//	if(lastFrameTime_ms > frameTime_ms) return false;
	msvFrameHeader header;
	header.frameSize = frameSize;
	header.stream = stream;
	header.time_ms = frameTime_ms;
	header.keyFrame = keyFrame;

	//если этот кадр уже не влезет в буфер
	if(bufferSize - (buffPtr - buffer) < frameSize + sizeof(msvFrameHeader)) flushBuffer();
	memcpy(buffPtr, &header, sizeof(msvFrameHeader));
	buffPtr += sizeof(msvFrameHeader);
	memcpy(buffPtr, frame, frameSize);
	buffPtr += frameSize;
	FileHeader.totalPlaytime_ms = frameTime_ms;
	return true;
}

void msvWriter::CloseFile()
{
	if(file.isOpen())
	{
		flushBuffer();

		file.seek(0);
		file.write((char *)(&FileHeader), sizeof(msvFileHeader));
		file.close();
	}
}

msvWriter::~msvWriter()
{
	CloseFile();
}

void msvWriter::flushBuffer()
{
	file.write(buffer, buffPtr - buffer);
	buffPtr = buffer;
}
