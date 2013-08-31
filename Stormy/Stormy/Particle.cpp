#include "Particle.h"
#include <QRectF>

Particle::Particle(void)
: m_mass(0.0), m_radius(0.0), m_posTime(0.0), dbg_level(0)/*, passive(false)*/
{
}

Particle::~Particle(void)
{
}

bool Particle::operator==(const Particle& other) const
{
	return m_pos == other.m_pos;
}

Particle Particle::moved(QVector2D acceleration, qreal time_s) const
{
	Particle result(*this);
	return result.move(acceleration, time_s);
}

Particle& Particle::move(QVector2D acceleration, qreal time_s)
{
	QVector2D dv = acceleration * time_s;
	QVector2D dp = (m_speed + dv/2) * time_s;

	m_speed += dv;
	m_pos += dp;

	m_posTime += time_s;

	return *this;
}

QRectF Particle::boundingRect(QVector2D acceleration, qreal timeLeft) const
{
	Particle p(*this);
	QRectF boundingRect;

	//expand radius a little bit so that bounding rect is a bit bigger and no border errors are present
	p.setRadius(p.radius() * 1.01);//1%

	//x
	if(qFuzzyIsNull(acceleration.x()))
	{
		qreal dx = p.speed().x()*timeLeft;
		if(dx > 0.0)
		{
			boundingRect.setLeft(p.pos().x()-p.radius());
			boundingRect.setRight(p.pos().x()+dx+p.radius());
		}
		else
		{
			boundingRect.setLeft(p.pos().x()+dx-p.radius());
			boundingRect.setRight(p.pos().x()+p.radius());
		}
	}
	else
	{
		qreal critTimeX = -p.speed().x()/acceleration.x();//point where speed changes sign
		if(critTimeX < 0.0 || critTimeX > timeLeft)
		{
			qreal dx = (p.speed().x() + acceleration.x()*timeLeft/2.0)*timeLeft;
			if(dx > 0.0)
			{
				boundingRect.setLeft(p.pos().x()-p.radius());
				boundingRect.setRight(p.pos().x()+dx+p.radius());
			}
			else
			{
				boundingRect.setLeft(p.pos().x()+dx-p.radius());
				boundingRect.setRight(p.pos().x()+p.radius());
			}
		}
		else
		{
			//qreal dxCrit = (p.speed().x() + acceleration.x()*critTimeX/2.0)*critTimeX;
			qreal dxCrit = p.speed().x()/2.0 * critTimeX;
			if(dxCrit > 0.0)
			{
				boundingRect.setLeft(p.pos().x()-p.radius());
				boundingRect.setRight(p.pos().x()+dxCrit+p.radius());
			}
			else
			{
				boundingRect.setLeft(p.pos().x()+dxCrit-p.radius());
				boundingRect.setRight(p.pos().x()+p.radius());
			}
		}
	}

	//y
	if(qFuzzyIsNull(acceleration.y()))
	{
		qreal dy = p.speed().y()*timeLeft;
		if(dy > 0.0)
		{
			boundingRect.setTop(p.pos().y()-p.radius());
			boundingRect.setBottom(p.pos().y()+dy+p.radius());
		}
		else
		{
			boundingRect.setTop(p.pos().y()+dy-p.radius());
			boundingRect.setBottom(p.pos().y()+p.radius());
		}
	}
	else
	{
		qreal critTimeY = -p.speed().y()/acceleration.y();//point where speed changes sign
		if(critTimeY < 0.0 || critTimeY > timeLeft)
		{
			qreal dy = (p.speed().y() + acceleration.y()*timeLeft/2.0)*timeLeft;
			if(dy > 0.0)
			{
				boundingRect.setTop(p.pos().y()-p.radius());
				boundingRect.setBottom(p.pos().y()+dy+p.radius());
			}
			else
			{
				boundingRect.setTop(p.pos().y()+dy-p.radius());
				boundingRect.setBottom(p.pos().y()+p.radius());
			}
		}
		else
		{
			//qreal dyCrit = (p.speed().y() + acceleration.y()*critTimeY/2)*critTimeY;
			qreal dyCrit = p.speed().y()/2.0 * critTimeY;
			qreal dyFinal = (p.speed().y() + acceleration.y()*timeLeft/2.0)*timeLeft;
			if(dyCrit > 0.0)
			{
				boundingRect.setBottom(p.pos().y()+dyCrit+p.radius());
				if(dyFinal > 0.0) dyFinal = 0.0;
				boundingRect.setTop(p.pos().y()+dyFinal-p.radius());
			}
			else
			{
				boundingRect.setTop(p.pos().y()+dyCrit-p.radius());
				if(dyFinal < 0.0) dyFinal = 0.0;
				boundingRect.setBottom(p.pos().y()+dyFinal+p.radius());
			}
		}
	}

	return boundingRect;
}

void Particle::setMass(qreal mass)
{
	m_mass = mass;
}

void Particle::setRadius(qreal radius)
{
	m_radius = radius;
}

void Particle::setPos(QVector2D pos)
{
	m_pos = pos;
}

void Particle::setSpeed(QVector2D speed)
{
	m_speed = speed;
}

void Particle::setPosTime(qreal time)
{
	m_posTime = time;
}
