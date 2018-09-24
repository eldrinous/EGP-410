#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

enum keyInput
{
	KEY_MESS_ESC,
	KEY_MESS_ENTER,
	KEY_MESS_D
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