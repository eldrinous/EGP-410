#include <cassert>

#include "Steering.h"
#include "ArriveFaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>

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
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	float slowRadius = 140;
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
	if (diff.getLength() > slowRadius)
	{
		targetSpeed = maxSpeed;
	}
	else
	{
		targetSpeed = maxSpeed * diff.getLength() / slowRadius;
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

	diff.normalize();
	diff *= targetSpeed;

	data.acc = diff - data.vel;
	data.acc /= .1;   //time to target

	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}

	data.rotVel = 1.0f;
	this->mData = data;
	return this;
}

