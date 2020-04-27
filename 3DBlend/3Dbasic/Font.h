#ifndef _FONT_H_
#define _FONT_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>
#include <fstream>

using namespace DirectX;
class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	Font();
	Font(const Font&);
	~Font();

	bool InitFont(ID3D11Device* dev, char * str, WCHAR* fileName);
	void ShutDown();
	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArr(void *, char*, float, float);

private:
	bool LoadFontData(char* fileName);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* dev, WCHAR * wchar);
	void ReleaseTexture();

private:
	FontType * m_font;
	


};

#endif // !_FONT_H_
