#include <cassert>

#include "Steering.h"
#include "WanderChaseSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>
#include "WanderSteering.h"
#include "SeekSteering.h"
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
}

Steering* WanderChaseSteering::getSteering()
{

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	mPlayerPos = gpGame->getUnitManager()->getUnit(mTargetID)->getPositionComponent()->getPosition();
	
	//check distance of the target position and owner position
	if ((mPlayerPos - pOwner->getPositionComponent()->getPosition()).getLength() < 200)
	{
		targetPlayer = true;	
	}
	else
	{
		targetPlayer = false;
	}

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (targetPlayer)
	{
		mTargetLoc = mPlayerPos;
		SeekSteering seekSteer(mOwnerID, mTargetLoc, mTargetID);
		data = seekSteer.getSteering()->getData();
	}
	else
	{
		//wander
		WanderSteering wanderSteer(mOwnerID, mTargetLoc, mTargetID);
		data = wanderSteer.getSteering()->getData();
		mTargetLoc = wanderSteer.getTargetLoc();
	}


	this->mData = data;
	return this;
}
