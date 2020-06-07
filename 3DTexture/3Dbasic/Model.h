#ifndef MODEL_H_
#define MODEL_H_
#include <d3d11.h>
#include <string>
#include <iostream>
#include <directxmath.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

using namespace DirectX;
enum ConstanBuffer
{
	cbNeverChange,// mView
	cbChangeOnResize, // myProjection
	cbFrame, //world
	NumCB,
};
struct CBChangesEveryFrame
{
	XMMATRIX mWorld;
};
class Model
{
private:
	static constexpr int numVertice = 24;
	static constexpr int numIndex = 36;
	ID3D11Buffer* g_constantBuf[NumCB];
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 Tex;

	};
	ID3D11Buffer*						pVBuffer = nullptr;
	ID3D11Buffer*						g_pIndexBuffer = nullptr;
	ID3D11ShaderResourceView*           g_pTextureRV = nullptr;
	ID3D11SamplerState*                 g_pSamplerLinear = nullptr;

	
public:
	Model();
	Model(const Model& model);
	ID3D11SamplerState* GetSamplerLinear()
	{
		return g_pSamplerLinear;
	}
	bool ModelInit(ID3D11Device* dev, std::string fileName);
	void RenderModel(ID3D11DeviceContext* devcon);
	void Rendering(ID3D11DeviceContext* devcon, XMMATRIX worldMatrix);
	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return g_pTextureRV;
	}
	ID3D11VertexShader		*pVS;
	ID3D11PixelShader		*pPS;
	void SetShader(ID3D11VertexShader* pvs, ID3D11PixelShader* pps);
	bool LoadTextureFile(ID3D11Device* dev, std::string fileName);



	ID3D11Buffer* GetpcBNeverChanges()
	{
		return g_constantBuf[cbNeverChange];
	}
	ID3D11Buffer* GetpcBChangesOnResize()
	{
		return g_constantBuf[cbChangeOnResize];
	}
	ID3D11Buffer* GetChangesEveryFrame()
	{
		return g_constantBuf[cbFrame];
	}
	int getNumIndex() const
	{
		return numIndex;
	}
	void ShutDown();
	~Model();
};




#endif