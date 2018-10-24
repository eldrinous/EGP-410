#include "InputManager.h"
#include "GameMessage.h"
#include "PathToMessage.h"
#include "KeyboardInputMessage.h"
#include "GameMessageManager.h"
#include "GameApp.h"

InputManager::InputManager()
{
	mMousePos = (0, 0);
}

InputManager::~InputManager()
{

}

void InputManager::inputManagerUpdate()
{
	SDL_Event nextEvent;
	
	int x, y;
	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		static Vector2D lastPos(0.0f, 0.0f);
		Vector2D pos(x, y);
		if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY())
		{
			GameMessage* pMessage = new PathToMessage(lastPos, pos);
			dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			lastPos = pos;

		}
	}

	while (true)
	{
		GameMessage * pMessage = NULL;
		if (!SDL_PollEvent(&nextEvent)) {
			return;
		}
		switch (nextEvent.type)
		{
		case SDL_KEYDOWN:
	
			if (nextEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				//send to message that should exit
				pMessage = new KeyboardToMessage(KEY_MESS_ESC);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_D)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_D);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_F)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_F);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_A)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_A);
			}
			if (pMessage != NULL)
			{
				dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			}

			break;
		case SDL_KEYUP:
			break;
		case SDL_QUIT: //clicks on red X
			
			break;
		default:
			break;
		}
	}
}

Vector2D InputManager::getMousePos()
{
	return mMousePos;
}


