#include "Graphics.h"
#include <string>
#include <iostream>
Graphics::Graphics()
{

	factory = NULL;
	rendertarget = NULL;
	brush = NULL;
}
Graphics::~Graphics()
{
	if (factory)
	{
		factory->Release();
		brush->Release();
	}
	if (rendertarget) rendertarget->Release();
}


bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;
	RECT rect;
	GetClientRect(windowHandle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(
		rect.right, rect.bottom);
	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, size),
		&rendertarget
	);
	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;

	BoardWidth =  float(rendertarget->GetPixelSize().width);
	BoardHeight =  float(rendertarget->GetPixelSize().height);
	EachWidth =  BoardWidth / 10.0f;
	EachHeight = BoardHeight / 10.0f;
	return true;
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	ID2D1SolidColorBrush *brush;
	rendertarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	rendertarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);
}
void Graphics::DrawTriangle(int x, int y)
{
	float half_Width = EachWidth / 2.0f;		// Half size of each grid Width
	float half_Height = EachHeight/2.0f;		// Half size of each grid Height
	HRESULT hr = NULL;
	ID2D1PathGeometry* m_pathGeometry = NULL;
	if (SUCCEEDED(hr))
	{
		hr = factory->CreatePathGeometry(&m_pathGeometry);

		if (SUCCEEDED(hr))
		{
			ID2D1GeometrySink *pSink = NULL;
			hr = m_pathGeometry->Open(&pSink);
			//Starts a new figure at the specified point.
			pSink->BeginFigure(
				D2D1::Point2F(x + (half_Width / 2), y + (half_Height / 2)), // Starting point : x+(width/4), y+(height/4)
				D2D1_FIGURE_BEGIN_FILLED //Indicates the figure will be filled by the FillGeometry
			);
			D2D1_POINT_2F point[2] = {
				D2D1::Point2F(x + EachWidth - (half_Width / 2), y + (half_Height / 2)), // Second Point
				D2D1::Point2F(x + half_Width,       y + EachHeight - (half_Height / 2)) // Third Point 
			};
			pSink->AddLines(point, ARRAYSIZE(point));		// Drawing Line(Connecting Each points)
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}
	brush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
	rendertarget->DrawGeometry(m_pathGeometry, brush, 1.0f, NULL);	// Drawing Triangle with color(Outline)
	rendertarget->FillGeometry(m_pathGeometry, brush);
}

void Graphics::DrawSmallTriangle(int x, int y)
{
	float half_Width = EachWidth / 2.0f;		// Half size of each grid Width
	float half_Height = EachHeight / 2.0f;		// Half size of each grid Height
	HRESULT hr = NULL;
	ID2D1PathGeometry* m_pathGeometry = NULL;
	if (SUCCEEDED(hr))
	{
		hr = factory->CreatePathGeometry(&m_pathGeometry);

		if (SUCCEEDED(hr))
		{
			ID2D1GeometrySink *pSink = NULL;
			hr = m_pathGeometry->Open(&pSink);
			//Starts a new figure at the specified point.
			pSink->BeginFigure(
				D2D1::Point2F(x + half_Width - half_Width/4, y + (half_Height / 2)), // Starting point : x+(width/4), y+(height/4)
				D2D1_FIGURE_BEGIN_FILLED //Indicates the figure will be filled by the FillGeometry
			);
			D2D1_POINT_2F point[2] = {
				D2D1::Point2F(x + half_Width + half_Width/4, y + (half_Height / 2)), // Second Point
				D2D1::Point2F(x + half_Width,       y + half_Height) // Third Point 
			};
			pSink->AddLines(point, ARRAYSIZE(point));		// Drawing Line(Connecting Each points)
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}
	brush->SetColor(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
	rendertarget->DrawGeometry(m_pathGeometry, brush, 1.0f, NULL);	// Drawing Triangle with color(Outline)
	rendertarget->FillGeometry(m_pathGeometry, brush);
}


void Graphics::DrawString(std::string str, float x, float y, float r, float g, float b, float a)
{
	HRESULT hr = NULL;
	IDWriteFactory* idFactory = NULL;
	IDWriteTextFormat *m_textFormat = NULL;
	ID2D1Factory* m_pDirect2dFactory = NULL; // Declare a factory pointer. 
											 // Create a direct2D Factory 
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	if (SUCCEEDED(hr))
	{
		// CREATE a DirectWrite Factory
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(idFactory),
			reinterpret_cast<IUnknown**>(&idFactory)
		);
	}
	// Applying text format.	
	if (SUCCEEDED(hr))
	{
		hr = idFactory->CreateTextFormat(
			L"Gabriola",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			25.0f,
			L"en-us",
			&m_textFormat
		);
	}
	D2D1_SIZE_F renderTargetSize = rendertarget->GetSize();
	std::wstring hey;
	for (int i = 0; i < str.length(); ++i)
	{
		hey += wchar_t(str[i]);
	}
	const wchar_t* StringW = hey.c_str();
	brush->SetColor(D2D1::ColorF(1, 1, 1, 1));
	rendertarget->DrawTextA(  // Drawing in the screen with format and size.
		StringW,
		str.length(),
		m_textFormat,
		D2D1::RectF(x, y, renderTargetSize.width, renderTargetSize.height),
		brush
	);
}



void Graphics::GridLine(float r, float g, float b, float a)
{
	for (int x = 0; x < 10; x++)
	{
		brush->SetColor(D2D1::ColorF(r, g, b, a));
		rendertarget->DrawLine(D2D1::Point2F(EachWidth *(x + 1), 0.0f),
			D2D1::Point2F(EachWidth *(x + 1), EachHeight* 10.0f),
			brush, 3.0f);
	}
	for (int y = 0; y < 10; y++)
	{
		brush->SetColor(D2D1::ColorF(r, g, b, a));
		rendertarget->DrawLine(D2D1::Point2F(0.0f, (EachHeight *(y + 1))),
			D2D1::Point2F(EachWidth * 10.0f, (EachHeight * (y + 1))),
			brush, 3.0f);
	}
}
void Graphics::ClearScreen(float r, float g, float b)
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}