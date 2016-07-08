#include "Spacewar.h"

Spacewar::Spacewar()
{
}

Spacewar::~Spacewar()
{
	releaseAll();
}

void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd);
	return;
}

void Spacewar::update()
{

	DWORD colors[4] = { SETCOLOR_ARGB(255,128,0,0), SETCOLOR_ARGB(255,0,128,0), SETCOLOR_ARGB(255,0,0,128), SETCOLOR_ARGB(255,128,128,128) };
	bool mousetemp = input->getMouseLButton();
	if (mousetemp && !mousePrev)
	{
		count = (count+1)%4;
	}
	mousePrev = mousetemp;
	graphics->setBackColor(colors[count]);

	std::string text = input->getTextIn();
	if (text.compare("exit") == 0)
	{
		exitGame();
	}

	input->checkControllers();
	UINT number;
	for (UINT i = 0; i < 4; i++)
	{
		if (input->getGamepadConnected(i))
		{
			number = i;
		}
	}

	BYTE Ltrigger = input->getGamepadLeftTrigger(number);
	BYTE Rtrigger = input->getGamepadRightTrigger(number);

	input->gamePadVibrateLeft(number,Ltrigger << 2, 1.0f);
	input->gamePadVibrateRight(number, Rtrigger << 2, 1.0f);
}

void Spacewar::ai()
{
}

void Spacewar::collisions()
{
}

void Spacewar::render()
{
}

void Spacewar::releaseAll()
{
	Game::releaseAll();
	return;
}

void Spacewar::resetAll()
{
	Game::resetAll();
	return;
}

