#include "graphics.h"

Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = true;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	backColor = SETCOLOR_ARGB(255, 0, 128, 0);
}

Graphics::~Graphics()
{
	releaseAll();
}

void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

void Graphics::initialize(HWND hw, int w, int h, bool ful)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = ful;

	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "ERROR initializing Direct3d"));
	}
	initD3Dpp();
	if (fullscreen)
	{
		if (isAdapterCompatible())
		{
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		}
		else
		{
			throw(GameError(gameErrorNS::FATAL_ERROR, "THe graphics device does not support the"\
				"specified resolution and/ro format"));
		}
	}

	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else 
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device3d);

	if (FAILED(result))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3d device"));
	}

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3d sprite"));
	}
}

void Graphics::initD3Dpp()
{
	try
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)
		{
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		else
		{
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		}
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	} catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL; // defalut = fail, if success change.

	result = device3d->Present(NULL, NULL, NULL, NULL);
	return result;
}

bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);

	for (UINT i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);
		if (pMode.Height == d3dpp.BackBufferHeight && pMode.Width == d3dpp.BackBufferWidth && pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
		{
			return true;
		}
	}
	return false;
}

HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;
	if (device3d == NULL)
	{
		return result;
	}
	result = device3d->TestCooperativeLevel();

	return result;
}

HRESULT Graphics::reset()
{
	result = E_FAIL;
	initD3Dpp();
	result = device3d->Reset(&d3dpp);

	return result;
}

HRESULT Graphics::beginScene()
{
	result = E_FAIL;
	if (device3d == NULL)
	{
		return result;
	}
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0f, 0);
	result = device3d->BeginScene();
	return result;
}

HRESULT Graphics::endScene()
{
	result = E_FAIL;
	if (device3d)
	{
		result = device3d->EndScene();
	}
	return result;
}

HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture)
{
	D3DXIMAGE_INFO info;
	result = E_FAIL;

	try 
	{
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
		{
			return result;
		}
		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(device3d, filename, info.Width, info.Height, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transcolor, &info, NULL, &texture);
	}
	catch (...)
	{
		throw (GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}

	return result;
}