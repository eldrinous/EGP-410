#include <Trackable.h>
#include "Steering.h"

class CohesionSteering : public Steering
{
public:
	CohesionSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	virtual Steering* getSteering();
	Vector2D getCohesion();
protected:
	Vector2D mCohesion;
};
