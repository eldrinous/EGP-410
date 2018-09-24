#include <Trackable.h>
#include "Steering.h"

class WanderChaseSteering : public Steering
{
public:
	WanderChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

protected:
	virtual Steering* getSteering();
	bool targetPlayer = false;
	Vector2D mPlayerPos;
	Vector2D randPos;
};
