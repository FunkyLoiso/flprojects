#ifndef Glass_h__
#define Glass_h__

#include <QPolygonF>
#include <QVector>
#include "Particle.h"

class Glass
{
public:
	Glass(void);
	~Glass(void);

	typedef QVector<Particle> TParticlesVector;
	QPolygonF border;	//in m
	TParticlesVector particles;//map coordX to particle
	double totalEnegry;
};
#endif // Glass_h__
