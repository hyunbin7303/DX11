// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <algorithm>
#include "DX.h"
#include "Resource.h"
#pragma comment(lib, "winmm.lib")


HWND hWnd = 0; // global window handle variable.


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int InitWin(HINSTANCE hInstance, int cmdShow);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!XMVerifyCPUSupport())
	{
		MessageBox(nullptr, TEXT("Failed to verify DirectX Math library support."), TEXT("Error"), MB_OK);
		return ERROR;
	}
	if (InitWin(hInstance, nCmdShow)!=0)
	{
		MessageBox(nullptr, TEXT("FAILED TO CREWATE WIN APPLICATION"), TEXT("ERROR"), MB_OK);
		return ERROR;
	}
	DX dx;
	// set up and initialize Direct3D
	if (dx.InitD3D(hWnd) != 0)
	{
		MessageBox(nullptr, TEXT("FAILED TO CREWATE DIRECTX 3D."), TEXT("ERROR"), MB_OK);
		return ERROR;

	}
	// enter the main loop:

	MSG msg;
	static DWORD preTime = timeGetTime();
	static const float targetFrame = 30.0f;
	static const float maxTimeStep = 1.0f / targetFrame;


	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - preTime) / 1000.0f;
			preTime = currentTime;
			deltaTime = std::min<float>(deltaTime, maxTimeStep);
			dx.Update(deltaTime);
			dx.RenderFrame();
		}

	}

	// clean up DirectX and COM
	dx.CleanD3D();
	return msg.wParam;
}
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
int InitWin(HINSTANCE hInstance, int cmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.lpszClassName = L"WindowClass";
	if (!RegisterClassEx(&wc))
	{
		return -1;
	}

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Texture Test",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, cmdShow);
	return 0;
}


