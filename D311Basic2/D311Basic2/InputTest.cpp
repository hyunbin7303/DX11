/*
FILE	   : InputTest.cpp
PROGRAMMER : Kevin Park(Hyunbin Park)
Resource From : https://www.braynzarsoft.net/viewtutorial/q16390-18-direct-input
*/


// Window Header

// Header for DirectX
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

//
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>
#include <dinput.h>
#include "WICTextureLoader.h"
using namespace DirectX;

// Global Variable setting
HWND g_hwnd = NULL;
HINSTANCE hInst;
const int Width = 500;
const int Height = 500;

// timer setting 
double countsPerSecond = 0.0;
__int64 CounterStart = 0;
int frameCount = 0;
int fps = 0;

__int64 frameTimeOld = 0;
double frameTime;

void StartTimer();
double GetTime();
double GetFrameTime();

// Global Variable setting For DirectX
IDXGISwapChain* SwapChain;
ID3D11Device*	dev;
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
ID3D11ShaderResourceView* sqShaderResourceview;
ID3D11SamplerState* SqSamplestate;
ID3D11Texture2D* backBuffer11;

// Used for lighting
ID3D11PixelShader* D2D_PS;
ID3D10Blob* D2D_PS_Buffer;



ID3D11Buffer* sqIndexBuf;		// Index Buffer for Square
ID3D11Buffer* sqVertexBuf;		// Vertex Buffer For Square
ID3D11Buffer* cbPerObjectBuffer;// Object Buffer
ID3D11Buffer* cbPerFrameBuffer; // Frame Buffer


								// Affect blend state.
ID3D11BlendState* Transparency;
ID3D11RasterizerState* CCWcullMode; // Counter clockwise culling
ID3D11RasterizerState* CWcullMode; // Clockwise culling.


								   //DirectX 2D
ID3D10Device1 *d3d101Device;
IDXGIKeyedMutex *keyedMutex11;
IDXGIKeyedMutex *keyedMutex10;
ID2D1RenderTarget *D2DRenderTarget;
ID2D1SolidColorBrush *Brush;
ID3D11Texture2D *BackBuffer11;
ID3D11Texture2D *sharedTex11;
ID3D11Buffer *d2dVertBuffer;
ID3D11Buffer *d2dIndexBuffer;
ID3D11ShaderResourceView *d2dTexture;
IDWriteFactory *DWriteFactory;
IDWriteTextFormat *TextFormat;


IDirectInputDevice8 * DIKeyboard;
IDirectInputDevice8 * DIMouse;
DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;	
bool InitDirectInput(HINSTANCE hInstance);
void DetectInput(double time);

float scaleX = 1.0f;
float scaleY = 1.0f;

std::wstring printText;
bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter);
void InitD2DScreenTexture();
void RenderText(std::wstring text, int inInt);

LPCTSTR WndClassName = L"firstwindow";
XMMATRIX WVP;
XMMATRIX sqWorld1; // World
XMMATRIX sqWorld2; // World
XMMATRIX ViewMatrix;
XMMATRIX ProjectionMatrix;

XMVECTOR camEye;
XMVECTOR camTarget;
XMVECTOR camUp;

XMMATRIX RotationX;
XMMATRIX RotationY;
XMMATRIX RotationZ;

XMMATRIX Scale;
XMMATRIX Translation;
float rot = 0.01f;
float rotX = 0.0f;
float rotZ = 0.0f;
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	float pad1;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};
struct cbPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;
};
cbPerObject cbPerObj;
Light light;
struct cbPerFrame
{
	Light light;
};
cbPerFrame constbufferPerFrame;

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		:pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};
D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
DXGI_FORMAT_R32G32B32A32_FLOAT enumerated type for the format, thats 16 bytes, so if we were to add another element to the input layout, the next element would have to put 28 for the 5th parameter, since it would be located 28 bytes from the beginning of the array.
*/
UINT numElements = ARRAYSIZE(layout);
void Update(double time);
void Render();
int MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		BOOL PeekMessageL(
			LPMSG lpMsg,
			HWND hWnd,
			UINT wMsgFilterMin,
			UINT wMsgFilterMax,
			UINT wRemoveMsg
		);
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
			frameCount++;
			if (GetTime() > 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				StartTimer();
			}
			frameTime = GetFrameTime();
			DetectInput(frameTime);
			Update(frameTime);
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

