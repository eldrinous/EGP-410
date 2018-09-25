#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "FaceSteering.h"
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
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	
	float addDegrees = -90 * 3.14/180;

	Vector2D orientationInVector(cos(pOwner->getFacing() + addDegrees), sin(pOwner->getFacing() + addDegrees));
	orientationInVector.normalize();

	//create circle and offset it and set a point
	int radius = 200;
	int rot = rand() % 360;
	int offset = 200;
	Vector2D targetPoint;
	targetPoint.setX(cos(rot *3.14 / 180) * radius);
	targetPoint.setY(sin(rot *3.14 / 180) * radius);

	mTargetLoc = pOwner->getPositionComponent()->getPosition() +  targetPoint + (orientationInVector * offset);
	
	//face towards target;
	FaceSteering faceSteer(mOwnerID, mTargetLoc, mTargetID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data = faceSteer.getSteering()->getData();
	

	data.acc = orientationInVector * pOwner->getMaxAcc();
	this->mData = data;
	return this;
}


