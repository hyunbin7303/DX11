#include "WindowsApp.h"

WindowsApp::WindowsApp()
{
}
WindowsApp::~WindowsApp()
{
}

int WindowsApp::InitWin(HINSTANCE hInstance, int cmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Our First Direct3D Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"Error creating window",L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return 0;
}

LRESULT WindowsApp::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,
		msg,
		wParam,
		lParam);
}