void ReleaseEverything();
bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = g_hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	IDXGIFactory1 *DXGIFactory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);
	// Use the first adapter    
	IDXGIAdapter1 *Adapter;
	hr = DXGIFactory->EnumAdapters1(0, &Adapter);
	DXGIFactory->Release();

	hr = D3D11CreateDeviceAndSwapChain(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &dev, NULL, &devcon);

	//Initialize Direct2D, Direct3D 10.1, DirectWrite
	InitD2D_D3D101_DWrite(Adapter);
	//Release the Adapter interface
	Adapter->Release();
	//Create our BackBuffer and Render Target
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer11);
	hr = dev->CreateRenderTargetView(BackBuffer11, NULL, &renderTarget);

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	dev->CreateTexture2D(&depthStencilDesc, NULL, &DepthStencilBuffer);
	dev->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView);


	InitD2DScreenTexture();
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "VS", "vs_4_0", dwShaderFlags, 0, &VS_Buffer, &pErrorBlob);
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "PS", "ps_4_0", dwShaderFlags, 0, &PS_Buffer, &pErrorBlob);
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "D2D_PS", "ps_4_0", dwShaderFlags, 0, &D2D_PS_Buffer, &pErrorBlob);
	if (FAILED(hr))
		return false;
	hr = dev->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = dev->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	hr = dev->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	if (FAILED(hr))
		return false;
	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);

	light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	Vertex v[] =
	{
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	DWORD indice[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};
	D3D11_BUFFER_DESC indexBufDesc;
	ZeroMemory(&indexBufDesc, sizeof(indexBufDesc));
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indice;
	dev->CreateBuffer(&indexBufDesc, &indexData, &sqIndexBuf);
	// Set Index Buffer  - Bind an index buffer to the input-assembly stage.
	//	devcon->IASetIndexBuffer(sqIndexBuf, DXGI_FORMAT_R32_UINT, 0); // A single-component, 32 bit unsigned integer

	D3D11_BUFFER_DESC vertexBufDesc;
	ZeroMemory(&vertexBufDesc, sizeof(vertexBufDesc));
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * 24;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = v; // Pointer to the initialization data.
	hr = dev->CreateBuffer(&vertexBufDesc, &vertexData, &sqVertexBuf);
	if (FAILED(hr))
	{
		return false;
	}
	hr = dev->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &verLayout);
	if (FAILED(hr))
		return false;
	devcon->IASetInputLayout(verLayout);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	devcon->RSSetViewports(1, &viewport);


	// Const buffer for per object
	D3D11_BUFFER_DESC constBuf;
	ZeroMemory(&constBuf, sizeof(D3D11_BUFFER_DESC));
	constBuf.Usage = D3D11_USAGE_DEFAULT;
	constBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuf.ByteWidth = sizeof(cbPerObject);
	constBuf.MiscFlags = 0;
	constBuf.CPUAccessFlags = 0;
	hr = dev->CreateBuffer(&constBuf, NULL, &cbPerObjectBuffer);

	// Const buffer for per frame
	ZeroMemory(&constBuf, sizeof(D3D11_BUFFER_DESC));
	constBuf.Usage = D3D11_USAGE_DEFAULT;
	constBuf.ByteWidth = sizeof(cbPerFrame);
	constBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuf.MiscFlags = 0;
	constBuf.CPUAccessFlags = 0;
	hr = dev->CreateBuffer(&constBuf, NULL, &cbPerFrameBuffer);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;


	hr = CreateWICTextureFromFile(dev, L"braynzar.jpg", nullptr, &sqShaderResourceview);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"WTF, Texturing Fail. ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = dev->CreateSamplerState(&samplerDesc, &SqSamplestate);
	if (FAILED(hr))
	{
		return false;
	}
	dev->CreateBlendState(&blendDesc, &Transparency);

	D3D11_RASTERIZER_DESC CullRasterDesc;
	ZeroMemory(&CullRasterDesc, sizeof(CullRasterDesc));
	CullRasterDesc.FillMode = D3D11_FILL_SOLID;
	CullRasterDesc.CullMode = D3D11_CULL_BACK;
	CullRasterDesc.FrontCounterClockwise = true;
	hr = dev->CreateRasterizerState(&CullRasterDesc, &CCWcullMode);
	CullRasterDesc.FrontCounterClockwise = false;
	hr = dev->CreateRasterizerState(&CullRasterDesc, &CWcullMode);





	camEye = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	// View matrix setting, which is camera view.
	ViewMatrix = XMMatrixLookAtLH(camEye, camTarget, camUp);
	ProjectionMatrix = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)Width / Height, 1.0f, 1000.0f);
	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
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
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcx.lpszMenuName = L"MainMenu";
	wcx.lpszClassName = L"Triangle";
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
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
	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);
	if (!InitializeDirect3d11App(hInstance))
	{
		return -1;
	}




	if (!InitDirectInput(hInstance))
		return -1;
	if (MessageLoop() == -1)
		return -1;


	ReleaseEverything();
	UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}
