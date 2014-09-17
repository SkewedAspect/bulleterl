#ifndef __BULLETENGINE_H__
#define __BULLETENGINE_H__

#include <vector>

#include "LinearMath/btAlignedObjectArray.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btTransform.h"


class btClock;
class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;
class btRigidBody;

typedef void* Model;
typedef Model (*MakeModelCallback)();
typedef void (*UpdateModelCallback)(Model, float*);


class BulletEngine
{
private:
	btCollisionDispatcher* _dispatcher;
	btBroadphaseInterface* _broadphase;
	btConstraintSolver* _solver;
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btDiscreteDynamicsWorld* _dynamicsWorld;

protected:
	int _maxSubSteps;
	bool _useCCD;
	btScalar _defaultContactProcessingThreshold;
	btVector3 _gravity;

	// Keep the collision shapes, for deletion/cleanup.
	btAlignedObjectArray<btCollisionShape*> _collisionShapes;

	std::vector<Model> _models;

	MakeModelCallback _makeModelCB;
	UpdateModelCallback _updateModelCB;

	btClock _clock;
	btScalar _msSinceLastFrame;

public:
	BulletEngine();
	virtual ~BulletEngine();

	btScalar msSinceLastFrame() const;
	virtual btScalar calcMSSinceLastFrame();

	virtual btRigidBody* localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

	virtual void initPhysics();
	virtual void exitPhysics();

	virtual void step();
	virtual void doFrame();

	virtual btCollisionDispatcher* dispatcher();
	virtual btBroadphaseInterface* broadphase();
	virtual btConstraintSolver* solver();
	virtual btDefaultCollisionConfiguration* collisionConfiguration();
	virtual btDiscreteDynamicsWorld* dynamicsWorld();

	virtual void displayCallback();

protected:
	void setMakeModelCallback(MakeModelCallback makeModelCB);
	void setUpdateModelCallback(UpdateModelCallback updateModelCB);
}; // end BulletEngine

#endif //__BULLETENGINE_H__
