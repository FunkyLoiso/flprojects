#include "ledqbemu.h"

#include "QBWidget.h"

LedQbEmu::LedQbEmu(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_qbWidget = new QBWidget(this);
	setCentralWidget(m_qbWidget);
}

LedQbEmu::~LedQbEmu()
{

}
