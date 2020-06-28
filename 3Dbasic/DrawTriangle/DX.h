#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class DX
{

private:
	IDXGISwapChain* SwapChain;
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11RenderTargetView* renderTarget;
	ID3D11Buffer* drawVertexBuf;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11InputLayout* verLayout;

public:
	bool D3d11Init(int width, int height,  HWND g_hwnd);
	void ReleaseEverything();
	void Render();

};
#endif