#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

//high definition mouse
#ifndef HID_USAGE_PAGE_GNENRIC
#define HID_USAGE_PAGE_GNENRIC		((USHORT)0x01)
#endif // !HID_USAGE_PAGE_GNENRIC
#ifndef HID_USAGE_GNENRIC_MOUSE
#define HID_USAGE_GNENRIC_MOUSE		((USHORT)0x02)
#endif // !HID_USAGE_GNENRIC_MOUSE

const short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0x7fff);
const short GAMEPAD_TRIGGER_DEADZONE = 20;
const DWORD MAX_CONTROLLERS = 4;

const DWORD GAMEPAD_DPAD_UP			 = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN		 = 0x0002; 
const DWORD GAMEPAD_DPAD_LEFT		 = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT		 = 0x0008;
const DWORD GAMEPAD_START_BUTTON	 = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON		 = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB		 = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB		 = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER	 = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER	 = 0x0200;
const DWORD GAMEPAD_A				 = 0x1000;
const DWORD GAMEPAD_B				 = 0x2000;
const DWORD GAMEPAD_X				 = 0x3000;
const DWORD GAMEPAD_Y				 = 0x4000;



namespace inputNS
{
	const int KEYS_ARRAY_LEAN = 256;

	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

struct ControllerState
{
	XINPUT_STATE		state;
	XINPUT_VIBRATION	vibration;
	float				vibrateTimeLeft;
	float				vibrateTimeRight;
	bool				connected;
};

class Input
{
private:
	bool keysDown[inputNS::KEYS_ARRAY_LEAN];
	bool keysPressed[inputNS::KEYS_ARRAY_LEAN];

	std::string textIn;
	char charIn;
	bool newLine;
	int mouseX, mouseY;
	int mouseRawX, mouseRawY; // high-definition mouse
	int mouseWheel;
	RAWINPUTDEVICE Rid[1];	// high-definition mouse

	bool mouseCaptured;
	bool mouseLButton;
	bool mouseMButton;
	bool mouseRButton;
	bool mouseX1Button;
	bool mouseX2Button;
	ControllerState controllers[MAX_CONTROLLERS];
	short thumbstickDeadzone;
	short triggerDeadzone;

public:
	Input();
	virtual ~Input();

	void initialize(HWND hwnd, bool capture);

	// keyboard
	void keyDown(WPARAM wParam);
	void keyUp(WPARAM wParam);
	void keyIn(WPARAM wParam);

	bool isKyeDown(UCHAR vkey) const;
	bool wasKeyPressed(UCHAR vkey) const;
	bool anyKeyPressed() const;

	std::string getTextIn() 
	{
		return textIn; 
	}

	void setTextIn(std::string str)
	{
		textIn = str;
	}

	char getCharIn()
	{
		return charIn;
	}

	// clear
	void clearKeyPress(UCHAR vkey);
	void clear(UCHAR what);

	void clearCharIn()
	{
		charIn = 0;
	}

	void clearTextIn()
	{
		textIn.clear();
	}

	void clearAll()
	{
		clear(inputNS::KEYS_MOUSE_TEXT);
	}

	// mosue
	void mouseIn(LPARAM lParam);
	void mouseRawIn(LPARAM lParam);
	void mouseWheelIn(WPARAM wParam);

	void setMouseLButton(bool b)
	{
		mouseLButton = b;
	}
	void setMouseMButton(bool b)
	{
		mouseMButton = b;
	}
	void setMouseRButton(bool b)
	{
		mouseRButton = b;
	}
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	int getMouseX() const
	{
		return mouseX;
	}
	int getMouseY() const
	{
		return mouseY;
	}

	// have to change??????
	int getMouseRawX() const
	{
		return mouseRawX;
	}
	int getMouseRawY() const
	{
		return mouseRawY;
	}
	int getMouseWheel() const
	{
		return mouseWheel;
	}
	// end

	bool getMouseLButton() const
	{
		return mouseLButton;
	}
	bool getMouseMButton() const
	{
		return mouseMButton;
	}
	bool getMouseRButton() const
	{
		return mouseRButton;
	}
	
	bool getMouseX1Button() const
	{
		return mouseX1Button;
	}
	bool getMouseX2Button() const
	{
		return mouseX2Button;
	}
	
	// controller
	void checkControllers();
	void readControllers();

	void setThumbstickDeadzone(short dz)
	{
		thumbstickDeadzone = dz;
	}

	void setTriggerDeadzone(BYTE dz)
	{
		triggerDeadzone = dz;
	}

	short getThumbstickDeadzone()
	{
		return thumbstickDeadzone;
	}

	BYTE getTriggerDeadzone()
	{
		return static_cast<BYTE>(triggerDeadzone);
	}

	const ControllerState* getControllerstate(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return &controllers[n];
	}

	bool getGamepadConnected(UINT n)
	{
		if (n > MAX_CONTROLLERS)
		{
			n = MAX_CONTROLLERS;
		}
		return controllers[n].connected;
	}

	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS)
		{
			n = MAX_CONTROLLERS;
		}
		return controllers[n].state.Gamepad.wButtons;
	}
	
	// non-Undead = dead zone applied
	// Undead = dead zone non applied
	BYTE getGamepadLeftTrigger(UINT n);
	BYTE getGamepadLeftTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return controllers[n].state.Gamepad.bLeftTrigger;
	}

	BYTE getGamepadRightTrigger(UINT n);
	BYTE getGamepadRightTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return controllers[n].state.Gamepad.bRightTrigger;
	}
	
	SHORT getGamepadThumbLX(UINT n);
	SHORT getGamepadThumbLXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return controllers[n].state.Gamepad.sThumbLX;
	}

	SHORT getGamepadThumbLY(UINT n);
	SHORT getGamepadThumbLYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return controllers[n].state.Gamepad.sThumbLY;
	}

	SHORT getGamepadThumbRX(UINT n);
	SHORT getGamepadThumbRXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return controllers[n].state.Gamepad.sThumbRX;
	}

	SHORT getGamepadThumbRY(UINT n);
	SHORT getGamepadThumbRYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		return controllers[n].state.Gamepad.sThumbRY;
	}

	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	void vibrateControllers(float frameTime);

		// gamepad checking
	// Return state of controller n D-pad Up
	bool getGamepadDPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	// Return state of controller n D-pad Down.
	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	// Return state of controller n D-pad Left.
	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	// Return state of controller n D-pad Right.
	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	// Return state of controller n Start button.
	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	// Return state of controller n Back button.
	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	// Return state of controller n Left Thumb button.
	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	// Return state of controller n Right Thumb button.
	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	// Return state of controller n Left Shoulder button.
	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// Return state of controller n Right Shoulder button.
	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// Return state of controller n A button.
	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	// Return state of controller n B button.
	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	// Return state of controller n X button.
	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	// Return state of controller n Y button.
	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}
};

#endif