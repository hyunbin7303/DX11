#include "DX.h"



DX::DX()
{
}


DX::~DX()
{
}

int DX::InitD3D(HWND hWnd)
{

	m_hwnd = hWnd;
	HRESULT hr = S_OK;
	RECT clientRect;
	GetClientRect(m_hwnd, &clientRect);
	// Compute the exact client dimensions. This will be used
	// to initialize the render targets for our swap chain.
	clientWidth = clientRect.right - clientRect.left;
	clientHeight = clientRect.bottom - clientRect.top;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};


	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &dev, &g_featureLevel, &devcon);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &dev, &g_featureLevel, &devcon);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = dev->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;



	DXGI_MODE_DESC dxgiBuf;
	ZeroMemory(&dxgiBuf, sizeof(DXGI_MODE_DESC));
	dxgiBuf.Width = clientWidth;
	dxgiBuf.Height = clientHeight;
	dxgiBuf.RefreshRate.Numerator = 60;
	dxgiBuf.RefreshRate.Denominator = 1;
	dxgiBuf.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiBuf.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiBuf.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;



	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	// fill the swap chain description struct
	scd.BufferDesc = dxgiBuf;
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.SampleDesc.Quality = 0;                             // multisample quality level
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//scd.BufferDesc.RefreshRate = QueryRefreshRate(clientWidth, clientHeight, 0);
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	D3D_FEATURE_LEVEL featureLevel;
	// create a device, device context and swap chain using the information in the scd struct
	 hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		&featureLevel,
		&devcon);


	//One or more arguments are invalid
	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
			D3D11_SDK_VERSION, &scd, &swapchain, &dev, &featureLevel,
			&devcon);
	}
	if (FAILED(hr))
	{
		return ERROR;
	}


	// Create a render target view.
	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer = nullptr;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{

		return ERROR;
	}

	// use the back buffer address to create the render target
	hr = dev->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		return ERROR;
	}
	
	// Depth Stencil Buffer Description.
	D3D11_TEXTURE2D_DESC dtDesc;
	ZeroMemory(&dtDesc, sizeof(D3D11_TEXTURE2D_DESC));
	dtDesc.ArraySize = 1;
	dtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtDesc.CPUAccessFlags = 0; // No CPU access required.
	dtDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtDesc.Width = clientWidth;		// Width and height are same with swapchain's back buffer.
	dtDesc.Height = clientHeight;
	dtDesc.MipLevels = 1;
	dtDesc.SampleDesc.Count = 1;
	dtDesc.SampleDesc.Quality = 0;
	dtDesc.Usage = D3D11_USAGE_DEFAULT;
	dtDesc.MiscFlags = 0;


	// 1st para : A pointer to a D3D11_TEXTURE2D_DESC structure that describes a 2D texture resource.
	// 3rd para : A pointer to a buffer that receives a pointer to a ID3D11Texture2D interface for the created texture.
	hr = dev->CreateTexture2D(&dtDesc, nullptr, &g_DepthStencilBuffer);
	if (FAILED(hr))
	{
		return ERROR;
	}


	// After created the depth/stencil buffer resource, must create a
	// ID3D11DepthStencilView before we can use this depth buffer for rendering
	
	
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = dtDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = dev->CreateDepthStencilView(g_DepthStencilBuffer, &descDSV, &g_DepthStencilView);
	if (FAILED(hr))
		return ERROR;
	// Done with initialization the render buffers.
	devcon->OMSetRenderTargets(1, &g_pRenderTargetView, g_DepthStencilView);
	
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(clientWidth);
	viewport.Height = static_cast<float>(clientHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	devcon->RSSetViewports(1, &viewport); 
	// Bind an array of viewports to the rasterizer stage of the pipeline.
	if (!InitShader()) //Vertex shader / Pixel shader
		return ERROR;
	if (!InitGraphics()) 
		return ERROR;



	// Define Blend Description
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	dev->CreateBlendState(&blendDesc, &Transparency);


	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(cmdesc));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = dev->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = dev->CreateRasterizerState(&cmdesc, &CWcullMode);


	//Camera Setting
	Cube1_world = XMMatrixIdentity();
	for (int i = 0; i < 10; i++)
	{
		ManyCubes_world[i] = XMMatrixIdentity();
	}

	
	m_camera = new Camera;
	g_ViewMatrix = m_camera->RenderCamera();
	g_constantBuf[cbNeverChange] = m_model->GetpcBNeverChanges();
	g_constantBuf[cbChangeOnResize] = m_model->GetpcBChangesOnResize();

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_ViewMatrix);
	devcon->UpdateSubresource(g_constantBuf[cbNeverChange], 0, nullptr, &cbNeverChanges, 0, 0);
	// Initialize the projection matrix
	g_ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, clientWidth / (FLOAT)clientHeight, 0.01f, 100.0f);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_ProjectionMatrix);
	devcon->UpdateSubresource(g_constantBuf[cbChangeOnResize], 0, nullptr, &cbChangesOnResize, 0, 0);
	return 0;
}

