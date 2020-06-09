#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#include "Font.h"
#include "FontShaderClass.h"

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float r, g, b;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texure;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Init_Text(ID3D11Device* dev, ID3D11DeviceContext* devcon, HWND, int, int, XMMATRIX);
	void ShutDown();
	bool Render(ID3D11DeviceContext* devcon, XMMATRIX, XMMATRIX);


private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device* dev);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext* devcon);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext* devcon, SentenceType*, XMMATRIX, XMMATRIX);


private:

	Font * m_Font;
	FontShaderClass* m_FontShader;
	int width, height;
	XMMATRIX m_baseViewMatrix;

	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};


#endif