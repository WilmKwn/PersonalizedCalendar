#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;

	writeFactory = NULL;
	writeFormat = NULL;
}

Graphics::~Graphics()
{
	factory->Release();
	renderTarget->Release();
	brush->Release();

	writeFactory->Release();
	writeFormat->Release();
}

void Graphics::Init(HWND window, std::pair<int, int> size)
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(size.first, size.second)), &renderTarget);

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &brush);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
}

void Graphics::Begin()
{
	renderTarget->BeginDraw();
}

void Graphics::End()
{
	renderTarget->EndDraw();
}

void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b, 1.0f));
}

void Graphics::DrawLine(std::pair<int, int> p1, std::pair<int, int> p2, D2D1_COLOR_F color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->DrawLine(D2D1::Point2F((float)p1.first, (float)p1.second), D2D1::Point2F((float)p2.first, (float)p2.second), brush);
}

void Graphics::DrawRect(std::pair<int, int> p1, std::pair<int, int> p2, D2D1_COLOR_F color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->FillRectangle(D2D1::RectF(p1.first, p1.second, p2.first, p2.second), brush);
}

void Graphics::WriteText(std::string text, int x, int y, int w, int h, D2D1_COLOR_F color)
{
	renderTarget->CreateSolidColorBrush(color, &brush);
	writeFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, w / 15.0, L"en-us", &writeFormat);
	renderTarget->DrawTextA(std::wstring(text.begin(), text.end()).c_str(), text.size(), writeFormat, D2D1::RectF(x, y, x + w, y + h), brush);
}