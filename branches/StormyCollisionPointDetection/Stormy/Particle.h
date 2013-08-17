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
	QVector2D speed;// in m/s
	qreal posTime;// time since the beginning of the frame witch pos is corresponding to

	QVector2D projectedSpeed;
	QVector2D projectedPos;

	bool passive; //all collisions initiated by this particle already calculated

	bool operator==(const Particle& other) const;
};
#endif // Particle_h__
