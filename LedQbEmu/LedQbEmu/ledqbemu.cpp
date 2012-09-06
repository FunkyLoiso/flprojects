#include "ledqbemu.h"

#include "QBWidget.h"

LedQbEmu::LedQbEmu(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	QGLFormat glFormat(QGL::SampleBuffers);
	//glFormat.setSwapInterval(1); // vsync
	glFormat.setSamples(2);		// multisampling

	m_qbWidget = new QBWidget(glFormat, this);
	int qbSize = 8;
	m_qbWidget->setSize(qbSize, qbSize, qbSize);
	setCentralWidget(m_qbWidget);

	m_emu.setDisplay(m_qbWidget);

	startTimer(500);
}

LedQbEmu::~LedQbEmu()
{

}

void LedQbEmu::timerEvent(QTimerEvent *)
{
	//setWindowTitle(QString("fps: %1").arg(m_qbWidget->fps(), 0, 'f', 2));

	static int x = 0, y = 0, z = 0;

	if(++x > 7) x = 0;
	if(++y > 7) y = 0;
	if(++z > 7) z = 0;

	m_emu.setLatch(false);
	m_emu.writeByte(1 << z);
	for(int i = 0; i < y; ++i)
	{
		m_emu.writeByte(0);
	}
	m_emu.writeByte(1 << x);
	m_emu.setLatch(true);

	//m_emu.setLatch(false);
	////m_emu.setLatch(false);
	//m_emu.writeByte(1);
	//m_emu.writeByte(1);
	//m_emu.setLatch(true);
}
