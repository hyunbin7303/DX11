#pragma once
#include "Champ.h"
class Point
{
private:

	float loc_X;
	float loc_Y;
	int loc_Tile_X;
	int loc_Tile_Y;

public:
	Point(Champ* champ);
	~Point();
	Champ* champ_;
	void setLocX(float Xvalue)
	{
		loc_X = Xvalue;
	}
	void setLocY(float Yvalue)
	{
		loc_Y = Yvalue;
	}
	float GetLocX()
	{
		return loc_X;
	}
	float GetLocY()
	{
		return loc_Y;
	}



	void jump() 
	{
		loc_Y -= 10.0f;
	}
	void GoRight() 
	{
		loc_X += 10.0f;
	}
	void GoLeft() 
	{
		loc_X -= 10.0f;
	}
	void GoDown() 
	{
		loc_Y += 10.0f;
	}

	void A_KeyPress();
	void S_KeyPress();
	void W_KeyPress();
	void D_KeyPress();
};

