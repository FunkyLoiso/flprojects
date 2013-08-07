#ifndef PhysicsEngine_h__
#define PhysicsEngine_h__

#include <QtGlobal>
#include <QObject>

class Glass;

static const double c_gravityOfEarth = 9.81f;
static const double c_pi = 3.1415;

template<typename T>
static int sign(T val)
{
	return val < 0 ? -1 : 1;
}

class PhysicsEngine : public QObject
{
	Q_OBJECT
public:
	PhysicsEngine() : m_gravityX(0.0f), m_gravityY(0.0f), m_friction(0.0f), m_restitution(0.0f) {}
	virtual ~PhysicsEngine() {}

	virtual void update(Glass* glass, qreal timePassed_s) = 0;

public slots:
	void setGravityX(double gravityX) {m_gravityX = gravityX;}
	void setGravityY(double gravityY) {m_gravityY = gravityY;}
	void setFriction(double friction) {m_friction = friction;}
	void setRestitution(double restitution) {m_restitution = restitution;}

protected:
	qreal m_gravityX, m_gravityY;
	qreal m_friction;
	qreal m_restitution;
};

#endif // PhysicsEngine_h__
