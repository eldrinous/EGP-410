#include <Trackable.h>
#include "Steering.h"

class SeperationSteering : public Steering
{
public:
	SeperationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	virtual Steering* getSteering();
	Vector2D getSeperation();
protected:
	Vector2D mSeperation;
};

