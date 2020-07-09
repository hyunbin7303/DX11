#include "Windows.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


bool Windows::InitWin(HINSTANCE hInstance, int cmdShow)
{
	// Initializing application 
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	// CS_VREDRAW - The window is redrawn if there is a change in the window's height or if the window is moved vertically.
	wcx.lpfnWndProc = WindowProc; // Point to window Procedure
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = L"MainMenu";
	wcx.lpszClassName = L"Triangle";
	wcx.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(5), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	if (!RegisterClassEx(&wcx)) {

		MessageBox(NULL, L"WTF, Registering Fail. ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	g_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"Triangle",
		L"Kevin- Simple Triangle",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (!g_hwnd)
		return false;

	return true;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}
		if (wParam == 'D')
		{
			SetWindowText(hWnd, L"Respects");
		}
		if (wParam == 'F')
		{
			MessageBox(hWnd, L"F Clicked", L"OKAY", MB_OK);
		}

		break;
	case WM_CREATE:
	{
		auto* const params = reinterpret_cast<LPCREATESTRUCT>(lParam);
		auto* const window = reinterpret_cast<Windows* const>(params->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		window->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		auto* const window = reinterpret_cast<Windows* const>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		window->onDestroy();
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd,
			message,
			wParam,
			lParam);
	}
}

void Windows::broadcast()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	onUpdate();

	Sleep(1);
}

bool Windows::IsRun()
{
	return _isRun;
}

void Windows::onCreate()
{
}

void Windows::onUpdate()
{
}

void Windows::onDestroy()
{
	_isRun = false;
}
