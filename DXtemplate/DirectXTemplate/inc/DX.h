#ifndef DX_H_
#define DX_H_

#include "Font.h"
#include "Camera.h"
#include "WICTextureLoader.h"
class DX
{

private:
	IDXGISwapChain * SwapChain;
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Buffer* squareIndexBuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D *BackBuffer11;
	ID3D11Texture2D * depthStencilBuffer;
	ID3D11Buffer* squareVertBuffer;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11PixelShader* D2D_PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D10Blob* D2D_PS_Buffer;
	ID3D11InputLayout* vertLayout;
	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11BlendState* Transparency;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;
	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11Buffer* cbPerFrameBuffer;
	int Width, Height;





	XMMATRIX groundWorld;
	XMMATRIX Rotation;
	XMMATRIX Scale;
	XMMATRIX Translation;
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;

public:
	DX();
	DX(int width, int height);
	~DX();
	bool DX11Init(HWND hWnd);
	bool DX11ScreenInit();

	void Update(double time);
	void Render();
	void CleanUp(HWND hwnd);

private:
	Font * font;
	Light light;
	Camera* camera;
	cbPerFrame constbuffPerFrame;
	cbPerObject cbPerObj;

	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX WVP;

};
#endif