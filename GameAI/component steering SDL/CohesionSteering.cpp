#include <cassert>

#include "Steering.h"
#include "CohesionSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>

using namespace std;
CohesionSteering::CohesionSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	setOwnerID(ownerID);
	setTargetID(targetID);
}

Steering* CohesionSteering::getSteering()
{
	float scanDistance = 200;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	UnitManager* unitMan = gpGame->getUnitManager();
	Vector2D ownerPos = pOwner->getPositionComponent()->getPosition();
	Vector2D targetLoc = 0;

	std::map<UnitID, Unit*> tempMap = unitMan->getMap();
	int unitCount = 0;
	for (auto it = tempMap.begin(); it != tempMap.end(); ++it)
	{
		if (it->second != pOwner)
		{
			Vector2D pos(it->second->getPositionComponent()->getPosition());
			Vector2D diff = pos - ownerPos;
			float length = diff.getLength();
			
			Vector2D currVel = pOwner->getPhysicsComponent()->getVelocity();
			diff.normalize();
			currVel.normalize();
			float facing = VectorDotProduct(diff,currVel);
			if (length < scanDistance && facing > 0 ) //check if unit is within the neighborhood
			{
				unitCount++;
				targetLoc += pos;
			}
		}
	}

	if (unitCount > 0)
	{
		targetLoc /= unitCount; //find average position of all units excluding the owner
		targetLoc = targetLoc - ownerPos;
	}
	mCohesion = targetLoc;
	return this;
}

Vector2D CohesionSteering::getCohesion()
{
	return mCohesion;
}

float CohesionSteering::VectorDotProduct(Vector2D v1, Vector2D v2)
{
	return v1.getX() * v2.getX() + v1.getY() * v2.getY();
}
