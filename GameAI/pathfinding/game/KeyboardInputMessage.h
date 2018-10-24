#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

enum keyInput
{
	KEY_MESS_ESC,
	KEY_MESS_D,
	KEY_MESS_F,
	KEY_MESS_A
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