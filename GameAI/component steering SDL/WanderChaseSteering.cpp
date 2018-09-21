#include <cassert>

#include "Steering.h"
#include "WanderChaseSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>

using namespace std;
WanderChaseSteering::WanderChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
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
	playerPos = targetLoc;
	if (!targetPlayer)
	{
		randPos.setX(rand() % 1000);
		randPos.setY(rand() % 750);
		mTargetLoc = randPos;
	}
}

Steering* WanderChaseSteering::getSteering()
{
	cout << targetPlayer;
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	if ((playerPos - pOwner->getPositionComponent()->getPosition()).getLength() < 200)
	{
		targetPlayer = true;
		mTargetLoc = playerPos;
		cout << "TARGET THAT PLAYER";
	}
	else if (targetPlayer)
	{
		targetPlayer = false;
	}
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
	//directional facing
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	float dir = atan2(diff.getY(), diff.getX()) + atan(1) * 4 / 2;
	bool goRight;

	float modRadianstoOriente = -90 * 3.14 / 180;
	float rotation = fmod(atan2(diff.getY(), diff.getX()) - pOwner->getFacing() - 3.14, 2 * 3.14) + modRadianstoOriente;
	float rotationSize = abs(rotation);
	float targetRotation;
	if (rotationSize < 0)
	{
		return 0;
	}

	float slowRadius = 140;
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
	//end of face

	if (abs(data.rotAcc) > data.maxRotAcc)
	{
		data.acc /= abs(data.rotAcc);
		data.acc *= data.maxRotAcc;
	}

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	if (targetPlayer == false)
	{
		//create circle and offset it and set a point
		int radius = 50;
		int rot = rand() % 360;
		int offset = 100;
		Vector2D targetPoint;
		targetPoint.setX(cos(rot *3.14 / 180) * radius);
		targetPoint.setY(sin(rot *3.14 / 180) * radius);

		mTargetLoc = pOwner->getPositionComponent()->getPosition() + targetPoint + (diff * offset);

		if (mTargetLoc.getY() > 750)
		{
			mTargetLoc.setY(750);
		}

		if (mTargetLoc.getX() > 1000)
		{
			mTargetLoc.setX(1000);
		}
	}
	data.acc = diff;
	this->mData = data;
	return this;
}
