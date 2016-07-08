#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN
#include "game.h"

class Spacewar : public Game
{
private:
	int count = 0;
	bool mousePrev = false;
public:
	Spacewar();
	virtual ~Spacewar();

	void initialize(HWND hwnd);
	void update();


	void ai();
	void collisions();
	void render();
	void releaseAll();
	void resetAll();
};
#endif