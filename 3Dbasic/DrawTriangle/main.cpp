/*
FILE	   : SimpleTriangle.cpp
PROGRAMMER : Kevin Park(Hyunbin Park)
DESCRIPTION : This application is used for displaying a colorful triangle on the screen.
*/

#include "DX.h"


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


ID3D11Buffer* drawVertexBuf;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* verLayout;



struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float r, float b, float g, float a) :position(x, y, z), color(r, b, g, a) {}
	XMFLOAT3 position;
	XMFLOAT4 color;
};

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
}

void Render()
{
	float Color[4] = { 0.0f, 0.5f, 0.5f, 0.0f };
	devcon->ClearRenderTargetView(renderTarget, Color);
	devcon->Draw(3, 0);
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

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "VS", "vs_4_0", dwShaderFlags, 0, &VS_Buffer, &pErrorBlob);
	hr = D3DCompileFromFile(L"effect.fx", nullptr, nullptr, "PS", "ps_4_0", dwShaderFlags, 0, &PS_Buffer, &pErrorBlob);
	if (FAILED(hr))
		return false;
	hr = dev->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = dev->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	if (FAILED(hr))
		return false;

	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);
	Vertex v[] = {
		Vertex{ 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		Vertex{ 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
		Vertex{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
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
	viewport.Width = Width;
	viewport.Height = Height;

	devcon->RSSetViewports(1, &viewport);
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
