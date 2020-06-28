#include "WindowsApp.h"
int WindowsApp::InitWin(HINSTANCE hInstance, int cmdShow)
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
	if (!RegisterClassEx(&wcx))
		MessageBox(NULL, L"WTF, Registering Fail. ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);


	g_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"Triangle",
		L"Kevin- Simple Triangle",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (!g_hwnd)
		return -1;
}
LRESULT CALLBACK WindowsApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,
		message,
		wParam,
		lParam);
}
