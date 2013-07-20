#ifndef MSVWRITER_H
#define MSVWRITER_H

#include <QFile>

#include "../commonclasses/common.h"

class msvWriter
{
public:
	bool OpenFile(char *szFileName);//false, ���� �� �������
	/* false ��� ������� �������� ���� �� ��������, ������� ��� � ����������� */
	bool WriteFrame(unsigned char *frame, unsigned int frameSize, unsigned int frameTime_ms, StreamType stream, unsigned int keyFrame);
	void CloseFile();//���������� ��� �������������� ����������
	~msvWriter();
	
	msvFileHeader FileHeader;//���� ��������� ����� ������� � ������ ����� ��� ������ CloseFile();
private:
	QFile file;
//	unsigned int lastFrameTime_ms;
	
	static const unsigned int bufferSize = 1048576 * 1;
	char buffer[bufferSize];
	char *buffPtr;
	
	void flushBuffer();
};

#endif /*MSVWRITER_H*/
