#ifndef _FONTSHADER_H_
#define _FONTSHADER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
class FontShaderClass
{
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct PixelBufferType
	{
		XMFLOAT3 pixelColor;
	};
public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass &);
	~FontShaderClass();


	bool FontShader_Init(ID3D11Device* dev, HWND hwnd);
	void ShutDown();
	bool Render(ID3D11DeviceContext* devcon, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	bool Init_Shader(ID3D11Device* dev, HWND hwnd, WCHAR*, WCHAR*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext* devcon, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

};



#endif