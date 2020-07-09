#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <windows.h>
#include <windowsx.h>
class Windows
{
protected:
	LPCTSTR WndClassName = L"Kevin Square Basic Manager";
	HWND g_hwnd = NULL;
	bool _isRun = true;
public:
	Windows() {};
	~Windows() { DestroyWindow(g_hwnd); };
	bool InitWin(HINSTANCE hInstance, int cmdShow);
	HWND GetWindowHandle() const { return g_hwnd; }
	
	void broadcast();
	bool IsRun();
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

};
#endif
