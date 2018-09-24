#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include <sstream>
#include "Trackable.h"
#include "PerformanceTracker.h"
#include <Vector2D.h>

class InputManager: public Trackable
{
public:
	InputManager();
	~InputManager();

	void inputManagerUpdate();
	Vector2D getMousePos();
private:
	Vector2D mMousePos;
};

