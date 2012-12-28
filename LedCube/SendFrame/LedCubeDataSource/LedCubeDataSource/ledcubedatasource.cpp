#include "ledcubedatasource.h"

#include <QTimer>
#include <QTime>

LedCubeDataSource::LedCubeDataSource(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	connect(ui.bSendMagic, SIGNAL(clicked()), this, SLOT(onSendMagic()));
	connect(ui.bSendFrame, SIGNAL(clicked()), this, SLOT(onSendFrame()));
	connect(ui.bSendOther, SIGNAL(clicked()), this, SLOT(onSendOther()));

	QTimer* receiveTimer = new QTimer(this);
	receiveTimer->start(100);
	connect(receiveTimer, SIGNAL(timeout()), this, SLOT(onReceive()));

	QTimer* sendTimer = new QTimer(this);
	sendTimer->start(1000);
	connect(sendTimer, SIGNAL(timeout()), this, SLOT(onSendOther()));

	m_serial.Open("COM5");
	m_serial.Set_baud("115200");
}

LedCubeDataSource::~LedCubeDataSource()
{

}

void LedCubeDataSource::onSendMagic()
{
	static const byte magic = 0x22;
	m_serial.Write(&magic, 1);

	appendSent("! magic sent");
}

void LedCubeDataSource::onSendFrame()
{
	static byte frame[64];
	for(int i = 0; i < 64; ++i) frame[i] = rand();

	m_serial.Write(frame, 64);

	appendSent("! sent 64 bytes of rand()");
}

void LedCubeDataSource::onSendOther()
{
	onSendMagic();
	onSendFrame();
}

void LedCubeDataSource::onReceive()
{
	static const int size = 1024;
	static char buf[size];
	memset(buf, 0, 1024);

	int read = m_serial.Read(buf, size);

	QString str("%1: %2");
	str = str.arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(buf);
	if(read) ui.eReceived->appendPlainText(str);
}

void LedCubeDataSource::appendSent(QString str)
{
	ui.eSent->appendPlainText(str);
}
