#ifndef PhysicsEngine_h__
#define PhysicsEngine_h__

class Glass;

class PhysicsEngine
{
public:
	PhysicsEngine() : m_gravityX(0.0f), m_gravityY(0.0f), m_friction(0.0f), m_restition(0.0f) {}

	void update(Glass* glass, long timePassed_mcs) = 0;

	void setGravityX(qreal gravityX) {m_gravityX = gravityX;}
	void setGravityY(qreal gravityY) {m_gravityY = gravityY;}
	void setFriction(qreal friction) {m_friction = friction;}
	void setRestition(qreal restition) {m_restition = restrict;}

protected:
	qreal m_gravityX, m_gravityY;
	qreal m_friction;
	qreal m_restition;
};

#endif // PhysicsEngine_h__
