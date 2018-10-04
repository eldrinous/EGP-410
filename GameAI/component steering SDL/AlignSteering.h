#include <Trackable.h>
#include "Steering.h"

class AlignSteering : public Steering
{
public:
	AlignSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	virtual Steering* getSteering();
	Vector2D getAlign();
protected:
	Vector2D mAlign;
};

