#ifndef Glass_h__
#define Glass_h__

#include <QPolygonF>
#include <QMultiMap>
#include <QVector>
#include <QMutex>
#include "Particle.h"

class Glass
{
public:
	Glass(void);
	~Glass(void);

	typedef QMultiMap<double, Particle> TParticlesMMap;
	//typedef QVector<Particle> TParticlesMMap;
	QPolygonF border;	//in m
	TParticlesMMap particles;//map coordX to particle
	QMutex particlesMutex;
	double totalEnegry;
};
#endif // Glass_h__
