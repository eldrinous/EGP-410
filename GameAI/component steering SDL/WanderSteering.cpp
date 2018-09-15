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

	if (diff.getLength() < 140)
	{
		closerSenpai = true;
	}

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	
	if (closerSenpai)
	{
		mTargetLoc.setX(rand() % 1000);
		mTargetLoc.setY(rand() % 750);
	}
	data.acc = diff;
	data.rotVel = 1.0f;
	this->mData = data;
	return this;
}
