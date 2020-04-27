#include "Model.h"



Model::Model()
{
}
Model::Model(const Model & model)
{
}

bool Model::ModelInit(ID3D11Device * dev)
{
	HRESULT hr = S_OK;
	const int numVertice = 8;
	const int numIndex = 36;
	Vertex vertices[numVertice] =
	{
		{ XMFLOAT3(-1.0f,	1.0f,  -1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,	1.0f,  -1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f,	1.0f,   1.0f),		XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,	1.0f,   1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  -1.0f,  -1.0f),		XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,  -1.0f,  -1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f,  -1.0f,   1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  -1.0f,   1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};
	WORD Indices[numIndex] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	D3D11_BUFFER_DESC buf;
	ZeroMemory(&buf, sizeof(buf));
	buf.Usage = D3D11_USAGE_DEFAULT;                // write access access by CPU and GPU
	buf.ByteWidth = sizeof(Vertex) * numVertice;             // size is the VERTEX struct * 3
	buf.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	buf.CPUAccessFlags = 0;    // allow CPU to write in buffer
	hr = dev->CreateBuffer(&buf, NULL, &pVBuffer);       // create the buffer
	if (FAILED(hr))
	{
		return false;
	}
	// copy the vertices into the buffer
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = dev->CreateBuffer(&buf, &InitData, &pVBuffer);
	if (FAILED(hr))
		return false;
	// Set vertex buffer

	// Create and init the index buf.
	buf.Usage = D3D11_USAGE_DEFAULT;
	buf.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buf.ByteWidth = sizeof(WORD) * numIndex;
	buf.CPUAccessFlags = 0;
	InitData.pSysMem = Indices;
	hr = dev->CreateBuffer(&buf, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return false;

	buf.Usage = D3D11_USAGE_DEFAULT;
	buf.ByteWidth = sizeof(ConstantBuffer);
	buf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buf.CPUAccessFlags = 0;
	hr = dev->CreateBuffer(&buf, nullptr, &g_ConstantBuffer);
	if (FAILED(hr))
		return false;

	return true;
}

void Model::RenderModel(ID3D11DeviceContext * devcon)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);// Set index buffer
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// Set primitive topology
}

void Model::ShutDown()
{
	if (pVBuffer)
	{
		pVBuffer->Release();
		pVBuffer = 0;
	}
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = 0;
	}
	if (g_ConstantBuffer)
	{
		g_ConstantBuffer->Release();
		g_ConstantBuffer = 0;
	}
	
}


Model::~Model()
{
}
