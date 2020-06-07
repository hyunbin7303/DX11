#pragma once
#include "DXApp.h"
#include "Champ.h"
#include "InputHandler.h"
#include <iostream>
//using Microsoft::WRL::ComPtr;

class TestGame : public DXApp
{
private:

	int LocTileX;
	int LocTileY;

	float ySpeed;
	float y;
//	ComPtr<ID3D11Device1> dev;              // the device interface
public:
	Point * point;
	Champ * champ;
	Command* command;
	InputHandler* input;
	std::string locationStr;
	std::string locationTileStr;
	float BoardWidth;
	float BoardHeight;
public:
	TestGame(HINSTANCE hInstance);
	~TestGame();
	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
};

