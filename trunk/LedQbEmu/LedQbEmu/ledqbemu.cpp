#include "ledqbemu.h"

#include "QBWidget.h"

LedQbEmu::LedQbEmu(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	QGLFormat glFormat(QGL::SampleBuffers);
	glFormat.setSwapInterval(1); // vsync
	glFormat.setSamples(2);

	m_qbWidget = new QBWidget(glFormat, this);
	int qbSize = 8;
	m_qbWidget->setSize(qbSize, qbSize, qbSize);
	m_qbWidget->setState(1, 1, 1, true);
	setCentralWidget(m_qbWidget);

	startTimer(500);
}

LedQbEmu::~LedQbEmu()
{

}

void LedQbEmu::timerEvent(QTimerEvent *)
{
	setWindowTitle(QString("fps: %1").arg(m_qbWidget->fps(), 0, 'f', 2));
}
