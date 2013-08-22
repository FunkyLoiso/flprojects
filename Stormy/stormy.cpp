#include "stormy.h"

#include <QLabel>

Stormy::Stormy(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_fpsLabel = new QLabel;
	statusBar()->addPermanentWidget(m_fpsLabel);

	connect(ui.bButton1, SIGNAL(clicked()), this, SLOT(onButton1()));

	m_glass.border << QPointF(0.0f, 0.0f) << QPointF(0.4f, 0.0f) << QPointF(0.4f, 0.3f) << QPointF(0.0f, 0.2f);/* << QPointF(0.15f, 0.2f) << QPointF(0.0f, 0.3f);*/
	//m_glass.border << QPointF(0.0f, 0.0f) << QPointF(0.4f, 0.0f) << QPointF(0.2f, 0.4f);
	//for(int i = 0; i < 1400; ++i)
	//{
	//	Particle p;
	//	p.pos.setX(double(qrand()%4000)/10000);
	//	p.pos.setY(double(qrand()%2000)/10000);
	//	p.radius = double(qrand()%4+2)/10000;
	//	p.mass = c_pi*p.radius*p.radius * p.radius * 7800;

	//	static const int maxSpeed = 100;
	//	p.speed.setX(double(qrand()%50)/10000-0.0025);
	//	p.speed.setY(double(qrand()%50)/10000-0.0025);

	//	m_glass.particles.insert(p.pos.x(), p);
	//}

	Particle p;
	p.pos.setX(0.12);
	p.pos.setY(0.11);
	p.radius = 0.01;
	p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	p.speed.setX(0.0);
	p.speed.setY(0.0);
	m_glass.particles.insert(p.pos.x(), p);

	//p.pos.setX(0.25);
	//p.pos.setY(0.02);
	//p.radius = 0.01;
	//p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	//p.speed.setX(0);
	//p.speed.setY(0);
	//m_glass.particles.insert(p.pos.x(), p);

	ui.glassWidget->setGlass(&m_glass);

	//connect(ui.sbRestitution, SIGNAL(valueChanged(double)), &m_engine, SLOT(setRestitution(double)));
	//m_engine.setRestitution(ui.sbRestitution->value());
	//m_engine.setRestitution(0.01);
	//connect(ui.sbFriction, SIGNAL(valueChanged(double)), &m_engine, SLOT(setFriction(double)));
	//m_engine.setFriction(ui.sbFriction->value());
	connect(ui.sbGravityX, SIGNAL(valueChanged(double)), &m_engine, SLOT(setGravityX(double)));
	m_engine.setGravityX(1.0/*ui.sbGravityX->value()*/);
	connect(ui.sbGravityY, SIGNAL(valueChanged(double)), &m_engine, SLOT(setGravityY(double)));
	m_engine.setGravityY(0.00/*ui.sbGravityY->value()*/);

	m_thread.setEngine(&m_engine);
	m_thread.setGlass(&m_glass);

	connect(&m_thread, SIGNAL(updated()), this, SLOT(glassWasUpdated()));

	//QThread::currentThread()->setPriority(QThread::HighPriority);
	//m_thread.setPriority(QThread::LowPriority);
	m_thread.start();
	m_timer.start();
}

Stormy::~Stormy()
{

}

void Stormy::glassWasUpdated()
{
	ui.glassWidget->update();
	//ui.leTotalEnergy->setText(QString::number(m_glass.totalEnegry, 'f'));

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
	p.speed.setX((qrand()%10-5)/5);
	p.speed.setY((qrand()%10-5)/5);
}
