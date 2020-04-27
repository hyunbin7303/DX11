#ifndef MODEL_H_
#define MODEL_H_
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Model
{
private:
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};
	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};
	ID3D11Buffer			*pVBuffer = nullptr;
	ID3D11Buffer*           g_pIndexBuffer = nullptr;
	ID3D11Buffer*		    g_ConstantBuffer = nullptr;


	
public:
	Model();
	Model(const Model& model);

	bool ModelInit(ID3D11Device* dev);
	void RenderModel(ID3D11DeviceContext* devcon);
	ID3D11Buffer* ReturnConstantBuffer()
	{
		return g_ConstantBuffer;
	}
	void ShutDown();
	~Model();
};




#endif