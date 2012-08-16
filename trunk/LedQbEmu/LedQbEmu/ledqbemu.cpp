#include "ledqbemu.h"

#include "QBWidget.h"

LedQbEmu::LedQbEmu(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_qbWidget = new QBWidget(this);
	int qbSize = 12;
	m_qbWidget->setSize(qbSize, qbSize, qbSize);
	m_qbWidget->setState(1, 1, 1, true);
	setCentralWidget(m_qbWidget);
}

LedQbEmu::~LedQbEmu()
{

}
