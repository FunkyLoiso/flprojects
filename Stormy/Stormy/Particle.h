#ifndef Particle_h__
#define Particle_h__

#include <QVector2D>


class Particle
{
public:
	Particle(void);
	~Particle(void);
	//Particle(const Particle& other);
	//Particle& operator=(const Particle& other);

	//debug
	int dbg_level;
	int sn;

	qreal mass() const {return m_mass;}
	void setMass(qreal mass);
	qreal radius() const {return m_radius;}
	void setRadius(qreal radius);
	QVector2D pos() const {return m_pos;}
	void setPos(QVector2D pos);
	QVector2D speed() const {return m_speed;}
	void setSpeed(QVector2D speed);
	qreal posTime() const {return m_posTime;}
	void setPosTime(qreal time);

	bool operator==(const Particle& other) const;
	bool operator<(const Particle& other) const;
	Particle moved(QVector2D acceleration, qreal time_s) const; //returns this particle after time_s seconds passed
	Particle& move(QVector2D acceleration, qreal time_s); //modifies the particle so that time_s seconds passed

	QRectF boundingRect(QVector2D acceleration, qreal timeLeft) const;

private:
	qreal m_mass;	//in kg
	qreal m_radius;//in m
	
	QVector2D m_pos;//in m
	QVector2D m_speed;// in m/s
	qreal m_posTime;// time since the beginning of the frame witch pos is corresponding to

	//bool passive; //all collisions initiated by this particle already calculated




};
#endif // Particle_h__
