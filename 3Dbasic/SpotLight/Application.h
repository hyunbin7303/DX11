#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <windows.h>
class Application
{
private:
	LPCTSTR WndClassName = L"FirstScreen View";
	HWND hwnd = NULL;
	double countsPerSecond = 0.0;
	__int64 CounterStart = 0;
	int frameCount = 0;
	int fps = 0;
	__int64 frameTimeOld = 0;
	double frameTime;

public:
	Application();
	~Application();
	bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int Height, int Width);

	HWND GetWindowHandle() const
	{
		return hwnd;
	}
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


};

#endif