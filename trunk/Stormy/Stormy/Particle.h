#ifndef Particle_h__
#define Particle_h__

#include <QPointF>


class Particle
{
public:
	Particle(void);
	~Particle(void);

	qreal mass;	//in kg
	qreal radius;//in m
	QPointF pos;//in m
	QPointF speed;// in m/s
};
#endif // Particle_h__
