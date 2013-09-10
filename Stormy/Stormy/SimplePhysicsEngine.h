#ifndef SimplePhysicsEngine_h__
#define SimplePhysicsEngine_h__

#include "physicsengine.h"
#include "Collision.h"
#include "Glass.h"

#include <QMultiMap>


class Glass;

class SimplePhysicsEngine : public PhysicsEngine
{
public:
	virtual void update(Glass* glass, qreal timePassed_s);
	int totalCollisions() const {return m_totalCollisions;}

private:
	void doCollisions();//detect and process all collisions for all particles
	bool findFirstCollision(Glass::TParticlesMMap::Iterator pi, Collision& out_collision) const;//returns true if collision was detected
	void processCollision(Collision& c);//moves p to collision point and updates speed direction
	
	Glass* m_glass;
	qreal m_time_s;

	//utility functions
	bool rectIntersectsLineSegment(const QRectF& rect, QVector2D p1, QVector2D p2) const;

	//debug
	int m_totalCollisions;
};
#endif // SimplePhysicsEngine_h__
