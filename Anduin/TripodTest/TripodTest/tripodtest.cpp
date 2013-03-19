#include "tripodtest.h"

#include <QMessageBox>

TripodTest::TripodTest(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, m_c0(0), m_c3(0), m_c5(0)
{
	ui.setupUi(this);

	//действие перед раскрытием меню портов
	connect(ui.menuConnect, SIGNAL(aboutToShow()), this, SLOT(portMenuAboutToOpen()));
	//действие при выборе порта
	connect(ui.menuConnect, SIGNAL(triggered(QAction *)), this, SLOT(onPortSelected(QAction *)));
	//новые значения
	connect(&m_readerThread, SIGNAL(valuesCahnged(short, short, short)), this, SLOT(onValuesChanged(short, short, short)));

	connect(ui.zeroCalibrate, SIGNAL(clicked()), this, SLOT(onZeroCalibrate()));
}

TripodTest::~TripodTest()
{

}

void TripodTest::portMenuAboutToOpen()
{
	QMenu *menu = ui.menuConnect;

	menu->clear();
	QStringList ports = m_port.port_list();
	for each(const QString& portName in ports)
	{
		QAction *action = new QAction(portName, NULL);
		action->setCheckable(true);
		if(portName == m_port.get_name()) action->setChecked(true);
		menu->addAction(action);
	}
}

void TripodTest::onPortSelected(QAction *newPort)
{
	if(newPort->text() == m_port.get_name()) return;//если выбран текущий порт

	m_port.Close();
	for each(QAction *action in ui.menuConnect->actions())
	{//снимем галки с прошлого порта
		action->setChecked(false);
	}

	int ret = m_port.Open(newPort->text());
	if( !ret )
	{
		newPort->setChecked(true);//установим галку на новый порт
		onPortChaned();
	}
	else QMessageBox::warning(this, "Error!", QString("Cannot open port %1, code %2").arg(newPort->text()).arg(ret));
}

void TripodTest::onPortChaned()
{
	ui.statusBar->showMessage("Using port " + m_port.get_name());

	m_readerThread.Start(&m_port);
}

void TripodTest::onValuesChanged(short v0, short v3, short v5)
{
	m_v0 = v0, m_v3 = v3, m_v5 = v5;
	v0 += m_c0;
	v3 += m_c3;
	v5 += m_c5;
	ui.bar0->setValue(v0);
	ui.sb0->setValue(v0);
	ui.bar3->setValue(v3);
	ui.sb3->setValue(v3);
	ui.bar5->setValue(v5);
	ui.sb5->setValue(v5);
}

void TripodTest::onZeroCalibrate()
{
	m_c0 = 100 - m_v0;
	m_c3 = 100 - m_v3;
	m_c5 = 100 - m_v5;
}
