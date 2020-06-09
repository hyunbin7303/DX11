#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include <windows.h>
#include <windowsx.h>

class WindowsApp
{
private:
	LPCTSTR WndClassName = L"Kevin Square Basic Manager";
	HWND hWnd = NULL;
public:
	WindowsApp();
	~WindowsApp();
	int InitWin(HINSTANCE hInstance, int cmdShow);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetWindowHandle() const {return hWnd;}

};




#endif // !_APPLICATION_H_



