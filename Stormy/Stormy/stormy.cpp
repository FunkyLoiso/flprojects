#include "stormy.h"

#include <QLabel>

template<typename TFrom, typename TTo>
inline TTo map(TFrom val, TFrom from1, TFrom from2, TTo to1, TTo to2)
{
	TTo result  = TTo(val - from1) * (to2 - to1) / (from2 - from1) + to1;
	return result;
};

qreal randBetween(qreal lower, qreal upper)
{
	return map(qrand(), 0, RAND_MAX, lower, upper);
}

Stormy::Stormy(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_fpsLabel = new QLabel;
	statusBar()->addPermanentWidget(m_fpsLabel);

	connect(ui.bButton1, SIGNAL(clicked()), this, SLOT(onButton1()));

	m_glass.border << QPointF(0.0f, 0.0f) << QPointF(0.4f, 0.0f) << QPointF(0.4f, 0.3f) << QPointF(0.2f, 0.3f) << QPointF(0.2f, 0.3f) << QPointF(0.0f, 0.3f);
	//m_glass.border << QPointF(0.0f, 0.0f) << QPointF(0.4f, 0.0f) << QPointF(0.2f, 0.4f);
	for(int i = 0; i < 50; ++i)
	{
		Particle p;
		p.sn = i;
		p.setRadius(randBetween(0.001, 0.005));
		//p.radius = 0.01;
		p.setMass(c_pi*p.radius()*p.radius() * p.radius() * 7800);

		p.setPos(QVector2D(randBetween(p.radius()+0.001, 0.4 - p.radius() - 0.001), randBetween(p.radius()+0.001, 0.1 - p.radius() - 0.001)));

		static const double maxSpeed = 0.1;
		p.setSpeed(QVector2D(randBetween(-maxSpeed, maxSpeed), randBetween(-maxSpeed, maxSpeed)));

		m_glass.particles.insert(p.pos().x(), p);
	}

	//Particle p;
	//p.pos.setX(0.1);
	//p.pos.setY(0.05);
	//p.radius = 0.01;
	//p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	//p.speed.setX(1000);
	//p.speed.setY(900.980);
	//m_glass.particles.insert(p.pos.x(), p);

	//p.pos.setX(0.12201);
	//p.pos.setY(0.05);
	//p.radius = 0.01;
	//p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	//p.speed.setX(0.001);
	//p.speed.setY(0.0);
	//m_glass.particles.insert(p.pos.x(), p);

	//p.pos.setX(0.15);
	//p.pos.setY(0.05);
	//p.radius = 0.01;
	//p.mass = c_pi*p.radius*p.radius * p.radius * 7800;
	//p.speed.setX(-0.001);
	//p.speed.setY(0.0);
	//m_glass.particles.insert(p.pos.x(), p);

	ui.glassWidget->setGlass(&m_glass);

	//connect(ui.sbRestitution, SIGNAL(valueChanged(double)), &m_engine, SLOT(setRestitution(double)));
	//m_engine.setRestitution(ui.sbRestitution->value());
	//m_engine.setRestitution(0.01);
	//connect(ui.sbFriction, SIGNAL(valueChanged(double)), &m_engine, SLOT(setFriction(double)));
	//m_engine.setFriction(ui.sbFriction->value());
	connect(ui.sbGravityX, SIGNAL(valueChanged(double)), &m_engine, SLOT(setGravityX(double)));
	m_engine.setGravityX(0.0/*ui.sbGravityX->value()*/);
	connect(ui.sbGravityY, SIGNAL(valueChanged(double)), &m_engine, SLOT(setGravityY(double)));
	m_engine.setGravityY(0.0/*ui.sbGravityY->value()*/);

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
	Particle p;
	p.setPos(QVector2D(0.2 ,0.05));
	p.setRadius(0.01);
	p.setMass(c_pi*p.radius()*p.radius() * p.radius() * 7800);
	p.setSpeed(QVector2D(0.001, 0.0));
	m_glass.particles.insert(p.pos().x(), p);
}
