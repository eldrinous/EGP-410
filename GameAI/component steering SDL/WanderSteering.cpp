#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>

using namespace std;
WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;

	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	mTargetLoc.setX(rand() % 1000);
	mTargetLoc.setY(rand() % 750);
}

Steering* WanderSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	bool closerSenpai = false;
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
	}

	if (mType == Steering::SEEK)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	float dir = atan2(diff.getY(), diff.getX()) + atan(1) * 4 / 2;
	pOwner->getPositionComponent()->setFacing(dir);

	diff.normalize();

	//create circle and offset it and set a point
	int radius = 50;
	int rot = rand() % 360;
	int offset = 100;
	Vector2D targetPoint;
	targetPoint.setX(cos(rot *3.14 / 180) * radius);
	targetPoint.setY(sin(rot *3.14 / 180) * radius);

	mTargetLoc = pOwner->getPositionComponent()->getPosition() +  targetPoint + (diff * offset);

	if (mTargetLoc.getY() > 750)
	{
		mTargetLoc.setY(750);
	}

	if (mTargetLoc.getX() > 1000)
	{
		mTargetLoc.setX(1000);
	}
	

	diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	
	data.acc = diff;
	data.rotVel = 1.0f;
	this->mData = data;
	return this;
}


