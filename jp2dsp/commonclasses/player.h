#ifndef MSVPLAYER_H
#define MSVPLAYER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QTimer>
#include <QQueue>
#include <QSemaphore>

#include <windows.h>

#include "../commonclasses/common.h"
#include "../io/msvreader.h"
#include "../ThreadClasses/dispatcherthread.h"
#include "../commonclasses/qmultimapts.h"
#include "../io/spoireader.h"
#include "../io/avireader.h"

enum PlayerMode {MSV_Mode, SPOI_Mode, AVI_Mode};

class Player : public QObject
{
	Q_OBJECT
public:
	Player(PlayerMode mode);
	bool OpenFile(char *szFileName);//false, ���� ���� ��������� �� ������
	/* ����� ����� �������� ���������� �������, ����� ����� �����
	 * ���������� ���� �� ���������������� � ��� ������.
	 * ������ ��������� ���������� ����� */
	void RegisterRenderer(IRenderer *renderer, StreamType stream);
	void CloseFile();
	unsigned int GetTotalPlaytime_ms(){return reader->GetTotalPlaytime_ms();};
	~Player();
signals:
	void SetProgressVal(int);	//��������� ������� ���������������

public slots:
	/* ����������� ���������� ��������� � ����� � ��������, ��������� � ���������.
	 * ����� �� �������� �� �����*/
	void Seek(int time_ms);
	void Pause(bool pause);//������������� ���������������
	void NextFrame();//������ �� ����� � ��������� ��������� ���� �� ���� �������
	void TerminateDispatchers();//������ ���������� ����������� �� ����, ��� ������� ��� �����, ������ � ��� ��������
	void TerminateDispatcher(StreamType stream);

private:
	QMutex seekMutex;
	QMap<StreamType, DecoderPair> renderers;
	QMultiMapTS<int, DecodedBuff> outMap;
	QSemaphore /**outMapFree[NumDataTypes], */outMapUsed;
	QMap<StreamType, QSemaphore *> outMapFree;
//	msvReader reader;
	IFileReader *reader;
	DispatcherThread dispatcher;

	PlayerMode mode;
	unsigned int currentTime;
	bool wasPaused;
};

#endif /*MSVREADER_H*/
