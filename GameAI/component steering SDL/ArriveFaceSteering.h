#include <Trackable.h>
#include "Steering.h"

class ArriveFaceSteering : public Steering
{
public:
	ArriveFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

protected:
	virtual Steering* getSteering();
};

