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

	QPolygonF border;
	QMultiMap<qreal, Particle> particles;//map xoordX to particle
};
#endif // Glass_h__
