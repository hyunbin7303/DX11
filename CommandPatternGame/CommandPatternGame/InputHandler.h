#pragma once
#include "Command.h"
class InputHandler
{
public:
	InputHandler();

	Command* handleInput();
	void CommandBinding(Point* point);

private:
	Command* buttonJ;
	Command* buttonD;
	Command* buttonL;
	Command* buttonR;
	Command* buttonEnter;
};

