#include <DX_PCH.h>
#include "DX.h"
#include "TimeManage.h"
const int g_Width = 550;
const int g_Height = 500;
HWND g_WindowHandle = NULL;	
LPCTSTR g_WindowClassName = L"DirectXWindowClass";
LPCSTR g_WindowName = "DirectX Template";
#define IDI_ICON1                       101
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int InitApplication(HINSTANCE hInstance, int cmdShow, int Width, int Height);
int msgLoop();
DX *dx;
using namespace std;
using namespace DirectX;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	if (!XMVerifyCPUSupport())
	{
		MessageBox(nullptr, TEXT("Direct-X lib fail."), TEXT("ERROR"), MB_OK);
		return -1;
	}
	if (!InitApplication(hInstance, cmdShow, g_Width, g_Height))
	{
		MessageBox(nullptr, TEXT("Window fail."), TEXT("ERROR"), MB_OK);
		return -1;
	}
	dx = new DX(g_Width, g_Height);
	if (!dx->DX11Init(g_WindowHandle))
	{
		MessageBox(nullptr, TEXT("Direct 11 Initialization fail."), TEXT("ERROR"), MB_OK);
		return -1;
	}
	if(!dx->DX11ScreenInit())
	{
		MessageBox(nullptr, TEXT("DirectX Screen Initialization fail."), TEXT("ERROR"), MB_OK);
		return -1;
	}
	msgLoop();

	
	dx->CleanUp(g_WindowHandle);
	return 0;
}

int InitApplication(HINSTANCE hInstance,int cmdShow, int Width, int Height)
{
	typedef struct _WNDCLASS {
		UINT cbSize;
		UINT style;
		WNDPROC lpfnWndProc;
		int cbClsExtra;
		int cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
	} WNDCLASS;

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = (LPCWSTR)g_WindowClassName;
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	g_WindowHandle = CreateWindowEx(
		NULL,
		g_WindowClassName,
		L"My DirectX Template",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		Width, Height,
		NULL, NULL,
		hInstance,
		NULL);
	if (!g_WindowHandle)
	{
		return -1;
	}

	ShowWindow(g_WindowHandle, cmdShow);
	UpdateWindow(g_WindowHandle);
	return 1;
}
int msgLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		BOOL PeekMessageL(
			LPMSG lpMsg,
			HWND hWnd,
			UINT wMsgFilterMin,
			UINT wMsgFilterMax,
			UINT wRemoveMsg
		);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// run game code    
			frameCount++;
			if (GetTime() > 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				StartTimer();
			}

			frameTime = GetFrameTime();

			//DetectInput(frameTime);
			dx->Update(frameTime);
			dx->Render();

		}
	}
	return msg.wParam;
}
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		message,
		wParam,
		lParam);
}