// SpoiInterface35.h : header file for use the DataSource DLL
//

#pragma once

struct SSummaryInfo {			// ��������� ���������� �� ����� ����������
	unsigned int nFrame;		// ����� ���������� ������ ������
	unsigned int playbackTime;	// ����� ��������������� � �����
};

enum StreamType { // ����������� ����� ������ (�������������� ������) ������ �����
	UndefinedDataID = 0,
	SoundDataID,
	VideoDataChannel_0,
	VideoDataChannel_1,
	VideoDataChannel_2,
	VideoDataChannel_3,
	VideoDataChannel_4,
	VideoDataChannel_5,
	NumDataTypes
};

struct SFrame { // ���������, ����������� ������ ������ �� ������������ ������ �������

	unsigned int time;	// ����� ����� � �����
	StreamType id;		// ������������� ������
	void *frameData;	// ������ �����, ������ �� JPEG2000, ��� �������� ������	
	int size;			// ������ ������ � ������
};

class CSpoiInterface {

public:

	virtual bool openFile (const char *fileName) = 0;
	virtual bool openDevice() = 0;
	virtual void close() = 0;

	// ��������� ��������� outSummaryInfo ��������� ����������� �� ���� ������
	virtual bool getInfo(SSummaryInfo & outSummaryInfo) = 0;

	// ��������� ��������� outFrame ������� �� frameIndex �����
	virtual bool getFrame(unsigned int frameIndex, SFrame &outFrame) = 0;

	// ������� ����� �����, ��������� � ��������� �������
	virtual int findFrame(unsigned int time) = 0;
};


