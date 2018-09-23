#include "InputManager.h"

InputManager::InputManager()
{
	mMousePos = (0, 0);
}

InputManager::~InputManager()
{

}

void InputManager::inputManagerUpdate()
{
	SDL_PumpEvents();

	int x, y;
	SDL_GetMouseState(&x, &y);
	mMousePos = (x, y);
}

Vector2D InputManager::getMousePos()
{
	return mMousePos;
}

bool InputManager::checkClick(keyPress key)
{

	SDL_PumpEvents();
	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	switch (key)
	{
	case MOUSELEFT:
		return SDL_BUTTON(SDL_BUTTON_LEFT);
		break;
	case MOUSERIGHT:
		return true;
		break;
	case ESCAPE_KEY:
		if(state[SDL_SCANCODE_ESCAPE])
		return true;
		break;
	default:
		return false;
		break;	
	}

}

