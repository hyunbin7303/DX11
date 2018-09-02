#ifndef FONT_H_
#define FONT_H_

#pragma comment(lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")

#include "AllStruct.h"
#include <D3D10_1.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class Font
{
public:
	Font(int Width, int Height);
	~Font();
	
private:
	ID3D10Device1 * d3d101Device;
	IDXGIKeyedMutex *keyedMutex11;
	IDXGIKeyedMutex *keyedMutex10;
	ID2D1RenderTarget *D2DRenderTarget;
	ID2D1SolidColorBrush *Brush;
	ID3D11Texture2D *sharedTex11;
	ID3D11Buffer *d2dVertBuffer;
	ID3D11Buffer *d2dIndexBuffer;
	ID3D11ShaderResourceView *d2dTexture;
	IDWriteFactory *DWriteFactory;
	IDWriteTextFormat *TextFormat;
	std::wstring printText;
	int width, height;
public:
	void InitD2DScreenTexture(ID3D11Device* d11Dev);
	bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter, ID3D11Device* d11Dev);
	void RenderText(ID3D11DeviceContext* devcon, std::wstring text, int inInt, XMMATRIX WVP, cbPerObject cbPerObject, ID3D11SamplerState* sampleState, ID3D11Buffer* cbPerObjectBuffer, ID3D11PixelShader* D2D_PS);
	void ReleaseAll();

};

#endif FONT_H_