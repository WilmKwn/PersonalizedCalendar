#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include <iostream>

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND window, std::pair<int,int> size);

	void Begin();
	void End();

	void ClearScreen(float r, float g, float b);
	void DrawLine(std::pair<int,int> p1, std::pair<int, int> p2, D2D1_COLOR_F color);
	void DrawRect(std::pair<int, int> p1, std::pair<int, int> p2, D2D1_COLOR_F color);
	void WriteText(std::string text, int x, int y, int w, int h, D2D1_COLOR_F color);

private:
	ID2D1Factory *factory;
	ID2D1HwndRenderTarget *renderTarget;
	ID2D1SolidColorBrush *brush;

	IDWriteFactory *writeFactory;
	IDWriteTextFormat *writeFormat;
};