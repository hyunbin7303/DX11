#ifndef _FONT_H_
#define _FONT_H_
#include "AllStruct.h"
#include <Windows.h>
#include <d3d11.h>

#include <D3D10_1.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>


class Font
{
public:
	Font(float Width, float Height);
	~Font();
	void InitD2DScreenTexture(ID3D11Device* d11Dev);
	bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter,ID3D11Device* d11Dev);
	void RenderText(ID3D11DeviceContext* devcon,std::wstring text, int inInt, XMMATRIX WVP, cbPerObject cbPerObject, ID3D11SamplerState* sampleState, ID3D11Buffer* cbPerObjectBuffer, ID3D11PixelShader* D2D_PS);
	void ReleaseAll();
private:
	ID3D10Device1 *d3d101Device;
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

	float Width;
	float Height;

};



#endif