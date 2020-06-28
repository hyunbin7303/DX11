#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <windows.h>
#include <windowsx.h>
class WindowsApp
{
private:
	LPCTSTR WndClassName = L"Kevin Square Basic Manager";
	HWND g_hwnd = NULL;
public:
	WindowsApp() {};
	~WindowsApp() {};
	int InitWin(HINSTANCE hInstance, int cmdShow);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetWindowHandle() const { return g_hwnd; }


};
#endif
