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
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//are we seeking a location or a unit?
	bool closerSenpai = false;
	bool slowRadius = 140;
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

	float modRadianstoOriente = -90 * 3.14 / 180;
	float rotation = fmod(atan2(diff.getY(), diff.getX()) - pOwner->getFacing() - 3.14, 2 * 3.14) + modRadianstoOriente;
	float rotationSize = abs(rotation);
	float targetRotation;
	if (rotationSize < 0)
	{
		return 0;
	}

	if (rotationSize > slowRadius)
	{
		targetRotation = pOwner->getMaxRotVel();
	}
	else
	{
		targetRotation = pOwner->getMaxRotVel() * rotationSize / slowRadius;
	}

	targetRotation *= rotation / rotationSize;

	data.rotAcc = targetRotation - data.rotVel;

	data.rotAcc /= .1;

	if (abs(data.rotAcc) > data.maxRotAcc)
	{
		data.acc /= abs(data.rotAcc);
		data.acc *= data.maxRotAcc;
	}

	this->mData = data;
	return this;
}
