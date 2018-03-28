#ifndef PhysicsEngine_h__
#define PhysicsEngine_h__

#include <QtGlobal>
#include <QObject>
#include <QVector2D>

class Glass;

static const double c_gravityOfEarth = 9.81f;
static const double c_pi = 3.1415926535897932384626433832795028841971f;

template<typename T>
static int sign(T val)
{
	return val < 0 ? -1 : 1;
}

class PhysicsEngine : public QObject
{
public:
	PhysicsEngine() /*: m_friction(0.0f), m_restitution(0.0f)*/ {}
	virtual ~PhysicsEngine() {}

	virtual void update(Glass* glass, qreal timePassed_s) = 0;

public slots:
	void setGravityX(double gravityX) {m_gravity.setX(gravityX);}
	void setGravityY(double gravityY) {m_gravity.setY(gravityY);}
	//void setFriction(double friction) {m_friction = friction;}
	//void setRestitution(double restitution) {m_restitution = restitution;}

protected:
	QVector2D m_gravity;
	//qreal m_friction;
	//qreal m_restitution;
};

#endif // PhysicsEngine_h__
