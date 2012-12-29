#include "ledcubedatasource.h"

#include <QTimer>
#include <QTime>

#include <QMatrix4x4>

#include "meshdrawer.h"
#include "cube.h"
#include "sphere.h"

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
    sendTimer->start(10);
	connect(sendTimer, SIGNAL(timeout()), this, SLOT(onSendOther()));

    int ret = m_serial.Open("COM5");
    ret = m_serial.Set_baud("115200");

    m_beg = QTime::currentTime();


}

LedCubeDataSource::~LedCubeDataSource()
{

}

void LedCubeDataSource::calculateFrame(byte frame[64])
{
    static MeshDrawer drawer;

    QMatrix4x4 matrix;
//    matrix.scale(7);

    QMatrix4x4 trans;
    trans.translate(3.5, 3.5, 3.5);

    QMatrix4x4 rot;
    int millis = m_beg.msecsTo(QTime::currentTime());
    rot.rotate((millis/8)%360, 0.0, 0.0, 1.0);

    matrix *= trans;// * rot * trans.inverted();
    matrix *= rot;
//    matrix.scale(0.7, 1, 0.7);
//    matrix *= trans;



    memset(frame, 0, 64*sizeof(byte));

    drawer.setMatrix(matrix);
    drawer.setDest(frame);

//    QVector3D p1(0.0, 0.5, 0.0);
//    QVector3D p2 (1.0, 0.5, 0.0);

//    drawer.drawLine(p1, p2);



//    Sphere sphere(3.5, 6, 8);
//    sphere.Draw(drawer);
    Cube cube1(QVector3D(1, 1, 2), 2.5);
    cube1.Draw(drawer);

    rot = QMatrix4x4();
    rot.rotate(-(millis/8)%360, 0.0, 0.0, 1.0);
    matrix = trans * rot;
    drawer.setMatrix(matrix);

    Cube cube(QVector3D(-1, -1, -2), 2.5);
    cube.Draw(drawer);
}

void LedCubeDataSource::onSendMagic()
{
	static const byte magic = 0x22;
	m_serial.Write(&magic, 1);

//	appendSent("! magic sent");
}

void LedCubeDataSource::onSendFrame()
{
	static byte frame[64];
    static bool firstPart = true;

    if(firstPart)
    {
        calculateFrame(frame);
        m_serial.Write(frame, 32);
    }
    else
    {
        m_serial.Write(frame+32, 32);
//        appendSent("! sent 32+32 bytes)");
    }
    firstPart = !firstPart;
}

void LedCubeDataSource::onSendOther()
{
    static bool begin = true;
    if(begin)
    {
        onSendMagic();
    }
    onSendFrame();
    begin = !begin;

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
