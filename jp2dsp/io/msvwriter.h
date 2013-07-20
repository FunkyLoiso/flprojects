#ifndef MSVWRITER_H
#define MSVWRITER_H

#include <QFile>

#include "../commonclasses/common.h"

class msvWriter
{
public:
	bool OpenFile(char *szFileName);//false, если не удалось
	/* false при попытке дописать кадр со временем, меньшим чем у предидущего */
	bool WriteFrame(unsigned char *frame, unsigned int frameSize, unsigned int frameTime_ms, StreamType stream, unsigned int keyFrame);
	void CloseFile();//«аписывает всю дополнительную информацию
	~msvWriter();
	
	msvFileHeader FileHeader;//Ётот заголовок будет записан в голову файла при вызове CloseFile();
private:
	QFile file;
//	unsigned int lastFrameTime_ms;
	
	static const unsigned int bufferSize = 1048576 * 1;
	char buffer[bufferSize];
	char *buffPtr;
	
	void flushBuffer();
};

#endif /*MSVWRITER_H*/
