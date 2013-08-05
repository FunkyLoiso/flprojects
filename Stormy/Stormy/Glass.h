#ifndef Glass_h__
#define Glass_h__

#include <QPolygonF>
#include <QMultiMap>
#include "Particle.h"

class Glass
{
public:
	Glass(void);
	~Glass(void);

	typedef QMultiMap<qreal, Particle> TParticlesMap;
	QPolygonF border;	//in m
	TParticlesMap particles;//map coordX to particle
};
#endif // Glass_h__
