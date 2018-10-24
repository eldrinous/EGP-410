#include "GameApp.h"
#include "GameMessageManager.h"
#include "KeyboardInputMessage.h"
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
		dynamic_cast<GameApp*>(gpGame)->markForExit();
	}
	else if (mInput == KEY_MESS_D) //dijkstra
	{
		dynamic_cast<GameApp*>(gpGame)->newPathFind(2);
	}
	else if (mInput == KEY_MESS_F) //depth first search
	{
		dynamic_cast<GameApp*>(gpGame)->newPathFind(1);
	}

	else if (mInput == KEY_MESS_A) //a star
	{
		dynamic_cast<GameApp*>(gpGame)->newPathFind(3);
	}


}

