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

bool Particle::operator<(const Particle& other) const
{
	return (m_pos.x() < other.m_pos.x()) ^ (m_pos.y() < other.m_pos.y());
}

Particle Particle::moved(QVector2D acceleration, qreal time_s) const
{
	Particle result(*this);
	return result.move(acceleration, time_s);
}

Particle& Particle::move(QVector2D acceleration, qreal time_s)
{
	QVector2D dv(acceleration * time_s);
	QVector2D dp((m_speed + dv/2) * time_s);

	m_speed += dv;
	m_pos += dp;

	m_posTime += time_s;

	return *this;
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

inline void expand(qreal& left, qreal& right, qreal val)
{
	if(val > 0.0)	right += val;
	else			left += val;
}

void getAxisBounds(qreal p, qreal v, qreal a, qreal t, qreal& out_left, qreal& out_right)
{
	out_left = out_right = p;
	if(qFuzzyIsNull(a))
	{//no acceleration, particle just travels v*t
		expand(out_left, out_right, v*t);
	}
	else
	{//we have acceleration, particle travels over a parabola
		qreal ct = -v/a;		//time where speed changes sign: {v = v0 + a*ct; v = 0;} => ct = -v0/a
		if(ct < 0.0 || ct > t)	//critical point is outside the frame
		{//particle travels v*t + (a*t^2)/2
			expand(out_left, out_right, (v + a*t/2.0)*t);
		}
		else	//critical point (the vertex of the parabola) is inside the frame
		{//particle travels v0*ct + (a*ct^2)/2 = v0/2 * ct forward, and than backward. Final position is at v0*t + (a*t^2)/2
			qreal dpCrit = v/2.0 * ct;
			qreal dpFinal = (v + a*t/2.0)*t;

			expand(out_left, out_right, dpCrit);
			if(dpCrit > 0.0 != dpFinal > 0.0) expand(out_left, out_right, dpFinal);
		}
	}
}

QRectF Particle::boundingRect(QVector2D acceleration, qreal timeLeft) const
{
	const qreal margin = m_radius*1.001;// radius + 0.1%
	qreal l, r, t, b;
	getAxisBounds(m_pos.x(), m_speed.x(), acceleration.x(), timeLeft, l, r);
	getAxisBounds(m_pos.y(), m_speed.y(), acceleration.y(), timeLeft, t, b);

	QRectF boundingRect;
	boundingRect.adjust(l-margin, t-margin, r+margin, b+margin);
	return boundingRect;
}
