#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <iostream>
#include "Model.h"
#include "Camera.h"

using namespace DirectX;


class DX
{
private:
	Model * m_model;
	Model * m_model2;
	Camera* m_camera;

	ID3D11Buffer			*g_constantBuf[NumCB];
	IDXGISwapChain			*swapchain;             // the pointer to the swap chain interface
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device			*dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext		*devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView	*g_pRenderTargetView;    // the pointer to our back buffer
	ID3D11InputLayout		*pLayout;
	ID3D11VertexShader		*pVS;
	ID3D11PixelShader		*pPS;


	ID3D11Texture2D*			g_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*		g_DepthStencilView = nullptr;
	ID3D11DepthStencilState*	g_DepthStencilState = nullptr;
	ID3D11RasterizerState*		g_RasterizerState = nullptr;

	ID3D11BlendState* Transparency = nullptr;
	ID3D11RasterizerState* CCWcullMode = nullptr;
	ID3D11RasterizerState* CWcullMode = nullptr;


	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};
	struct CBNeverChanges
	{
		XMMATRIX mView;
	};
	struct CBChangeOnResize
	{
		XMMATRIX mProjection;
	};
	struct CBChangesEveryFrame
	{
		XMMATRIX mWorld;
	};

	HWND m_hwnd;
	float t; // time

	XMMATRIX WVP;
	XMMATRIX Cube1_world;
	XMMATRIX ManyCubes_world[10];
	XMMATRIX g_ViewMatrix;
	XMMATRIX g_ProjectionMatrix;
	unsigned int clientWidth;
	unsigned int clientHeight;
	XMFLOAT4  g_vMeshColor;

public:
	DX();
	~DX();
										   // function prototypes
	int InitD3D(HWND hWnd);    // sets up and initializes Direct3D
	void RenderFrame(void);     // renders a single frame
	void ClearScreen();
	void Update(float time);
	void CleanD3D(void);        // closes Direct3D and releases memory
	bool InitGraphics(void);
	bool InitShader(void);
	std::string getVersion();
	template<typename T>
	inline void SafeRelease(T& ptr)
	{
		if (ptr != NULL)
		{
			ptr->Release();
			ptr = NULL;
		}
	};
	
};



#endif