void StartTimer()
{
	LARGE_INTEGER freCount;
	QueryPerformanceFrequency(&freCount);
	countsPerSecond = double(freCount.QuadPart);
	QueryPerformanceCounter(&freCount);
	CounterStart = freCount.QuadPart;
}
double GetTime()
{

	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);
	return double(curTime.QuadPart - CounterStart) / countsPerSecond;
}
double GetFrameTime()
{
	LARGE_INTEGER curTime;
	__int64 tickCount;
	QueryPerformanceCounter(&curTime);
	tickCount = curTime.QuadPart - frameTimeOld;
	frameTimeOld = curTime.QuadPart;
	if (tickCount < 0)
	{
		tickCount = 0;
	}
	return float(tickCount) / countsPerSecond;
}
bool InitDirectInput(HINSTANCE hInstance)
{
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Direct Input Create fail ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	// GUID - Global Unique Identifier 
	hr = DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	hr = DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL);

	// c_dfDIKeyboard - Standard Keyboard structure. An array of 256 characters, one for each key.
	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(g_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	//hr = DIMouse->SetCooperativeLevel(g_hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	return true;
}
void DetectInput(double time)
{
	DIMOUSESTATE mouseCurrState;
	BYTE keyboardState[256];
	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(g_hwnd, WM_DESTROY, 0, 0);
	if (keyboardState[DIK_LEFT] & 0x80)
	{
		rotZ -= 1.0f * time;
	}
	if (keyboardState[DIK_RIGHT] & 0x80)
	{
		rotZ += 1.0f * time;
	}
	if (keyboardState[DIK_UP] & 0x80)
	{
		rotX += 1.0f * time;
	}
	if (keyboardState[DIK_DOWN] & 0x80)
	{
		rotX -= 1.0f * time;
	}
	/*if (mouseCurrState.lX != mouseLastState.lX)
	{
		scaleX -= (mouseCurrState.lX * 0.001f);
	}
	if (mouseCurrState.lY != mouseLastState.lY)
	{
		scaleY -= (mouseCurrState.lY * 0.001f);
	}*/
	if (rotX > 6.28f)
		rotX -= 6.28f;
	else if (rotX < 0)
		rotX = 6.28f + rotZ;
	
	
	if (rotZ > 6.28f)
	{
		rotZ -= 6.28f;
	}
	else if (rotZ < 0.0f)
	{
		rotZ = 6.28f + rotZ;
	}
	mouseLastState = mouseCurrState;
	return;
}
bool InitD2D_D3D101_DWrite(IDXGIAdapter1 * Adapter)
{
	HRESULT hr = S_OK;
	hr = D3D10CreateDevice1(Adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &d3d101Device);
	if (FAILED(hr))
		return false;
	D3D11_TEXTURE2D_DESC sharedTexDesc;
	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));
	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.Height = Height;
	sharedTexDesc.Width = Width;
	sharedTexDesc.ArraySize = 1;
	sharedTexDesc.MipLevels = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	hr = dev->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);
	hr = sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);

	IDXGIResource* sharedResouce10;
	HANDLE sharedHandle10;
	hr = sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResouce10);
	hr = sharedResouce10->GetSharedHandle(&sharedHandle10);
	sharedResouce10->Release();

	IDXGISurface1 *sharedSurface10;
	hr = d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)&sharedSurface10);
	hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);

	//create D2D Factory
	ID2D1Factory *D2DFactory;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"D2DFactory Creating Fail ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperty;
	ZeroMemory(&renderTargetProperty, sizeof(renderTargetProperty));
	renderTargetProperty.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperty.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperty, &D2DRenderTarget);
	sharedSurface10->Release();
	D2DFactory->Release();
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Creating DXGI Surface Fail. ", L"ERROR", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f), &Brush);
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DWriteFactory));
	hr = DWriteFactory->CreateTextFormat(
		L"Script",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&TextFormat
	);
	hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	return true;
}
void InitD2DScreenTexture()
{
	HRESULT hr = S_OK;
	//Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	dev->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);

	//Create A shader resource view from the texture D2D will render to,
	//So we can use it to texture a square which overlays our scene
	dev->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
}
void RenderText(std::wstring text, int inInt)
{
	//Release the D3D 11 Device
	keyedMutex11->ReleaseSync(0);

	//Use D3D10.1 device
	keyedMutex10->AcquireSync(0, 5);

	//Draw D2D content        
	D2DRenderTarget->BeginDraw();

	//Clear D2D Background
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	//Create our string
	std::wostringstream printString;
	printString << text << inInt;
	printText = printString.str();

	//Set the Font Color
	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	//Set the brush color D2D will use to draw with
	Brush->SetColor(FontColor);

	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, Width, Height);

	//Draw the Text
	D2DRenderTarget->DrawText(
		printText.c_str(),
		wcslen(printText.c_str()),
		TextFormat,
		layoutRect,
		Brush
	);

	D2DRenderTarget->EndDraw();

	//Release the D3D10.1 Device
	keyedMutex10->ReleaseSync(1);

	//Use the D3D11 Device
	keyedMutex11->AcquireSync(1, 5);

	//Use the shader resource representing the direct2d render target
	//to texture a square which is rendered in screen space so it
	//overlays on top of our entire scene. We use alpha blending so
	//that the entire background of the D2D render target is "invisible",
	//And only the stuff we draw with D2D will be visible (the text)

	//Set the blend state for D2D render target texture objects
	devcon->OMSetBlendState(Transparency, NULL, 0xffffffff);
	devcon->PSSetShader(D2D_PS, 0, 0);

	//Set the d2d Index buffer
	devcon->IASetIndexBuffer(d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Set the d2d vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &d2dVertBuffer, &stride, &offset);

	WVP = XMMatrixIdentity();
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devcon->PSSetShaderResources(0, 1, &d2dTexture);
	devcon->PSSetSamplers(0, 1, &SqSamplestate);

	devcon->RSSetState(CWcullMode);
	//Draw the second cube
	devcon->DrawIndexed(6, 0, 0);
}
void Update(double time)
{
	// update constant vuale every frame.
	rot += 1.0f * static_cast<float>(time);
	if (rot > 6.28f)
	{
		rot = 0.0f;
	}

	//Update the Lights Position
	XMVECTOR lightVec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	lightVec = XMVector3TransformCoord(lightVec, sqWorld1);

	sqWorld1 = XMMatrixIdentity(); // Reset
	XMVECTOR rotateX_Axis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rotateY_Axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rotateZ_Axis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	RotationX = XMMatrixRotationAxis(rotateX_Axis, rotX);
//	RotationY = XMMatrixRotationAxis(rotateY_Axis, rot);
	RotationZ = XMMatrixRotationAxis(rotateZ_Axis, rotZ);
	Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	sqWorld1 = Translation * RotationX * /*RotationY **/ RotationZ;


	sqWorld2 = XMMatrixIdentity();
	RotationY = XMMatrixRotationAxis(rotateY_Axis, -rot);
	Scale = XMMatrixScaling(scaleX, scaleY, 1.3f);
	sqWorld2 = RotationY * Scale;
}

