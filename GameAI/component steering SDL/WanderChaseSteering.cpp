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

	bool closerSenpai = false;
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
	float curDir = fmod(pOwner->getFacing(), 2 * 3.14);

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


	if (diff.getLength() < 140 && targetPlayer == false)
	{
		closerSenpai = true;
	}

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	

	if (closerSenpai && targetPlayer == false)
	{
		mTargetLoc.setX(rand() % 1000);
		mTargetLoc.setY(rand() % 750);
	}
	data.acc = diff;
	this->mData = data;
	return this;
}
