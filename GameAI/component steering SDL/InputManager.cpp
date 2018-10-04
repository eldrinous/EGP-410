#include "InputManager.h"
#include "GameMessage.h"
#include "PlayerMoveToMessage.h"
#include "KeyboardInputMessage.h"
#include "GameMessageManager.h"
#include "Game.h"

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
		Vector2D pos(x, y);
		mMousePos = pos;
		GameMessage* pMessage = new PlayerMoveToMessage(pos);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	while (true)
	{
		if (!SDL_PollEvent(&nextEvent)) {
			return;
		}
		switch (nextEvent.type)
		{
		case SDL_KEYDOWN:
			GameMessage * pMessage;
			if (nextEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				//send to message that should exit
				pMessage = new KeyboardToMessage(KEY_MESS_ESC);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				//send to message that should create unit
				pMessage = new KeyboardToMessage(KEY_MESS_ENTER);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_D)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_D);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
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