void Render()
{
	float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	devcon->ClearRenderTargetView(renderTarget, Color);
	devcon->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	constbufferPerFrame.light = light;
	devcon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbufferPerFrame, 0, 0);
	devcon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);
	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);

	devcon->OMSetRenderTargets(1, &renderTarget, DepthStencilView);
	devcon->OMSetBlendState(0, 0, 0xffffffff);
	//Set the cubes vertex buffer
	devcon->IASetIndexBuffer(sqIndexBuf, DXGI_FORMAT_R32_UINT, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &sqVertexBuf, &stride, &offset);







	WVP = sqWorld1 * ViewMatrix * ProjectionMatrix;
	cbPerObj.World = XMMatrixTranspose(sqWorld1);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devcon->PSSetShaderResources(0, 1, &sqShaderResourceview);
	devcon->PSSetSamplers(0, 1, &SqSamplestate);

	devcon->RSSetState(CWcullMode);
	devcon->DrawIndexed(36, 0, 0);







	WVP = sqWorld2 * ViewMatrix * ProjectionMatrix;
	cbPerObj.World = XMMatrixTranspose(sqWorld2);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devcon->PSSetShaderResources(0, 1, &sqShaderResourceview);
	devcon->PSSetSamplers(0, 1, &SqSamplestate);

	devcon->RSSetState(CWcullMode);
	devcon->DrawIndexed(36, 0, 0);

	RenderText(L"FPS: ", fps);
	SwapChain->Present(0, 0);

}
void ReleaseEverything()
{
	/*if (SwapChain)
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
	if (backBuffer11)
	{
		backBuffer11->Release();
		backBuffer11 = 0;
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
	if (Transparency)
	{
		Transparency->Release();
		Transparency = 0;
	}
	if (CCWcullMode)
	{
		CCWcullMode->Release();
		CCWcullMode = 0;
	}
	if (CWcullMode)
	{
		CWcullMode->Release();
		CWcullMode = 0;
	}

	if (d3d101Device)
	{
		d3d101Device->Release();
		d3d101Device = 0;
	}
	if (keyedMutex11)
	{
		keyedMutex11->Release();
		keyedMutex11 = 0;
	}
	if (keyedMutex10)
	{
		keyedMutex10->Release();
		keyedMutex10 = 0;
	}
	D2DRenderTarget->Release();
	Brush->Release();
	BackBuffer11->Release();
	sharedTex11->Release();
	DWriteFactory->Release();
	TextFormat->Release();
	d2dTexture->Release();
	cbPerFrameBuffer->Release();
	if (DIKeyboard)
	{
		DIKeyboard->Unacquire();
		DIKeyboard->Release();
		DIKeyboard = 0;
	}
	if (DIMouse)
	{
		DIMouse->Unacquire();
		DIMouse->Release();
		DIMouse = 0;
	}
	if (DirectInput)
	{
		DirectInput->Release();
		DirectInput = 0;
	}*/

	SwapChain->Release();
	dev->Release();
	devcon->Release();
	renderTarget->Release();
	sqIndexBuf->Release();
	sqVertexBuf->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	verLayout->Release();
	DepthStencilView->Release();
	DepthStencilBuffer->Release();
	cbPerObjectBuffer->Release();
	Transparency->Release();
	CCWcullMode->Release();
	CWcullMode->Release();

	d3d101Device->Release();
	keyedMutex11->Release();
	keyedMutex10->Release();
	D2DRenderTarget->Release();
	Brush->Release();
	BackBuffer11->Release();
	sharedTex11->Release();
	DWriteFactory->Release();
	TextFormat->Release();
	d2dTexture->Release();

	cbPerFrameBuffer->Release();

	///////////////**************new**************////////////////////
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DirectInput->Release();
}
