#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>

#include "Calendar.h"

class Window
{
public:
	Window();
	~Window();

	void Init(int w, int h, HINSTANCE hInstance);

	HWND GetWindow();

	static void SetDimensions(int w, int h) { dimensions = { w,h }; }
	static std::pair<int, int> GetDimensions() { return dimensions; }

private:
	HWND window;

	static std::pair<int, int> dimensions;
};