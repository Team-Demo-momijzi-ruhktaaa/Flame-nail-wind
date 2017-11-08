#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>

#include"Direct3D.h"

#include"Map.h"
#include"DirectInput.h"

#include"Sprite.h"
#include"Texture.h"

#include"MeshX.h"



LRESULT CALLBACK WndPrc
(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}


static const TCHAR* WC_BASIC = _T("Basic");

HRESULT RegistClassEx(HINSTANCE hInstance)
{

	WNDCLASSEX wcex;

	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//�w���v�@�\��t����Ȃ�R�R�I
	wcex.lpszMenuName = NULL;

	wcex.lpszClassName = WC_BASIC;

	wcex.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	wcex.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	return RegisterClassEx(&wcex);

}

HRESULT MakeWindow
(
	HINSTANCE hInstance,
	HWND &refHWnd,
	//��ʂ̔䗦
	int width = 800,
	int height = 600
)
{

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd;

	hWnd = CreateWindowEx
	(
		0,
		WC_BASIC,
		_T("����[�[�["),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hWnd == NULL)
	{
		return S_FALSE;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);

	UpdateWindow(hWnd);

	refHWnd = hWnd;

	return S_OK;

}

int _stdcall WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	//MessageBox
	//(
	//	NULL,
	//	TEXT("�e�X�g"),
	//	TEXT("�e�X�g-�^�C�g��"),
	//	MB_OK
	//);

	HWND hWnd = NULL;

	if (FAILED(RegistClassEx(hInstance)))
	{
		return 0;
	}

	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		return 0;
	}

	Direct3D& d3d = Direct3D::GetInstance();

	if (d3d.TryCreate(hWnd))
	{

	}
	
	Map map;
	map.CleateMap(20, 20);

	//�}�b�v�̑S�̂̑傫��
	int mapheight = map.CallMapHeight();
	int mapwidth = map.CallMapWidth();

	//�X�v���C�g�̐ݒ�
	Sprite sprite;
	sprite.SetAlpha(0);
	sprite.SetSize(map.MAPCIPSIZE, map.MAPCIPSIZE);
	sprite.SetAngle(0);

	//�摜�i�[�ϐ��@�܂��i�[
	Texture wall;
	wall.Load(_T("Texture/block.png"));
	wall.SetDivide(1, 1);

	Texture fixwall;
	fixwall.Load(_T("Texture/Magic2.png"));
	fixwall.SetDivide(1, 1);

	Texture Stage;
	Stage.Load(_T("Texture/Stage.png"));
	Stage.SetDivide(1, 1);

	DirectInput*pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);


	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
		}

		





		if (SUCCEEDED(d3d.BeginScene()))
		{
			d3d.ClearScreen();
			//�`��
			//�}�b�v��������
			for (int i = 0; i < mapwidth; i++)
			{
				for (int j = 0; j < mapheight; j++)
				{
					//�ǂ̎��`��
					if (map.CallMap(i, j) == map.WALL)
					{
						sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2);
						sprite.Draw(wall);
					}
					//�󂹂Ȃ��ǂ̕`��
					if (map.CallMap(i, j) == map.FIXWALL)
					{
						sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2);
						sprite.Draw(fixwall);
					}
				}
			}

			d3d.EndScene();

			d3d.Present();
		}
	}

	return 0;

}