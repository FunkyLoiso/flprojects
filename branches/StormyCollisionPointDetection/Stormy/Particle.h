#ifndef Particle_h__
#define Particle_h__

#include <QVector2D>


class Particle
{
public:
	Particle(void);
	~Particle(void);

	qreal mass;	//in kg
	qreal radius;//in m
	QVector2D pos;//in m
	QVector2D tmpPos;//general field
	QVector2D speed;// in m/s
	QVector2D tmpSpeed;

	bool mark;
};
#endif // Particle_h__
