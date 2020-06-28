#include "DX.h"
#include "WindowsApp.h"
using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WindowsApp windowApp;
	if (!windowApp.InitWin(hInstance, nCmdShow))
	{
		return -1;
	}

	DX dx;
	if (!dx.D3d11Init(800, 600, windowApp.GetWindowHandle()))
		return -1;

	ShowWindow(windowApp.GetWindowHandle(), nCmdShow);
	UpdateWindow(windowApp.GetWindowHandle());
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dx.Render();
		}
	}
	dx.ReleaseEverything();
	UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}

