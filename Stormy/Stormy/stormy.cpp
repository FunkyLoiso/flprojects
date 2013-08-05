#include "stormy.h"

Stormy::Stormy(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_glass.border << QPointF(0.0f, 0.0f) << QPointF(100.0f, 0.0f) << QPointF(100.0f, 100.0f) << QPointF(0.0f, 100.0f);
	for(int i = 0; i < 5; ++i)
	{
		Particle p;
		p.mass = qrand();
		p.pos.setX(qrand()%100);
		p.pos.setY(qrand()%100);
		p.radius = qrand()%10;

		m_glass.particles.insert(p.pos.x(), p);
	}

	ui.glass->setGlass(&m_glass);
}

Stormy::~Stormy()
{

}
