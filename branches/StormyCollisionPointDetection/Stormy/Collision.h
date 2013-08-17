#ifndef Collision_h__
#define Collision_h__

#include <QVector2D>
#include "Particle.h"

class Collision
{
public:
	enum
	{
		WithEdge,
		WithVertex,
		WithParticle
	}type;

	Particle* particle;
	qreal contactTime_s;	//time after last stable position when collision took place

	Particle* otherParticle;//NULL if type is not WithParticle
	QVector2D contactVal;//direction vector for edge or contact vertex

};

#endif // Collision_h__
