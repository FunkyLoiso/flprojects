#include "Particle.h"

Particle::Particle(void)
: passive(false)
{
}

Particle::~Particle(void)
{
}

bool Particle::operator==(const Particle& other) const
{
	return pos == other.pos;
}
