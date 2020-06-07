#pragma once
#include <d2d1.h>
#include <Dwrite.h>
#include <Windows.h>
#include <iostream>
class Graphics
{


	ID2D1Factory* factory; //The factory allows us to create many other types of D2D resources
	ID2D1HwndRenderTarget* rendertarget; //this is typically an area in our GPU memory.. like a back buffer 
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!


private:
	float BoardWidth;
	float BoardHeight; 
	float EachWidth;
	float EachHeight;

public:
	Graphics();
	~Graphics();
	bool Init(HWND windowHandle);

public:

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawTriangle(int x, int y);
	void DrawSmallTriangle(int x, int y);
	void DrawString(std::string str, float x, float y, float r, float g, float b, float a);
	void GridLine(float r, float g, float b, float a);

	
	float getBoardWidth() const
	{
		return BoardWidth;
	}
	float getBoardHeight() const
	{
		return BoardHeight;
	}
	float getEachWidth() const {
		return EachWidth;
	}
	float getEachHeight() const {
		return EachHeight;
	}
	
};

