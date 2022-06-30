#include "Window.h"
#include "Graphics.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) 
{
	Window *window = new Window();
	window->Init(800, 600, hInstance);

	Graphics *graphics = new Graphics();
	graphics->Init(window->GetWindow(), window->GetDimensions());

	Calendar *calendar = new Calendar(graphics, window->GetDimensions(), window->GetWindow());
	
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		graphics->Begin();
		
		graphics->ClearScreen(0.1f, 0.1f, 0.1f);
		calendar->Update();

		graphics->End();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}
	}
	return (int)msg.wParam;
}