#ifndef CUBE_HALF_EXTENTS
#	define CUBE_HALF_EXTENTS 1
#endif

#ifndef EXTRA_HEIGHT
#	define EXTRA_HEIGHT 1.
#endif


#include <functional>

#include "BulletEngineTest.h"
#include "log.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btMatrixX.h"


BulletEngineTest::BulletEngineTest() :
		BulletEngine()
{
} // end BulletEngineTest::BulletEngineTest

BulletEngineTest::~BulletEngineTest()
{
} // end BulletEngineTest::~BulletEngineTest


void BulletEngineTest::initPhysics()
{
	BulletEngine::initPhysics();

	// ----------------------------------------------------------------------------------------------------------------
	// Create the ground collider.

	LOG("  Creating ground collider");
	btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(110.), btScalar(1.), btScalar(110.)));
	//groundShape->initializePolyhedralFeatures();
	//btStaticPlaneShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 50);

	_collisionShapes.push_back(groundShape);

	btTransform groundTransform(btTransform::getIdentity());
	//groundTransform.setOrigin(btVector3(5, 5, 5));

	btRigidBody* body = localCreateRigidBody(0., groundTransform, groundShape);
	_models.push_back(_makeModelCB());

	body->setFriction(0.5);
	//body->setRollingFriction(0.3);

	// ----------------------------------------------------------------------------------------------------------------
	// Create a few dynamic rigidbodies.
	// Re-using the same collision shape is better for memory usage and performance.

	LOG("  Creating rigid bodies");
	//btCollisionShape* shape = new btSphereShape(btScalar(1.));
	//btCollisionShape* shape = new btCylinderShape(btVector3(CUBE_HALF_EXTENTS, CUBE_HALF_EXTENTS, CUBE_HALF_EXTENTS));
	btCollisionShape* shape = new btBoxShape(btVector3(CUBE_HALF_EXTENTS, CUBE_HALF_EXTENTS, CUBE_HALF_EXTENTS));
	_collisionShapes.push_back(shape);

	int gNumObjects = 120; //120;
	int i;
	for(i = 0; i < gNumObjects; i++)
	{
		btTransform trans;
		trans.setIdentity();

		// stack them
		int colsize = 10;
		int row = (i * CUBE_HALF_EXTENTS * 2) / (colsize * 2 * CUBE_HALF_EXTENTS);
		int row2 = row;
		int col = (i % colsize) - (colsize / 2);

		if(col > 3)
		{
			col = 11;
			row2 |= 1;
		}

		btVector3 pos(
				(col * 2 * CUBE_HALF_EXTENTS) + ((row2 % 2) * CUBE_HALF_EXTENTS),
				(row * 2 * CUBE_HALF_EXTENTS) + CUBE_HALF_EXTENTS + EXTRA_HEIGHT,
				0
				);

		trans.setOrigin(pos);

		btScalar mass(1.);

		btRigidBody* body = localCreateRigidBody(mass, trans, shape);
		_models.push_back(_makeModelCB());

		body->setAnisotropicFriction(shape->getAnisotropicRollingFrictionDirection(),
				btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

		body->setFriction(.5);
		//body->setRollingFriction(.3);

		// When using CCD:
		if(_useCCD)
		{
			body->setCcdMotionThreshold(CUBE_HALF_EXTENTS);
			body->setCcdSweptSphereRadius(0.9 * CUBE_HALF_EXTENTS);
		} // end if
	} // end for

	LOG("  Done");
} // end BulletEngineTest::initPhysics

void BulletEngineTest::exitPhysics()
{
	// Cleanup in the reverse order of creation/initialization.

	// Remove the rigidbodies from the dynamics world and delete them.
	for(int i = dynamicsWorld()->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld()->getCollisionObjectArray()[i];

		btRigidBody* body = btRigidBody::upcast(obj);
		if(body && body->getMotionState())
		{
			delete body->getMotionState();
		} // end if

		dynamicsWorld()->removeCollisionObject(obj);
		delete obj;
	} // end for

	// Delete collision shapes.
	for(int j = 0; j < _collisionShapes.size(); j++)
	{
		btCollisionShape* shape = _collisionShapes[j];
		delete shape;
	} // end for
	_collisionShapes.clear();

	BulletEngine::exitPhysics();
} // end BulletEngineTest::exitPhysics

void BulletEngineTest::displayCallback()
{
	btTransform trans;
	//btMatrix3x3 rot; rot.setIdentity();

	const int numObjects = dynamicsWorld()->getNumCollisionObjects();
	for(int i = 0; i < numObjects; i++)
	{
		btCollisionObject* colObj = dynamicsWorld()->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(colObj);

		if(body && body->getMotionState())
		{
			btDefaultMotionState* myMotionState = (btDefaultMotionState*) body->getMotionState();
			trans = myMotionState->m_graphicsWorldTrans;
			//rot = myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else
		{
			trans = colObj->getWorldTransform();
			//rot = colObj->getWorldTransform().getBasis();
		} // end if

		btMatrix3x3 basis = trans.getBasis();
		btVector3 row0 = basis[0], row1 = basis[1], row2 = basis[2];
		btVector3 origin = trans.getOrigin();

		float mat[16] = {
#if 0
			row0.x(), row1.x(), row2.x(), origin.x(),
			row0.y(), row1.y(), row2.y(), origin.y(),
			row0.z(), row1.z(), row2.z(), origin.z(),
			0., 0., 0., 0.
#else
			row0.x(), row0.y(), row0.z(), 0.,
			row1.x(), row1.y(), row1.z(), 0.,
			row2.x(), row2.y(), row2.z(), 0.,
			origin.x(), origin.y(), origin.z(), 0.
#endif
		};

		_updateModelCB(_models[i], mat);
	} // end for
} // end BulletEngineTest::displayCallback
