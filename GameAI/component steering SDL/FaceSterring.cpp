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
	bool slowRadius = 10;

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	float modRadianstoOriente = 90 * 3.14 / 180;
	float rotation = fmod(atan2(diff.getY(), diff.getX()) - pOwner->getFacing(), 2 * 3.14) + modRadianstoOriente;
	float rotationSize = abs(rotation);
	float targetRotation;
	

	if (rotationSize > slowRadius)
	{
		targetRotation = pOwner->getMaxRotVel();
	}
	else
	{
		targetRotation = pOwner->getMaxRotVel() * rotationSize / slowRadius;
	}

	if (rotationSize < 5 * 3.14 / 180)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}

	targetRotation *= rotation / rotationSize;

	data.rotAcc = targetRotation - data.rotVel;

	data.rotAcc /= .1;

	if (abs(data.rotAcc) > pOwner->getMaxRotAcc())
	{
		data.rotAcc /= abs(data.rotAcc);
		data.rotAcc *= pOwner->getMaxRotAcc();
	}

	this->mData = data;
	return this;
}
