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
	Particle* otherParticle;//NULL if type is not WithParticle
	qreal contactTime_s;	//time after last stable position when collision took place
	QVector2D directionVector;//direction vector for contact line

};

#endif // Collision_h__
