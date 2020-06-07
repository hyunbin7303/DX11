#include "Board.h"



Board::Board(Graphics & gfx)
	: gfx(gfx)
{
	InitBoard();
	EachWidth = gfx.getEachWidth();
	EachHeight = gfx.getEachHeight();

}

void Board::InitBoard()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			boardTile[i][j] = 0;
		}
	}

}

