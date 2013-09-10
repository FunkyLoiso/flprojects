#ifndef Collision_h__
#define Collision_h__

#include <QVector2D>
#include "Particle.h"
#include "Glass.h"

class Collision
{
public:
	enum
	{
		WithEdge,
		WithVertex,
		WithParticle
	}type;

	//Particle* particle;
	Glass::TParticlesMMap::Iterator particle;
	qreal contactTime_s;	//time after last stable position when collision took place

	//Particle* otherParticle;//NULL if type is not WithParticle
	Glass::TParticlesMMap::Iterator otherParticle;
	QVector2D contactVal;//direction vector for edge or contact vertex

};

#endif // Collision_h__
