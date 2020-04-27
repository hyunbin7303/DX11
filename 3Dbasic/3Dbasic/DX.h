#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <iostream>
#include "Model.h"

// include the Direct3D Library file
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};
class DX
{
private:
	// global declarations
	IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
	// Stores the buffer that are used for rendering data.
	// Also used to determine how the buffers are swapped when the rendered image should be
	// presented to the screen.
	Model * m_model;


	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;



	ID3D11Device			*dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext		*devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView	*g_pRenderTargetView;    // the pointer to our back buffer
	ID3D11InputLayout		*pLayout;
	ID3D11VertexShader		*pVS;
	ID3D11PixelShader		*pPS;
	//ID3D11Buffer			*pVBuffer = nullptr;
	//ID3D11Buffer*           g_pIndexBuffer = nullptr;
	ID3D11Buffer*		    g_ConstantBuffer = nullptr;


	ID3D11Texture2D*		 g_DepthStencilBuffer = nullptr;
	// Depth/stencil view for use as a depth buffer.
	ID3D11DepthStencilView*	 g_DepthStencilView = nullptr;
	ID3D11DepthStencilState* g_DepthStencilState = nullptr;
	ID3D11RasterizerState*	 g_RasterizerState = nullptr;
	ID3D11ShaderResourceView* g_texture = nullptr;

	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};
	HWND m_hwnd;
	float t; // time

	/*
	Constant buffers - used for storing shader variables that remain constant during 
	current draw call.

	An example of a constant shader variable is the camera’s projection matrix

	the projection matrix will be the same for every vertex of the object,
	X need to be passed to the shader using vertex data.

	*/	
	XMMATRIX g_world1;
	XMMATRIX g_world2;
	XMMATRIX g_ViewMatrix;
	XMMATRIX g_ProjectionMatrix;
	unsigned int clientWidth;
	unsigned int clientHeight;

public:
	DX();
	~DX();
										   // function prototypes
	int InitD3D(HWND hWnd);    // sets up and initializes Direct3D
	void RenderFrame(void);     // renders a single frame
	void Update(void);
	void CleanD3D(void);        // closes Direct3D and releases memory
	void InitGraphics(void);
	void InitPipeline(void);
	std::string getVersion();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	template<typename T>
	inline void SafeRelease(T& ptr)
	{
		if (ptr != NULL)
		{
			ptr->Release();
			ptr = NULL;
		}
	};
	

private:

};



#endif