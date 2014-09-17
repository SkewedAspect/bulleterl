// Using motionstate is recommended; it provides interpolation capabilities, and only synchronizes 'active' objects.
#ifndef USE_MOTIONSTATE
#	define USE_MOTIONSTATE 1
#endif


#include "BulletEngine.h"
#include "log.h"

#include "btBulletDynamicsCommon.h"


BulletEngine::BulletEngine() :
		_dispatcher(NULL),
		_broadphase(NULL),
		_solver(NULL),
		_collisionConfiguration(NULL),
		_dynamicsWorld(NULL),

		_maxSubSteps(10),
		_useCCD(true),
		_defaultContactProcessingThreshold(0.),
		_gravity(0, -10, 0),

		_makeModelCB(NULL),
		_updateModelCB(NULL),

		_msSinceLastFrame(0)
{
} // end BulletEngine::BulletEngine

BulletEngine::~BulletEngine()
{
	exitPhysics();
} // end BulletEngine::~BulletEngine


btScalar BulletEngine::msSinceLastFrame() const
{
	return _msSinceLastFrame;
} // end BulletEngine::msSinceLastFrame

btScalar BulletEngine::calcMSSinceLastFrame()
{
	btScalar ms = (btScalar) _clock.getTimeMicroseconds() / 1000.;
	_clock.reset();
	return ms;
} // end BulletEngine::calcMSSinceLastFrame


btRigidBody* BulletEngine::localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	// The rigidbody is dynamic if and only if its mass is nonzero; otherwise, it's static.
	bool isDynamic = (mass != 0.);

	btVector3 localInertia(0, 0, 0);
	if(isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	} // end if

#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(_defaultContactProcessingThreshold);

#else
	btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif

	_dynamicsWorld->addRigidBody(body);

	return body;
} // end BulletEngine::localCreateRigidBody


void BulletEngine::initPhysics()
{
	// Override in subclasses, but **call this in your implementation**.

	//collisionConfiguration()->setConvexConvexMultipointIterations();

	//dispatcher()->registerCollisionCreateFunc(BOX_SHAPE_PROXYTYPE, BOX_SHAPE_PROXYTYPE, _collisionConfiguration->getCollisionAlgorithmCreateFunc(CONVEX_SHAPE_PROXYTYPE, CONVEX_SHAPE_PROXYTYPE));

	LOG("  Setting solver mode: |= SOLVER_USE_2_FRICTION_DIRECTIONS | SOLVER_RANDMIZE_ORDER");
	dynamicsWorld()->getSolverInfo().m_solverMode |= SOLVER_USE_2_FRICTION_DIRECTIONS | SOLVER_RANDMIZE_ORDER;

	//dynamicsWorld()->getSolverInfo().m_splitImpulse = false;

	LOG("  Setting CCD mode: %c", _useCCD ? 'T' : 'F');
	dynamicsWorld()->getDispatchInfo().m_useContinuous = _useCCD;

	LOG("  Setting gravity: (0, -10, 0)");
	dynamicsWorld()->setGravity(_gravity);

	// Call calcMSSinceLastFrame() once but don't set _msSinceLastFrame, so we don't get weird clock issues at startup.
	calcMSSinceLastFrame();
} // end BulletEngine::initPhysics

void BulletEngine::exitPhysics()
{
	// Override in subclasses, but **call this in your implementation**.

	delete _dynamicsWorld;

	delete _solver;

	delete _broadphase;

	delete _dispatcher;

	delete _collisionConfiguration;
} // end BulletEngine::exitPhysics


void BulletEngine::step()
{
	// Step the simulation
	if(_dynamicsWorld)
	{
		_msSinceLastFrame = calcMSSinceLastFrame();
		//LOG("_msSinceLastFrame == %f", _msSinceLastFrame);

		_dynamicsWorld->stepSimulation(_msSinceLastFrame / 1000., _maxSubSteps);
	} // end if
} // end BulletEngine::step

void BulletEngine::doFrame()
{
	step();

	displayCallback();
} // end BulletEngine::doFrame


btCollisionDispatcher* BulletEngine::dispatcher()
{
	if(_dispatcher == NULL)
	{
		// Use the default collision dispatcher. For parallel processing you can use a different dispatcher (see
		// Extras/BulletMultiThreaded)
		_dispatcher = new btCollisionDispatcher(collisionConfiguration());
	} // end if

	return _dispatcher;
} // end BulletEngine::dispatcher

btBroadphaseInterface* BulletEngine::broadphase()
{
	if(_broadphase == NULL)
	{
		_broadphase = new btDbvtBroadphase();
	} // end if

	return _broadphase;
} // end BulletEngine::broadphase

btConstraintSolver* BulletEngine::solver()
{
	if(_solver == NULL)
	{
		// The default constraint solver. For parallel processing you can use a different solver (see
		// Extras/BulletMultiThreaded)
		_solver = new btSequentialImpulseConstraintSolver;
	} // end if

	return _solver;
} // end BulletEngine::solver

btDefaultCollisionConfiguration* BulletEngine::collisionConfiguration()
{
	if(_collisionConfiguration == NULL)
	{
		// Collision configuration contains default setup for memory, collision setup
		_collisionConfiguration = new btDefaultCollisionConfiguration();
	} // end if

	return _collisionConfiguration;
} // end BulletEngine::collisionConfiguration

btDiscreteDynamicsWorld* BulletEngine::dynamicsWorld()
{
	if(_dynamicsWorld == NULL)
	{
		_dynamicsWorld = new btDiscreteDynamicsWorld(
				dispatcher(),
				broadphase(),
				solver(),
				collisionConfiguration()
				);
	} // end if

	return _dynamicsWorld;
} // end BulletEngine::dynamicsWorld

void BulletEngine::displayCallback()
{
} // end BulletEngine::displayCallback

void BulletEngine::setMakeModelCallback(MakeModelCallback makeModelCB)
{
	_makeModelCB = makeModelCB;
} // end BulletEngine::setMakeModelCallback

void BulletEngine::setUpdateModelCallback(UpdateModelCallback updateModelCB)
{
	_updateModelCB = updateModelCB;
} // end BulletEngine::setUpdateModelCallback
