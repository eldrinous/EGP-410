#include <cassert>

#include "Steering.h"
#include "Facesteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>

using namespace std;
FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	setTargetLoc(targetLoc);
}

Steering* FaceSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::SEEK)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	float dir = atan2(diff.getY(), diff.getX()) + atan(1) * 4 / 2;
	bool goRight;
	float curDir = fmod(pOwner->getFacing(),2*3.14);
	
	if (dir - curDir > 0)
	{
		data.rotAcc = 1;
	}
	else
	{
		data.rotAcc = -1;
	}

	if (abs(curDir - dir) < 5 * 3.14 / 180)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
	}
	this->mData = data;

	return this;
}
