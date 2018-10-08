#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

enum keyInput
{
	KEY_MESS_ESC,
	KEY_MESS_ENTER,
	KEY_MESS_D,
	KEY_MESS_Q,
	KEY_MESS_W,
	KEY_MESS_E,
	KEY_MESS_R,
	KEY_MESS_T,
	KEY_MESS_Y
};

class KeyboardToMessage :public GameMessage
{
public:
	KeyboardToMessage(const keyInput& mInput);
	~KeyboardToMessage();

	void process();

private:
	keyInput mInput;
};