
#include "WindowsApp.h"
#include "DX.h"

#define ERROR -1;


HWND hWnd = 0; // global window handle variable.


int InitWin(HINSTANCE hInstance, int cmdShow);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!XMVerifyCPUSupport())
	{
		MessageBox(nullptr, TEXT("Failed to verify DirectX Math library support."), TEXT("Error"), MB_OK);
		return ERROR;
	}


	WindowsApp* windowsApp;
	windowsApp = new WindowsApp();
	if (windowsApp->InitWin(hInstance, nCmdShow)!=0)
	{
		MessageBox(nullptr, TEXT("FAILED TO CREWATE WIN APPLICATION"), TEXT("ERROR"), MB_OK);
		return ERROR;
	}

	DX dx;
	if (dx.InitD3D(windowsApp->GetWindowHandle()) != 0)
	{
		MessageBox(nullptr, TEXT("FAILED TO CREWATE DIRECTX 3D."), TEXT("ERROR"), MB_OK);
		return ERROR;

	}
	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		dx.Update();
		dx.RenderFrame();
	}
	dx.CleanD3D();
	return msg.wParam;
}


