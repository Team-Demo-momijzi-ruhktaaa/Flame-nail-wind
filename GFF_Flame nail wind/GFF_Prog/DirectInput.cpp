#include "DirectInput.h"

DirectInput* DirectInput::pInstance = nullptr;

DirectInput::DirectInput()
{
	pDInput = nullptr;

	pKeyDevice = nullptr;
	pMouseDevice = nullptr;
	initialized = false;
	mousePos.Set(0, 0);
	mousePosDelta.Set(0, 0);
	mousePosPreview.Set(0, 0);

	ZeroMemory(KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(KeyInputPreview, sizeof(int)*INPUT_BUFFER_SIZE);
}

DirectInput::~DirectInput()
{
	Release();
}

HRESULT DirectInput::Init(HWND hWnd)
{
	this->hWnd = hWnd;

	if (initialized == false)
	{

		HRESULT hresult
			= DirectInput8Create
			(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
				IID_IDirectInput8, (VOID**)&pDInput, NULL);

		if (FAILED(hresult))
		{
			return hresult;
		}

		hresult = InitKey();

		hresult = InitMouse();

		if (FAILED(hresult))
		{
			return hresult;
		}

		initialized = true;
	}

	return S_OK;
}

HRESULT DirectInput::InitKey()
{
	HRESULT hresult;

	hresult = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL);

	if (FAILED(hresult))
	{
		return hresult;
	}

	hresult = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hresult))
	{
		return hresult;
	}

	hresult = pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	if (FAILED(hresult))
	{
		return hresult;
	}

	pKeyDevice->Acquire();

	return S_OK;
}

HRESULT DirectInput::InitMouse()
{
	HRESULT hresult;

	hresult = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);

	if (FAILED(hresult))return hresult;

	hresult = pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hresult))return hresult;

	hresult = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hresult))return hresult;

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	hresult = pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	if (FAILED(hresult))return hresult;

	hresult = pMouseDevice->Acquire();

	if (FAILED(hresult))return hresult;

	RECT rect;
	GetClientRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	POINT localPos;

	localPos.x = width / 2;
	localPos.y = height / 2;

	POINT worldPos = localPos;
	ClientToScreen(hWnd, &worldPos);

	SetCursorPos(worldPos.x, worldPos.y);

	mousePos.Set(localPos.x, localPos.y);
	mousePosPreview.Set(localPos.x, localPos.y);
	mousePosDelta.Set(0, 0);

	return S_OK;
}

HRESULT DirectInput::Update()
{
	if (initialized == false)
	{
		return S_FALSE;
	}

	HRESULT hresult = pKeyDevice->Acquire();

	if (hresult == DI_OK || hresult == S_FALSE)
	{
		memcpy(KeyInputPreview, KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);

		BYTE tKeyInputWork[INPUT_BUFFER_SIZE];

		pKeyDevice->GetDeviceState(sizeof(BYTE)*INPUT_BUFFER_SIZE, &tKeyInputWork);

		for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])
			{
				if (KeyInput[i] < 2147483647)
				{
					KeyInput[i]++;
				}
			}
			else
			{
				KeyInput[i] = 0;
			}
		}
	}

	hresult = pMouseDevice->Acquire();

	if (hresult == DI_OK || hresult == S_FALSE)
	{
		memcpy(&mouseInputPreview, &mouseInput, sizeof(DIMOUSESTATE2));

		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseInput);

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
		{
			if (mouseInput.rgbButtons[i])
			{
				if (mouseButtonCounter[i] < 2147483647)
				{
					mouseButtonCounter[i]++;
				}
			}
			else
			{
				mouseButtonCounter[i] = 0;
			}
		}
	}

	POINT screenPos;

	GetCursorPos(&screenPos);

	POINT localPos = screenPos;
	ScreenToClient(hWnd, &localPos);

	mousePosPreview = mousePos;
	mousePos.Set(localPos.x, localPos.y);
	mousePosDelta = mousePos - mousePosPreview;

	return S_OK;
}

bool DirectInput::KeyState(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code] > 0;
	}

	return false;
}

bool DirectInput::KeyJustPressed(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code] == 1;
	}

	return false;
}

bool DirectInput::KeyJustReleased(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return (KeyInput[code] == 0 && KeyInputPreview[code] > 0);
	}

	return false;
}

int DirectInput::KeyCount(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code];
	}

	return 0;
}

bool DirectInput::MouseButton(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return mouseInput.rgbButtons[num] >= 1;
	}
	else
	{
		return false;
	}
}

bool DirectInput::MouseButtonJustPressed(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return (mouseInput.rgbButtons[num] >= 1) && (mouseInputPreview.rgbButtons[num] == 0);
	}
	else
	{
		return false;
	}
}

bool DirectInput::MouseButtonJustReleased(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return (mouseInput.rgbButtons[num] == 0) && (mouseInputPreview.rgbButtons[num] >= 1);
	}
	else
	{
		return false;
	}
}

int DirectInput::MouseButtonCount(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return mouseButtonCounter[num];
	}
	else
	{
		return 0;
	}
}

HRESULT DirectInput::Release()
{
	if (pMouseDevice)
	{
		pMouseDevice->Unacquire();
		pMouseDevice->Release();
		pMouseDevice = NULL;
	}

	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
		pKeyDevice->Release();
		pKeyDevice = NULL;
	}

	if (pDInput)
	{
		pDInput->Release();
		pDInput = NULL;
	}

	initialized = false;

	return S_OK;
}