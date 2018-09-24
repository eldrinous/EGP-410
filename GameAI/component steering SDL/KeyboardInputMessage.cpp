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
		Unit* pUnit = gpGame->getUnitManager()->createRandomUnit(*gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));
	
	}
	else if (mInput == KEY_MESS_D)
	{
		gpGame->getUnitManager()->deleteRandomUnit();
	}
}

