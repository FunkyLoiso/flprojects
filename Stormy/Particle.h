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

	bool passive; //all collisions initiated by this particle already calculated

	int dbg_level;
	int sn;

	bool operator==(const Particle& other) const;
	Particle moved(QVector2D acceleration, qreal time_s) const; //returns this particle after time_s seconds passed
	Particle& move(QVector2D acceleration, qreal time_s); //modifies the particle so that time_s seconds passed
};
#endif // Particle_h__
