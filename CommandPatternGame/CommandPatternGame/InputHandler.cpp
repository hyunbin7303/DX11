#include "InputHandler.h"
#include <wrl.h>

using Microsoft::WRL::ComPtr;


void InputHandler::CommandBinding(Point* point)
{
	buttonJ = new JumpCommand(point, point->GetLocX(), point->GetLocY());
	buttonD = new DownCommand(point, point->GetLocY(), point->GetLocY());
	buttonL = new GoLeftCommand(point, point->GetLocY(), point->GetLocY());
	buttonR = new GoRightCommand(point, point->GetLocY(), point->GetLocY());
	buttonEnter = new EnterCommand(point, point->GetLocY(), point->GetLocY());
}

InputHandler::InputHandler()
{
}

Command* InputHandler::handleInput()
{
	if (GetAsyncKeyState(VK_UP))	// Upkey(Start game)
	{
		return buttonJ;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		return buttonD;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		return buttonL;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		return buttonR;
	}
	else if (GetAsyncKeyState(VK_RETURN))
	{
		return buttonEnter;
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		return buttonJ;

	}


	return nullptr;
}

