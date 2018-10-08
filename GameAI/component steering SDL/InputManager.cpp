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
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				//send to message that should create unit
				pMessage = new KeyboardToMessage(KEY_MESS_ENTER);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_D)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_D);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_Q)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_Q);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_W)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_W);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_E)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_E);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_R)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_R);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_T)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_T);
			}
			else if (nextEvent.key.keysym.scancode == SDL_SCANCODE_Y)
			{
				//send to message that should delete a unit
				pMessage = new KeyboardToMessage(KEY_MESS_Y);
			}
			if (pMessage != NULL)
			{
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


