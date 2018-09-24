#include <cassert>

#include "Steering.h"
#include "ArriveFaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>
#include "FaceSteering.h"
using namespace std;
ArriveFaceSteering::ArriveFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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

Steering* ArriveFaceSteering::getSteering()
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
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::SEEK)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	float targetSpeed;
	float maxSpeed = pOwner->getMaxSpeed();
	if (diff.getLength() < 0)
	{
		return 0;
	}
	if (diff.getLength() > 140)
	{
		targetSpeed = maxSpeed;
	}
	else
	{
		targetSpeed = maxSpeed * diff.getLength() / 140;
	}

	diff.normalize();
	diff *= targetSpeed;

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	FaceSteering faceSteer(mOwnerID, mTargetLoc, mTargetID);
	data = faceSteer.getSteering()->getData();

	data.acc = diff - data.vel;
	data.acc /= .1;   //time to target

	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}



	this->mData = data;
	return this;
}

