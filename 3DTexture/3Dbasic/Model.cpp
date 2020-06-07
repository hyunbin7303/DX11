#include "Model.h"



Model::Model()
{
}
Model::Model(const Model & model)
{
}

bool Model::ModelInit(ID3D11Device * dev, std::string fileName)
{
	HRESULT hr = S_OK;
	Vertex vertices[numVertice] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),		XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(	1.0f, 1.0f, -1.0f),		XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(	1.0f, 1.0f,  1.0f),		XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f,  1.0f),		XMFLOAT2(1.0f, 0.0f) },
											   					 
		{ XMFLOAT3(-1.0f,-1.0f, -1.0f),		XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),		XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),		XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f,-1.0f,  1.0f),		XMFLOAT2(1.0f, 0.0f) },
											   				
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),		XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),		XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),		XMFLOAT2(1.0f, 0.0f) },
											   				
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),		XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),		XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),		XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),		XMFLOAT2(1.0f, 0.0f) },
											   				
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),		XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),		XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),		XMFLOAT2(1.0f, 0.0f) },
											   				
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),		XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),		XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),		XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),		XMFLOAT2(1.0f, 0.0f) },
	};
	WORD Indices[numIndex] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
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
	buf.ByteWidth = sizeof(XMMATRIX);
	buf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buf.CPUAccessFlags = 0;


	hr = dev->CreateBuffer(&buf, nullptr, &g_constantBuf[cbNeverChange]);
	if (FAILED(hr))
		return false;

	hr = dev->CreateBuffer(&buf, nullptr, &g_constantBuf[cbChangeOnResize]);
	if (FAILED(hr))
		return false;
	hr = dev->CreateBuffer(&buf, nullptr, &g_constantBuf[cbFrame]);
	if (FAILED(hr))
		return false;
	LoadTextureFile(dev, fileName);
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

void Model::Rendering(ID3D11DeviceContext * devcon, XMMATRIX worldMatrix)
{
	RenderModel(devcon);
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(worldMatrix);
	devcon->UpdateSubresource(g_constantBuf[cbFrame], 0, nullptr, &cb, 0, 0);

	// Setting Constant buffer in here.
	devcon->VSSetShader(pVS, nullptr, 0);
	devcon->VSSetConstantBuffers(0, 1, &g_constantBuf[cbNeverChange]);
	devcon->VSSetConstantBuffers(1, 1, &g_constantBuf[cbChangeOnResize]);
	devcon->VSSetConstantBuffers(2, 1, &g_constantBuf[cbFrame]);


	devcon->PSSetShader(pPS, nullptr, 0);
	devcon->PSSetConstantBuffers(2, 1, &g_constantBuf[cbFrame]);
	devcon->PSSetShaderResources(0, 1, &g_pTextureRV);
	devcon->PSSetSamplers(0, 1, &g_pSamplerLinear);
	devcon->DrawIndexed(numIndex, 0, 0);
	

}

void Model::SetShader(ID3D11VertexShader * pvs, ID3D11PixelShader * pps)
{
	pVS = pvs;
	pPS = pps;
}

std::string getFileExtension(const std::string& str)
{
	size_t i = str.rfind('.', str.length());
	if (i != std::string::npos)
	{
		return (str.substr(i + 1, str.length() - i));
	}
	return "";
}

bool Model::LoadTextureFile(ID3D11Device * dev, std::string fileName)
{
	HRESULT hr = S_OK;

	// Here... I need to check whether file extension is WIC or DDS.
	std::string ExtensionValue = getFileExtension(fileName);
	std::wstring wideStr = std::wstring(fileName.begin(), fileName.end());
	const wchar_t* widecstr = wideStr.c_str();
	if (ExtensionValue == "dds")
	{
		hr = CreateDDSTextureFromFile(dev, widecstr, nullptr, &g_pTextureRV);
	}
	else
	{
		hr = CreateWICTextureFromFile(dev, widecstr, nullptr, &g_pTextureRV);
	}
	if (FAILED(hr))
		return false;
	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = dev->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
		return false;


	return false;
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

	if (g_pTextureRV)
	{
		g_pTextureRV->Release();
		g_pTextureRV = 0;
	}
	if (g_pSamplerLinear)
	{
		g_pSamplerLinear->Release();
		g_pSamplerLinear = 0;
	}
	if (g_constantBuf[0])
	{
		g_constantBuf[0]->Release();
		g_constantBuf[0] = 0;
	}
	if (g_constantBuf[1])
	{
		g_constantBuf[1]->Release();
		g_constantBuf[1] = 0;
	}
	if (g_constantBuf[2])
	{
		g_constantBuf[2]->Release();
		g_constantBuf[2] = 0;
	}
}


Model::~Model()
{
}
