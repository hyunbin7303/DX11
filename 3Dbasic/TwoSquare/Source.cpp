/*
FILE	   : TwoSquare.cpp
PROGRAMMER : Kevin Park(Hyunbin Park)
*/

// Window Header
#include <Windows.h>

// Header for DirectX
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "DXGI.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

// Global Variable setting
HWND g_hwnd = NULL;
HINSTANCE hInst;
const int Width = 800;
const int Height = 600;


// Global Variable setting For DirectX
IDXGISwapChain* SwapChain;
ID3D11Device* dev;
ID3D11DeviceContext* devcon;
ID3D11RenderTargetView* renderTarget;


ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* verLayout;

ID3D11DepthStencilView* DepthStencilView;
ID3D11Texture2D* DepthStencilBuffer; // store the depth - stencil Buffer.
ID3D11RasterizerState* wireRasterizerState;






ID3D11Buffer* sqIndexBuf;		// Index Buffer for Square
ID3D11Buffer* sqVertexBuf;		// Vertex Buffer For Square
ID3D11Buffer* cbPerObjectBuffer;

XMMATRIX WVP;
XMMATRIX WorldMatrix;
XMMATRIX ViewMatrix;
XMMATRIX ProjectionMatrix;


XMMATRIX sqWorld1;
XMMATRIX sqWorld2;

XMVECTOR camEye;
XMVECTOR camTarget;
XMVECTOR camUp;

XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;
float rot = 0.01f;

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float r, float b, float g, float a) :position(x, y, z), color(r, b, g, a) {}
	XMFLOAT3 position;
	XMFLOAT4 color;
};
struct cbPerObject
{
	XMMATRIX WVP;
};
cbPerObject cbPerObj;

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
};
/*
DXGI_FORMAT_R32G32B32A32_FLOAT enumerated type for the format, thats 16 bytes, so if we were to add another element to the input layout, the next element would have to put 28 for the 5th parameter, since it would be located 28 bytes from the beginning of the array.
*/
UINT numElements = ARRAYSIZE(layout);
void Update();
void Render();
int MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}
	return (int)msg.wParam;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

bool D3d11Init();
void ReleaseEverything();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Initializing application 
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	// CS_VREDRAW - The window is redrawn if there is a change in the window's height or if the window is moved vertically.
	wcx.lpfnWndProc = WindowProc; // Point to window Procedure
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInst;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = L"MainMenu";
	wcx.lpszClassName = L"Triangle";
	wcx.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(5), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	if (!RegisterClassEx(&wcx))
		MessageBox(NULL, L"WTF, Registering Fail. ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);

	g_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"Triangle",
		L"Kevin- Simple Triangle",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height,
		NULL, NULL, hInst, NULL);

	if (!g_hwnd)
		return -1;


	if (!D3d11Init())
		return -1;

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);

	if (MessageLoop() == -1)
		return -1;


	ReleaseEverything();
	UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}
void Update()
{
	rot += .0005f;
	if (rot > 6.28)
	{
		rot = 0.0f;
	}

	sqWorld1 = XMMatrixIdentity(); // Reset
	XMVECTOR rotateAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotateAxis, rot);
	Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	sqWorld1 = Translation * Rotation;


	sqWorld2 = XMMatrixIdentity();
	Rotation = XMMatrixRotationAxis(rotateAxis, -rot);
	Scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);
	sqWorld2 = Rotation * Scale;
}

void Render()
{
	float Color[4] = { 0.0f, 0.0f, 0.5f, 0.0f };
	devcon->ClearRenderTargetView(renderTarget, Color);
	devcon->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



	WVP = sqWorld1 * ViewMatrix * ProjectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devcon->DrawIndexed(36, 0, 0);

	WVP = sqWorld2 * ViewMatrix * ProjectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devcon->DrawIndexed(36, 0, 0);




	SwapChain->Present(0, 0);

}

