#include "Particle.h"

Particle::Particle(void)
: mass(0.0), radius(0.0), posTime(0.0), dbg_level(0), passive(false)
{
}

Particle::~Particle(void)
{
}

bool Particle::operator==(const Particle& other) const
{
	return pos == other.pos;
}

Particle Particle::moved(QVector2D acceleration, qreal time_s) const
{
	Particle result(*this);
	return result.move(acceleration, time_s);
}

Particle& Particle::move(QVector2D acceleration, qreal time_s)
{
	QVector2D dv = acceleration * time_s;
	QVector2D dp = (speed + dv/2) * time_s;

	speed += dv;
	pos += dp;

	posTime += time_s;

	return *this;
}
