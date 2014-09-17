#ifndef __BULLETENGINETEST_H__
#define __BULLETENGINETEST_H__

#include <vector>

#include "BulletEngine.h"


class BulletEngineTest : public BulletEngine
{
public:
	BulletEngineTest();
	virtual ~BulletEngineTest();

	virtual void initPhysics();
	virtual void exitPhysics();

	virtual void displayCallback();
}; // end BulletEngineTest

#endif //__BULLETENGINETEST_H__
