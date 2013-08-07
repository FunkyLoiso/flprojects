#include "stormy.h"

#include <QLabel>

Stormy::Stormy(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_fpsLabel = new QLabel;
	statusBar()->addPermanentWidget(m_fpsLabel);

	connect(ui.bButton1, SIGNAL(clicked()), this, SLOT(onButton1()));

	m_glass.border << QPointF(0.0f, 0.0f) << QPointF(100.0f, 0.0f) << QPointF(100.0f, 150.0f)<< QPointF(50.0f, 65.0f) << QPointF(0.0f, 100.0f);
	//for(int i = 0; i < 500; ++i)
	//{
	//	Particle p;
	//	p.mass = qrand();
	//	p.pos.setX(qrand()%100);
	//	p.pos.setY(qrand()%100);
	//	p.radius = qrand()%10;

	//	p.speed.setX(qrand()%10);
	//	p.speed.setY(qrand()%10);

	//	m_glass.particles.insert(p.pos.x(), p);
	//}

	Particle p;
	p.mass = 1;
	p.pos.setX(35);
	p.pos.setY(30);
	p.radius = 10;
	p.speed.setX(0);
	p.speed.setY(300);
	m_glass.particles.insert(p.pos.x(), p);

	ui.glassWidget->setGlass(&m_glass);

	connect(ui.sbRestitution, SIGNAL(valueChanged(double)), &m_engine, SLOT(setRestitution(double)));
	m_engine.setRestitution(0.4*ui.sbRestitution->value());

	m_thread.setEngine(&m_engine);
	m_thread.setGlass(&m_glass);

	connect(&m_thread, SIGNAL(updated()), this, SLOT(glassWasUpdated()));

	m_thread.start();
	m_timer.start();
}

Stormy::~Stormy()
{

}

void Stormy::glassWasUpdated()
{
	ui.glassWidget->update();

	static int count = 0;
	++count;
	quint64 curTime = m_timer.elapsed();
	if(curTime-m_lastTime > 1000.0f)
	{
		m_fpsLabel->setText(QString("fps: %1").arg(count/((curTime-m_lastTime)/1000.0f)));
		m_lastTime = curTime;
		count = 0;
	}
}

void Stormy::onButton1()
{
	Particle& p = *m_glass.particles.begin();
	p.speed.setX(qrand()%100-50);
	p.speed.setY(qrand()%100-50);
}