#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincodec.h>
#include <string>
#include "DXUtil.h"
#include "Graphics.h"

class DXApp
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp(void);
	// MAIN Application Run
	int Run();
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
	Graphics* graphic;

protected:
	// WIN32 Attributes
	HWND m_hAppWnd;				// window screen.
	HINSTANCE m_hAppInstance;
	UINT m_ClientWidth;			// Width of the whole screen. 
	UINT m_ClientHeight;		// Height of the whole screen.
	std::string m_AppTitle;
	DWORD m_WndStyle; 

	//Direct X Attributes
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* rendertarget;
protected:
	// Initialize WIN32 WINDOW
	bool InitWindow();

};


