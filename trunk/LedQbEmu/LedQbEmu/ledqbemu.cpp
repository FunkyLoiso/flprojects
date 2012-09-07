#include "ledqbemu.h"

#include "QBWidget.h"
#include "Player.h"

LedQbEmu::LedQbEmu(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	QGLFormat glFormat(QGL::SampleBuffers);
	//glFormat.setSwapInterval(1); // vsync
	glFormat.setSamples(2);		// multisampling

	int qbSize = 8;
	m_qbWidget = new QBWidget(qbSize, qbSize, qbSize, glFormat, this);
	
	setCentralWidget(m_qbWidget);

	m_emu.setDisplay(m_qbWidget);

	m_player = new Player(&m_emu, this);

	startTimer(500);
}

LedQbEmu::~LedQbEmu()
{

}

void LedQbEmu::timerEvent(QTimerEvent *)
{
	setWindowTitle(QString("fps: %1").arg(m_qbWidget->fps(), 0, 'f', 2));
}