bool D3d11Init()
{
	HRESULT hr = S_OK;

	// Describes a display mode
	// Describing the back buffer.
	DXGI_MODE_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(DXGI_MODE_DESC));
	bufDesc.Width = Width;
	bufDesc.Height = Height;
	bufDesc.RefreshRate.Numerator = 60;
	bufDesc.RefreshRate.Denominator = 1;
	bufDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
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

	IDXGIFactory1* DXGIFactory;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);
	// Use the first adapter    
	IDXGIAdapter1* Adapter;

	hr = DXGIFactory->EnumAdapters1(0, &Adapter);

	DXGIFactory->Release();

	// Create swapchain in here.
	hr = D3D11CreateDeviceAndSwapChain(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, NULL, D3D11_SDK_VERSION, &swapchainDesc, &SwapChain, &dev, NULL, &devcon);

	// Create Back Buffer
	ID3D11Texture2D* backBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	hr = dev->CreateRenderTargetView(backBuffer, NULL, &renderTarget);
	backBuffer->Release();
	devcon->OMSetRenderTargets(1, &renderTarget, NULL);


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1; // Structure that specifies multisampling parameters for the texture
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Create the Depth/Stencil View
	dev->CreateTexture2D(&depthStencilDesc, NULL, &DepthStencilBuffer);
	dev->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView);

	// Binding the DepthStencil View
	devcon->OMSetRenderTargets(1, &renderTarget, DepthStencilView);

	// Adding Depth values to the view port.

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "VS", "vs_5_0", dwShaderFlags, 0, &VS_Buffer, &pErrorBlob);
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "PS", "ps_5_0", dwShaderFlags, 0, &PS_Buffer, &pErrorBlob);
	if (FAILED(hr))
		return false;
	hr = dev->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = dev->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	if (FAILED(hr))
		return false;

	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);
	Vertex v[] = {
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f),

	};
	DWORD indice[] = {
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	D3D11_BUFFER_DESC indexBufDesc;
	ZeroMemory(&indexBufDesc, sizeof(indexBufDesc));
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.ByteWidth = sizeof(Vertex) * 12 * 3;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indice;
	dev->CreateBuffer(&indexBufDesc, &indexData, &sqIndexBuf);
	// Set Index Buffer  - Bind an index buffer to the input-assembly stage.
	devcon->IASetIndexBuffer(sqIndexBuf, DXGI_FORMAT_R32_UINT, 0); // A single-component, 32 bit unsigned integer


	D3D11_BUFFER_DESC vertexBufDesc;
	ZeroMemory(&vertexBufDesc, sizeof(vertexBufDesc));
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * 8;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = v; // Pointer to the initialization data.
	hr = dev->CreateBuffer(&vertexBufDesc, &vertexData, &sqVertexBuf);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &sqVertexBuf, &stride, &offset);

	hr = dev->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &verLayout);
	if (FAILED(hr))
		return false;

	devcon->IASetInputLayout(verLayout);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3D11_BUFFER_DESC constBuf;
	ZeroMemory(&constBuf, sizeof(constBuf));
	constBuf.Usage = D3D11_USAGE_DEFAULT;
	constBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuf.ByteWidth = sizeof(cbPerObj);
	constBuf.MiscFlags = 0;
	constBuf.CPUAccessFlags = 0;
	hr = dev->CreateBuffer(&constBuf, NULL, &cbPerObjectBuffer);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	devcon->RSSetViewports(1, &viewport);



	camEye = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	// View matrix setting, which is camera view.
	ViewMatrix = XMMatrixLookAtLH(camEye, camTarget, camUp);
	ProjectionMatrix = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)Width / Height, 1.0f, 1000.0f);

	D3D11_RASTERIZER_DESC RasterDesc;
	ZeroMemory(&RasterDesc, sizeof(RasterDesc));
	RasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	RasterDesc.CullMode = D3D11_CULL_NONE;
	hr = dev->CreateRasterizerState(&RasterDesc, &wireRasterizerState);
	devcon->RSSetState(wireRasterizerState);
	return true;

}

void ReleaseEverything()
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
	if (DepthStencilView)
	{
		DepthStencilView->Release();
		DepthStencilView = 0;
	}
	if (DepthStencilBuffer)
	{
		DepthStencilBuffer->Release();
		DepthStencilBuffer = 0;
	}
	if (cbPerObjectBuffer)
	{
		cbPerObjectBuffer->Release();
		cbPerObjectBuffer = 0;
	}
	if (wireRasterizerState)
	{
		wireRasterizerState->Release();
		wireRasterizerState = 0;
	}
}
