#ifndef SimplePhysicsEngine_h__
#define SimplePhysicsEngine_h__

#include "physicsengine.h"
#include "Collision.h"

#include <QMultiMap>

class Glass;

class SimplePhysicsEngine : public PhysicsEngine
{
public:
	virtual void update(Glass* glass, qreal timePassed_s);

private:
	void writeProjectedSpeedAndPosition(Particle& p);
	void doCollisions(Particle& p, int level);//recursively detect and process collisions for p and everything it touches
	bool findFirstCollision(Particle& p, Collision& out_collision) const;//returns true if collision was detected

	void processCollision(Collision& c);//update projected speed and position due to collision c
	
	QRectF getBoundingRect(Particle p, QVector2D acceleration, qreal timeLeft) const;
	bool rectIntersectsLine(const QRectF& rect, QVector2D p1, QVector2D p2) const;

	Glass* m_glass;
	qreal m_time_s;

//debug
	QMultiMap<qreal, Collision> colls;
};
#endif // SimplePhysicsEngine_h__
