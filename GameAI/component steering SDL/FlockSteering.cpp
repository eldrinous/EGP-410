#include <cassert>

#include "Steering.h"
#include "FlockSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cmath>
#include "CohesionSteering.h"
#include "SeperationSteering.h"
#include "AlignSteering.h"
#include "WanderSteering.h"
using namespace std;
FlockSteering::FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	setOwnerID(ownerID);
	setTargetID(targetID);

}

Steering* FlockSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D seperation, alignment, cohesion;

	//flocking behaviours
	SeperationSteering seperationSteer(mOwnerID, mTargetLoc, mTargetID);
	seperationSteer.getSteering();
	seperation = seperationSteer.getSeperation() * gpGame->getWeight(SEPERATE);;

	CohesionSteering cohesionSteer(mOwnerID, mTargetLoc, mTargetID);
	cohesionSteer.getSteering();
	cohesion = cohesionSteer.getCohesion() * gpGame->getWeight(COHESION);;
	
	AlignSteering alignSteer(mOwnerID, mTargetLoc, mTargetID);
	alignSteer.getSteering();
	alignment = alignSteer.getAlign() * gpGame->getWeight(ALIGN);

	Vector2D flockTarget =  seperation + cohesion + alignment;

	float addDegrees = -90 * 3.14 / 180;
	Vector2D orientationInVector(cos(pOwner->getFacing() + addDegrees), sin(pOwner->getFacing() + addDegrees));
	orientationInVector.normalize();
	//create circle and offset it and set a point
	int radius = 100;
	int rot = rand() % 360;
	int offset = 100;
	Vector2D targetPoint;
	targetPoint.setX(cos(rot *3.14 / 180) * radius);
	targetPoint.setY(sin(rot *3.14 / 180) * radius);

	mTargetLoc = targetPoint + (orientationInVector * offset);
	mTargetLoc *= .3;

	mTargetLoc += flockTarget;
	//face towards target;
	FaceSteering faceSteer(mOwnerID, mTargetLoc + pOwner->getPositionComponent()->getPosition(), mTargetID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data = faceSteer.getSteering()->getData();

	mTargetLoc.normalize();
	data.acc = mTargetLoc * pOwner->getMaxAcc();
	this->mData = data;
	return this;
}


