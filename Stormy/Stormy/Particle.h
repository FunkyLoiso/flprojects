#ifndef Particle_h__
#define Particle_h__

#include <QVector2D>

class Particle
{
public:
	Particle(void);
	~Particle(void);

	qreal mass;
	qreal radius;
	QVector2D pos;
	QVector2D speed;
};
#endif // Particle_h__
