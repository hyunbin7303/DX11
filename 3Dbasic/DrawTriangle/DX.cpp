#include "DX.h"
D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
};
// Global Variable setting For DirectX
UINT numElements = ARRAYSIZE(layout);
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float r, float b, float g, float a) :position(x, y, z), color(r, b, g, a) {}
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

bool DX::D3d11Init(int width, int height, HWND g_hwnd)
{
	HRESULT hr = S_OK;
	DXGI_MODE_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(DXGI_MODE_DESC));
	bufDesc.Width = width;
	bufDesc.Height = height;
	bufDesc.RefreshRate.Numerator = 60;
	bufDesc.RefreshRate.Denominator = 1;
	bufDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchainDesc.BufferDesc = bufDesc;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 1;
	swapchainDesc.OutputWindow = g_hwnd;
	swapchainDesc.Windowed = TRUE;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Setting up is done.

	// Create swapchain in here.
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapchainDesc, &SwapChain, &dev, NULL, &devcon);


	// Create Back Buffer
	ID3D11Texture2D* backBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	hr = dev->CreateRenderTargetView(backBuffer, NULL, &renderTarget);
	backBuffer->Release();
	devcon->OMSetRenderTargets(1, &renderTarget, NULL);
	if (FAILED(hr))
		return false;


	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "VS", "vs_4_0", dwShaderFlags, 0, &VS_Buffer, &pErrorBlob);
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "PS", "ps_4_0", dwShaderFlags, 0, &PS_Buffer, &pErrorBlob);
	if (FAILED(hr))
	{
		MessageBox(0, L"Files not exist.",
			L"Error", MB_OK);
		return false;
	}
	hr = dev->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = dev->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	if (FAILED(hr))
		return false;

	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);

	Vertex v[] = {
		Vertex{ 0.0f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		Vertex{ 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
		Vertex{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
	};

	D3D11_BUFFER_DESC verBufDesc;
	ZeroMemory(&verBufDesc, sizeof(verBufDesc));
	verBufDesc.Usage = D3D11_USAGE_DEFAULT;
	verBufDesc.ByteWidth = sizeof(Vertex) * 3;
	verBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	verBufDesc.CPUAccessFlags = 0;
	verBufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexData.pSysMem = v; // Pointer to the Initialization Data.
	hr = dev->CreateBuffer(&verBufDesc, &vertexData, &drawVertexBuf);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &drawVertexBuf, &stride, &offset);


	hr = dev->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &verLayout);
	if (FAILED(hr))
		return false;

	devcon->IASetInputLayout(verLayout);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	devcon->RSSetViewports(1, &viewport);
	return true;

}

void DX::ReleaseEverything()
{
	if (SwapChain)
	{
		SwapChain->Release();
		SwapChain = 0;
	}
	if (dev)
	{
		dev->Release();
		dev = 0;
	}
	if (devcon)
	{
		devcon->Release();
		devcon = 0;
	}
	if (renderTarget)
	{
		renderTarget->Release();
		renderTarget = 0;
	}

	if (drawVertexBuf)
	{
		drawVertexBuf->Release();
		drawVertexBuf = 0;
	}
	if (VS)
	{
		VS->Release();
		VS = 0;
	}
	if (PS)
	{
		PS->Release();
		PS = 0;
	}
	if (VS_Buffer)
	{
		VS_Buffer->Release();
		VS_Buffer = 0;
	}
	if (PS_Buffer)
	{
		PS_Buffer->Release();
		PS_Buffer = 0;
	}
	if (verLayout)
	{
		verLayout->Release();
		verLayout = 0;
	}
}
void DX::Render()
{
	float Color[4] = { 0.0f, 0.5f, 0.5f, 0.0f };
	devcon->ClearRenderTargetView(renderTarget, Color);
	devcon->Draw(3, 0);
	SwapChain->Present(0, 0);
}
