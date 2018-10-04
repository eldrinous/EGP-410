#include <cassert>

#include "Steering.h"
#include "SeperationSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>

using namespace std;
SeperationSteering::SeperationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	setOwnerID(ownerID);
	setTargetID(targetID);
}

Steering* SeperationSteering::getSteering()
{
	float scanDistance = 50;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	UnitManager* unitMan = gpGame->getUnitManager();
	Vector2D ownerPos = pOwner->getPositionComponent()->getPosition();
	Vector2D targetLoc = 0;

	std::map<UnitID, Unit*> tempMap = unitMan->getMap();

	for (auto it = tempMap.begin(); it != tempMap.end(); ++it)
	{
		Vector2D pos(it->second->getPositionComponent()->getPosition());
		if ((pos - ownerPos).getLength() < scanDistance) //check if unit is within the neighborhood
		{
			targetLoc += pos - ownerPos;
		}
	}
	targetLoc *= -1; // move in the other oposite direction

	mSeperation = targetLoc;
	return this;
}

Vector2D SeperationSteering::getSeperation()
{
	return mSeperation;
}


