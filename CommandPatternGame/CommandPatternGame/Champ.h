#pragma once
class Champ
{

private:
	float loc_X;
	float loc_Y;
	
public:
	Champ();
	~Champ();



	float TotalWidth;
	float TotalHeight;
	float getWidth() const { return loc_X; }
	float getHeight() const { return loc_Y; }
	void setTotalWidth(float width) { TotalWidth = width; }
	void setTotalHeight(float height) {	TotalHeight = height; }

	float champLocationSettingX(float loc_X)
	{
		return ((loc_X / TotalWidth) * 10) * (TotalWidth / 10);
	}
	float champLocationSettingY(float loc_Y)
	{
		return ((loc_Y / TotalHeight) * 10) * (TotalHeight / 10);
	}
	void setWidth(float loc_X)
	{
		this->loc_X = loc_X;
	}
	void setHeight(float loc_Y)
	{
		this->loc_Y = loc_Y;
	}
};

