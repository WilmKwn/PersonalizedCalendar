#include "Window.h"

Window::Window()
{
	window = NULL;
}

Window::~Window()
{
}

std::pair<int, int> Window::dimensions = { 0, 0 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CHAR:
		if (wp == VK_ESCAPE) Calendar::SetFocus(-1, -1);
		else if (wp == VK_TAB) Calendar::SetSave(true);
		else if (wp == VK_BACK) Calendar::SetText("<-");
		else if ((char)wp == '}') Calendar::SetTakeScreenshot(true);
		else if ((char)wp == '`') Calendar::SetText("###");
		else if ((char)wp == '\\') Calendar::SetDeleteAll(true);
		else Calendar::SetText(std::string(1, (char)wp));

		return 0;
	case WM_LBUTTONDOWN:
		Calendar::SetFocus(LOWORD(lp), HIWORD(lp));
		return 0;
	case WM_SIZE:
		RECT r;
		GetClientRect(hWnd, &r);
		Window::SetDimensions((int)(r.right - r.left), (int)(r.bottom - r.top));
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void Window::Init(int w, int h, HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = hInstance;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_HAND);
	wcex.hIconSm = LoadIcon(NULL, IDI_HAND);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "calendar";
	wcex.lpfnWndProc = WndProc;
	RegisterClassEx(&wcex);

	w = GetSystemMetrics(SM_CXSCREEN) / 1.2;
	h = GetSystemMetrics(SM_CYSCREEN) / 1.2;

	int x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (w / 2);
	int y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (h / 2);

	window = CreateWindowEx(NULL, "calendar", "Calendar", WS_VISIBLE | WS_POPUPWINDOW, x, y, w, h, NULL, NULL, hInstance, NULL);

	dimensions = { w, h };
}

HWND Window::GetWindow()
{
	return window;
}