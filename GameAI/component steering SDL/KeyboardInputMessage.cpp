#include "Game.h"
#include "GameMessageManager.h"
#include "KeyboardInputMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"

using namespace std;
KeyboardToMessage::KeyboardToMessage(const keyInput& input)
	:GameMessage(KEYBOARD_INPUT)
{
	cout << input << endl;
	mInput = input;
}

KeyboardToMessage::~KeyboardToMessage()
{
}

void KeyboardToMessage::process()
{
	if (mInput == KEY_MESS_ESC)
	{
		gpGame->modifyExit(true);
	}
	else if (mInput == KEY_MESS_ENTER)
	{
		for (int i = 0; i < 10; i++)
		{
			Unit* pUnit = gpGame->getUnitManager()->createRandomUnit(*gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));

		}
	}
	else if (mInput == KEY_MESS_D)
	{
		gpGame->getUnitManager()->deleteRandomUnit();
	}
	else if (mInput == KEY_MESS_Q)
	{
		gpGame->modifyWeight(-.1, ALIGN);
	}
	else if (mInput == KEY_MESS_W)
	{
		gpGame->modifyWeight(.1, ALIGN);
	}
	else if (mInput == KEY_MESS_E)
	{
		gpGame->modifyWeight(-.1, COHESION);
	}
	else if (mInput == KEY_MESS_R)
	{
		gpGame->modifyWeight(.1, COHESION);
	}
	else if (mInput == KEY_MESS_T)
	{
		gpGame->modifyWeight(-.1, SEPERATE);
	}
	else if (mInput == KEY_MESS_Y)
	{
		gpGame->modifyWeight(.1, SEPERATE);
	}
}

