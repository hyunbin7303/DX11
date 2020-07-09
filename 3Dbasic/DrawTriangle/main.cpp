#include "DX.h"
#include "Application.h"
using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application app;
	if (!app.InitWin(hInstance, nCmdShow))
	{
		return -1;
	}
	while (app.IsRun())
	{
		app.broadcast();
	}
	//DX dx;
	//if (!dx.D3d11Init(800, 600, app.GetWindowHandle()))
	//	return -1;

	//ShowWindow(app.GetWindowHandle(), nCmdShow);
	//UpdateWindow(app.GetWindowHandle());
	//MSG msg;
	//ZeroMemory(&msg, sizeof(MSG));
	//while (true)
	//{
	//	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	//	{
	//		if (msg.message == WM_QUIT)
	//		{
	//			break;
	//		}
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//	else
	//	{
	//		dx.Render();
	//	}
	//}
	//dx.ReleaseEverything();
	//UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}

