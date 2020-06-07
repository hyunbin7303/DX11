#include "TestGame.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;
TestGame::TestGame(HINSTANCE hInstance) : DXApp(hInstance)
{

	champ = new Champ();
	point = new Point(champ);
	input = new InputHandler();
	y = 0.0f;
	
}
TestGame::~TestGame()
{
}

bool TestGame::Init()
{
	input->CommandBinding(point);
	if (!DXApp::Init())
	{
		return false;
	}


	return true;
}

void TestGame::Update(float dt)
{
	ySpeed = 2.0f;
	champ->setTotalWidth(BoardWidth);
	champ->setTotalHeight(BoardHeight);
	command = input->handleInput();
	if (command)
	{
		command->execute();
	}

	locationStr = std::to_string(point->GetLocX()) + " - " + std::to_string(point->GetLocY());
	LocTileX = point->GetLocX() / graphic->getBoardWidth() * 10;
	LocTileY = point->GetLocY() / graphic->getBoardHeight() * 10;
	locationTileStr = std::to_string(LocTileX) + " - " + std::to_string(LocTileY);

	
}

void TestGame::Render(float dt)
{
	graphic->ClearScreen(0.0f, 0.0f, 0.5f);
	graphic->GridLine(1.0f,1.0f,1.0f,1.0f);
	graphic->DrawTriangle(0.0f, 0.0f); // Assuming that this is player.
//	graphic->DrawSmallTriangle(champ->GetLocX(), champ->GetLocY()); // Assuming that this is player.
	graphic->DrawString(locationStr, 0.0f, 100.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	graphic->DrawString(locationTileStr, 0.0f, 150.0f, 1.0f, 0.0f, 0.0f, 1.0f); // Gonna display area number.
	graphic->DrawCircle(point->GetLocX(), point->GetLocY(), 10.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	graphic->DrawCircle(champ->getWidth(), champ->getHeight(), 20.0f, 0.0f, 1.0f, 0.0f, 1.0f);

}
