#pragma once
#include "Graphics.h"

class Board
{
public:
	Board(Graphics& gfx);

	void InitBoard(); 
	bool IsFull() const;

private:
	static constexpr int dimension = 10;
	static constexpr int width =	 10;
	static constexpr int height =	 10;
	float EachWidth;
	float EachHeight;
	
	float BoardWidth;
	float BoardHeight;

	int boardTile[width][height];
	Graphics& gfx;
};
/*
0 means empty
1 means exist 
*/