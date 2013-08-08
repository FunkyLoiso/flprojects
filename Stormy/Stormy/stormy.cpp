#include "stormy.h"

#include <QLabel>

Stormy::Stormy(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_fpsLabel = new QLabel;
	statusBar()->addPermanentWidget(m_fpsLabel);

	connect(ui.bButton1, SIGNAL(clicked()), this, SLOT(onButton1()));

	m_glass.border << QPointF(0.0f, 0.0f) << QPointF(400.0f, 0.0f) << QPointF(400.0f, 300.0f)<< QPointF(150.0f, 240.0f) << QPointF(0.0f, 300.0f);
	for(int i = 0; i < 400; ++i)
	{
		Particle p;
		p.mass = qrand();
		p.pos.setX(qrand()%100);
		p.pos.setY(qrand()%100);
		p.radius = qrand()%4+0.1;
		p.mass = c_pi*p.radius*p.radius * p.radius * 7800;

		p.speed.setX(qrand()%100);
		p.speed.setY(qrand()%100);

		m_glass.particles.insert(p.pos.x(), p);
	}

	//Particle p;
	//p.pos.setX(35);
	//p.pos.setY(30);
	//p.radius = 8;
	//p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	//p.speed.setX(20);
	//p.speed.setY(0);
	//m_glass.particles.insert(p.pos.x(), p);

	//p.pos.setX(180);
	//p.pos.setY(35);
	//p.radius = 10;
	//p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	//p.speed.setX(-20);
	//p.speed.setY(0);
	//m_glass.particles.insert(p.pos.x(), p);

	ui.glassWidget->setGlass(&m_glass);

	connect(ui.sbRestitution, SIGNAL(valueChanged(double)), &m_engine, SLOT(setRestitution(double)));
	//m_engine.setRestitution(ui.sbRestitution->value());
	m_engine.setRestitution(1.0f);
	connect(ui.sbFriction, SIGNAL(valueChanged(double)), &m_engine, SLOT(setFriction(double)));
	m_engine.setFriction(ui.sbFriction->value());

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
	ui.leTotalEnergy->setText(QString::number(m_glass.totalEnegry, 'f'));

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
	p.speed.setX(qrand()%1000-500);
	p.speed.setY(qrand()%1000-500);
}
