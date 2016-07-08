#define _CRTDBG_MAP_ALLOC // pervent memory leaking
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <crtdbg.h>
#include <stdlib.h>
#include "Spacewar.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
bool CreateMainWindow(HWND &,HINSTANCE, int);
LRESULT WINAPI WInProc(HWND, UINT, WPARAM, LPARAM);


Spacewar *game;
HWND hwnd = NULL;


/*application starting point*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;

	game = new Spacewar;

	// create window
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
	{
		return 1;
	}

	try
	{
		game->initialize(hwnd);

		//main loop
		int done = 0;
		while (!done)
		{
			// PeekMessage = checking window messge(non-blocking)
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					done = 1;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				game->run(hwnd);
			}
		}

		safeDelete(game);
		return msg.wParam;
	}
	catch (const GameError &error)
	{
		MessageBox(NULL, error.getMessage(), "error", MB_OK);
	}
	catch (...)
	{
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}
	safeDelete(game);
	return 0;
}

/*window event callback func*/
LRESULT WINAPI WInProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hwnd, msg, wParam, lParam);
}

/*create window*/
bool CreateMainWindow(HWND &hwnd,HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WInProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);

	// backgournd
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	// class register
	if (RegisterClassEx(&wcx) == 0) // error occur
	{
		return false;
	}

	DWORD style;
	if (FULLSCREEN)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	hwnd = CreateWindow(
		CLASS_NAME,
		GAME_TITLE,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GAME_WIDTH,
		GAME_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL
	);
	if (!hwnd)
	{
		return false;
	}

	if (!FULLSCREEN)             // if window
	{
		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}

	ShowWindow(hwnd, nCmdShow);

	return true;
}