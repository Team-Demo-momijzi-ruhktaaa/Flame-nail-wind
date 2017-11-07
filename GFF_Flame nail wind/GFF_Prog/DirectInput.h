#pragma once

#define D3D_DEBUG_INFO

#define DIRECT_INPUT_VERSION 0x0800

#define MOUSE_BUTTON_LEFT	0
#define MOUSE_BUTTON_RIGHT	1
#define MOUSE_BUTTON_MIDDLE	2

#define MOUSE_BUTTON_MAX 8

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#pragma comment(lib,"dinput8.lib")

#include<comdef.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<mmsystem.h>
#include<dinput.h>

#include"Vector2D.h"

#define INPUT_BUFFER_SIZE 256

class DirectInput
{
private:
	static DirectInput* pInstance;
public:
	static DirectInput* GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new DirectInput();
		}
		return pInstance;
	}

private:

	LPDIRECTINPUT8 pDInput;

	LPDIRECTINPUTDEVICE8 pKeyDevice;

	LPDIRECTINPUTDEVICE8 pMouseDevice;

	int KeyInput[INPUT_BUFFER_SIZE];

	int KeyInputPreview[INPUT_BUFFER_SIZE];

	Vector2<int> mousePos;

	Vector2<int> mousePosPreview;

	Vector2<int> mousePosDelta;

	DIMOUSESTATE2 mouseInput;

	DIMOUSESTATE2 mouseInputPreview;

	int mouseButtonCounter[MOUSE_BUTTON_MAX];

	HWND hWnd;

	bool initialized;

private:

	DirectInput();
	~DirectInput();
	DirectInput(DirectInput& refDi) {};
	DirectInput& operator=(DirectInput& refDi) {};

	HRESULT InitKey();
	HRESULT InitMouse();

public:
	HRESULT Init(HWND hWnd);

	HRESULT Release();

	HRESULT Update();

	bool KeyState(int code);
	bool KeyJustPressed(int code);
	bool KeyJustReleased(int code);
	int KeyCount(int code);

	bool MouseButton(int num);
	bool MouseButtonJustPressed(int num);
	bool MouseButtonJustReleased(int num);
	int MouseButtonCount(int num);

	Vector2<int> MousePosition() { return mousePos; }
	Vector2<int> MousePositionDelta() { return mousePosDelta; }
};