void DX::RenderFrame(void)
{
	ClearScreen();
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	//Set the default blend state (no blending) for opaque objects
	devcon->OMSetBlendState(0, 0, 0xffffffff);
	devcon->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	XMVECTOR cubePos = XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, Cube1_world);

	float distanceX = XMVectorGetX(cubePos) - XMVectorGetX(m_camera->getEye());
	float distanceY = XMVectorGetY(cubePos) - XMVectorGetY(m_camera->getEye());
	float distanceZ = XMVectorGetZ(cubePos) - XMVectorGetZ(m_camera->getEye());
	// get distance for the first cub ( in the center)
	float cube1Dist = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;

	cubePos = XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, ManyCubes_world[0]);
	distanceX = XMVectorGetX(cubePos) - XMVectorGetX(m_camera->getEye());
	distanceY = XMVectorGetY(cubePos) - XMVectorGetY(m_camera->getEye());
	distanceZ = XMVectorGetZ(cubePos) - XMVectorGetZ(m_camera->getEye());
	// get distance for the rotating cub
	float cube2Dist = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;
	

	// if the rotating cube is further from the camera, then switch.
	if (cube1Dist < cube2Dist)
	{
		XMMATRIX tempMatrix = Cube1_world;
		Cube1_world = ManyCubes_world[0];
		ManyCubes_world[0] = tempMatrix;
	}
	else
	{

	}
	g_ViewMatrix = m_camera->RenderCamera();
	m_model->Rendering(devcon, Cube1_world);
	m_model->Rendering(devcon, ManyCubes_world[0]);
	swapchain->Present(0, 0);	
}



void DX::Update(float time)
{
	// Update our time
	t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}
	Cube1_world = XMMatrixIdentity();
	XMMATRIX mSpin = XMMatrixRotationZ(-t);
	XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);
	Cube1_world = mSpin * mOrbit;


	XMMATRIX m2Trans = XMMatrixTranslation(-5.0f, 0.0f, 0.0f);
	XMMATRIX mScale = XMMatrixScaling(0.6f, 0.6f, 0.6f);
	
	ManyCubes_world[0] = XMMatrixIdentity();
	ManyCubes_world[0] = m2Trans * mOrbit * mScale;

}

void DX::CleanD3D(void)
{

	SafeRelease(dev);
	SafeRelease(devcon);
	SafeRelease(g_pRenderTargetView);
	SafeRelease(pLayout);
	SafeRelease(pVS);
	SafeRelease(pPS);
	SafeRelease(g_RasterizerState);
	SafeRelease(g_constantBuf[0]);
	SafeRelease(g_constantBuf[1]);
	SafeRelease(g_constantBuf[2]);
	SafeRelease(g_DepthStencilBuffer);
	SafeRelease(g_DepthStencilView);
	SafeRelease(g_DepthStencilState);

	SafeRelease(Transparency);
	SafeRelease(CCWcullMode);
	SafeRelease(CWcullMode);

	if (m_model)
	{
		m_model->ShutDown();
		delete m_model;
		m_model = 0;
	}
	if (m_model2)
	{
		m_model2->ShutDown();
		delete m_model2;
		m_model = 0;
	}
	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}
}

bool DX::InitGraphics(void)
{
	bool check;
	m_model = new Model;	// First Model
	if (!m_model)
		return false;
	check = m_model->ModelInit(dev, "braynzar.jpg");
	if (!check)
		return false;
	m_model->SetShader(pVS, pPS);

	m_model2 = new Model; // Second Model 
	if (!m_model2)
		return false;
	check = m_model2->ModelInit(dev, "seafloor.dds");
	if (!check)
		return false;
	m_model2->SetShader(pVS, pPS);
	//g_constantBuf[cbNeverChange] = m_model2->GetpcBNeverChanges();
	//g_constantBuf[cbChangeOnResize] = m_model2->GetpcBChangesOnResize();
	//g_constantBuf[cbFrame] = m_model2->GetChangesEveryFrame();

	return true;
}
bool DX::InitShader(void)
{
	HRESULT hr = S_OK;
	ID3D10Blob *VS, *PS;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	//std::string version = getVersion(); I got version 5.0
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(L"Shaders/Texture.fx", nullptr, nullptr, "VS", getVersion().c_str(), dwShaderFlags, 0, &VS, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			SafeRelease(VS);
			pErrorBlob->Release();
		}
		return false;
	}
	hr = dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	if (FAILED(hr))
	{
		SafeRelease(VS);
		return false;
	}

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElement = ARRAYSIZE(ied);
	hr = dev->CreateInputLayout(ied, numElement, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	SafeRelease(VS);
	if (FAILED(hr))
	{
		return false;
	}

	// Set the input layout
	devcon->IASetInputLayout(pLayout);
	hr = D3DCompileFromFile(L"Shaders/Texture.fx", nullptr, nullptr, "PS", "ps_4_0", dwShaderFlags, 0, &PS, &pErrorBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Checked the PS.", L"Error", MB_OK);
		return false;
	}

	hr = dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	PS->Release();
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled. Failed to create Pixel Shader.", L"Error", MB_OK);
		return false;
	}
	return true;
}

std::string DX::getVersion()
{
	// Gets the feature level of the hardware device.
	D3D_FEATURE_LEVEL fl = dev->GetFeatureLevel();
	switch (fl)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
	{
		return "vs_5_0";
	}
	break;
	case D3D_FEATURE_LEVEL_10_1:
	{
		return "vs_4_1";
	}
	break;
	case D3D_FEATURE_LEVEL_10_0:
	{
		return "vs_4_0";
	}
	break;
	case D3D_FEATURE_LEVEL_9_3:
	{
		return "vs_4_0_level_9_3";
	}
	break;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:
	{
		return "vs_4_0_level_9_1";
	}
	break;
	}

	return "NONE";
}
void DX::ClearScreen()
{
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(g_pRenderTargetView, color);
	devcon->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}