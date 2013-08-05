#ifndef SimplePhysicsEngine_h__
#define SimplePhysicsEngine_h__

#include "physicsengine.h"

class SimplePhysicsEngine : public PhysicsEngine
{
public:
	virtual void update(Glass* glass, qreal timePassed_mcs);

};
#endif // SimplePhysicsEngine_h__
