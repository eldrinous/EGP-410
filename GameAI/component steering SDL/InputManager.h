#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include <sstream>
#include "Trackable.h"
#include "PerformanceTracker.h"
#include <Vector2D.h>

enum keyPress
{
	MOUSELEFT,
	MOUSERIGHT,
	ESCAPE_KEY
};

class InputManager: public Trackable
{
public:
	InputManager();
	~InputManager();

	void inputManagerUpdate();
	Vector2D getMousePos();
	bool checkClick(keyPress key);
private:
	Vector2D mMousePos;
};

