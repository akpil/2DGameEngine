#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

class Graphics;

#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

// DirectX pointer
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9
#define LP_SPRITE	LPD3DXSPRITE
#define LP_TEXTURE	LPDIRECT3DTEXTURE9

// define color
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) ((COLOR_ARGB)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)))

class Graphics
{
private:
	// DirectX pointer & releated variables
	LP_3D		direct3d;
	LP_3DDEVICE	device3d;
	LP_SPRITE	sprite;

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;
	
	// other variables
	HRESULT		result;
	HWND		hwnd;
	bool		fullscreen;
	int			width;
	int			height;
	COLOR_ARGB backColor;
	
	// inside of engine
	// D3D presentation Parameters reset
	void		initD3Dpp();
public:
	Graphics();

	virtual ~Graphics();

	void releaseAll();

	/*
	DirectX Graphics reset
	hw = window handle
	width = fixel width
	height = fixel height
	fullscreen = fullscreen mode toggle
	*/
	void initialize(HWND hw, int width, int height, bool fullscreen);
	// set backbuffer
	HRESULT showBackbuffer();

	HRESULT beginScene();
	HRESULT endScene();

	HRESULT getDeviceState();

	HRESULT reset();

	bool isAdapterCompatible();

	//get device context
	HDC getDC()
	{
		return GetDC(hwnd);
	}

	void setBackColor(COLOR_ARGB c)
	{
		backColor = c;
	}
	
	// get direct3d
	LP_3D get3D()
	{
		return direct3d;
	}

	LP_3DDEVICE get3Ddevice()
	{
		return device3d;
	}

	// call texture
	HRESULT loadTexture(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
};
#endif // !_GRAPHICS_H
