#include "DXApp.h"

namespace
{
	DXApp* g_pApp = nullptr; // forward messages to user defined proc function.?
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp) return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hInstance)
{
	m_hAppInstance = hInstance;
	m_hAppWnd = NULL;
	m_ClientWidth = 800;
	m_ClientHeight = 600;
	m_AppTitle = "Turn Based Game";
	m_WndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;
}

DXApp::~DXApp()
{ 


}

int DXApp::Run()
{


	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{ // PeekMessage - Dispatches incoming sent messages.
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			Update(0.0f);
			graphic->BeginDraw();
			Render(0.0f);
			graphic->EndDraw();
		}
	}
	return static_cast<int>(msg.wParam);
}

bool DXApp::Init()
{
	if (!InitWindow())
	{
   		return false;
	}
	return true;
}
LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0; // Post quit message to pipe line.
	case WM_LBUTTONDOWN:
	/*	int x = LOWORD(lParam);
		int y = HIWORD(lParam);

*/

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool DXApp::InitWindow()
{

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = m_hAppInstance;
	wcex.lpfnWndProc = MainWndProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "Turn Based Game";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("\nFAILED TO CREATE WINDOW CLASS\n");

		return false;
	}
	RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&r, m_WndStyle, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;
	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width  / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	m_hAppWnd = CreateWindow("Turn Based Game", m_AppTitle.c_str(), m_WndStyle, x, y, width, height, NULL, NULL, m_hAppInstance, NULL);
	if (!m_hAppWnd)
	{
		OutputDebugString("\nFailed to create window class \n");
		return false;
	}

	graphic = new Graphics();
	if (!graphic->Init(m_hAppWnd))
	{
		delete graphic;
		return false;
	}
	ShowWindow(m_hAppWnd, SW_SHOW);
	return true;

}
