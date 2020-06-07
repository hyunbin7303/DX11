#include "Point.h"
#include "Graphics.h"




Point::Point(Champ* champ)
	: loc_X(0.0f), loc_Y(0.0f), champ_(champ)
{
	champ_ = champ;
}

Point::~Point()
{
}



// Selected Tile

void Point::A_KeyPress()
{
}

void Point::S_KeyPress()
{
}

void Point::W_KeyPress()
{
}

void Point::D_KeyPress()
{
}
