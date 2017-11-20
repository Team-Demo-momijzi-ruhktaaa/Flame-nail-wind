#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>

#include"Direct3D.h"

#include"Map.h"

#include"Sprite.h"
#include"Texture.h"

#include"MeshX.h"

#include"DirectInput.h"

#include "Player.h"
#include "Menu.h"
#include"EnemyManager.h"

#define windowWidth 832
#define windowHeight 704
#define CommonMapSize 64

void Scroll(Player *player, Map *map, int viewX, int viewY, int &scrollX, int &scrollY);
void SetPlayer(Player *player, Map *map);

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

	//ヘルプ機能を付けるならココ！
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
	int width = windowWidth,
	int height = windowHeight
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
		_T("焔爪風牙 ver0.208"),
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
//int main()
{
	//HINSTANCE hInstance = GetModuleHandle(NULL);
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

	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);
	d3d.SetProjectionMatrixs();

	//後にこの辺りも管理する
	//ここを変えれば次の階層が作成されるようにする
	Map map;
	map.CleateMap(20, 20);
	Player playerData;
	Menu menuData;
	EnemyManager eMane;
	eMane.SetEnemy(map.CallMapWidth,map.CallMapHeight,map.CallCleateRoomNum);
	eMane.SetEStatus(map.CallMapWidth,map.CallMapHeight,map.CallCleateRoomNum);

	if (map.CallPlayerRoom() != -1)
	{
		playerData.SetPlayer(map.ChoicePlayerMapX(map.CallPlayerRoom())*map.MAPCIPSIZE,
			map.ChoicePlayerMapY(map.CallPlayerRoom())*map.MAPCIPSIZE);
	}
	else
	{
		playerData.SetPlayer((rand() % (map.CallMapWidth() - 2) + 1) * map.MAPCIPSIZE,
			(rand() % (map.CallMapHeight() - 2) + 1) * map.MAPCIPSIZE);
	}

	int mapheight = map.CallMapHeight();
	int mapwidth = map.CallMapWidth();
	int scrollX = 0;
	int scrollY = 0;
	int mapCenterX = windowWidth / 2;
	int mapCenterY = windowHeight / 2;
	int playerviewX = (windowWidth / CommonMapSize * map.CallMapCipSize()) / 2;
	int playerviewY = (windowHeight / CommonMapSize * map.CallMapCipSize()) / 2;


	Sprite sprite;
	sprite.SetSize(map.MAPCIPSIZE, map.MAPCIPSIZE);

	Sprite halfsprite;
	halfsprite.SetSize(map.MAPCIPSIZE, map.MAPCIPSIZE / 2);

	Sprite s_menu;
	s_menu.SetAlpha(0.9f);
	s_menu.SetSize(windowWidth / 2, windowHeight / 2);

	Sprite mini;
	mini.SetAlpha(0.75f);
	mini.SetSize(map.MINIMAPSIZE, map.MINIMAPSIZE);

	Texture wall;
	wall.Load(_T("Texture/wall2.png"));

	Texture tuta;
	tuta.Load(_T("Texture/tutaWall.png"));

	Texture t_stair;
	t_stair.Load(_T("Texture/DownStair.png"));

	Texture fixwall;
	fixwall.Load(_T("Texture/Magic2.png"));

	Texture mapnumber;
	mapnumber.Load(_T("Texture/number3.png"));
	mapnumber.SetDivide(10, 1);

	Texture fuuka;
	fuuka.Load(_T("Texture/fuuka.png"));

	Texture t_enemy;
	t_enemy.Load(_T("Texture/golemu.png"));

	Texture t_enemy2;
	t_enemy2.Load(_T("Texture/fly.png"));


	Texture t_menu;
	t_menu.Load(_T("Texture/menu.png"));

	Texture ministair;
	ministair.Load(_T("Texture/Magic3.png"));

	DirectInput*pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	SetPlayer(&playerData, &map);

	Scroll(&playerData, &map, playerviewX, playerviewY, scrollX, scrollY);

	map.KnowMiniMap(&playerData);

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
			DirectInput*pDi = DirectInput::GetInstance();
			pDi->Update();

			playerData.MoveControl(&map, pDi, scrollX, scrollY, playerviewX, playerviewY);

			if (map.CallNextStair() == true && pDi->KeyJustPressed(DIK_RETURN)
				&& scrollX%map.CallMapCipSize() == 0 && scrollY%map.CallMapCipSize() == 0)
			{
				map.NewMapCleate();
				SetPlayer(&playerData, &map);
				Scroll(&playerData, &map, playerviewX, playerviewY, scrollX, scrollY);
				mapheight = map.CallMapHeight();
				mapwidth = map.CallMapWidth();
			}

			if (menuData.CallMenuOpen() == false && pDi->KeyJustPressed(DIK_X))
			{
				menuData.CleateMenu();
			}
			else if(menuData.CallMenuOpen() == true && pDi->KeyJustPressed(DIK_X))
			{
				menuData.ClauseMenu();
			}


			if (SUCCEEDED(d3d.BeginScene()))
			{
				d3d.ClearScreen();
				d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);

				int viewWidth = -playerData.CallPlayerX() - map.MAPCIPSIZE / 2 + playerviewX*CommonMapSize / map.CallMapCipSize();
				int viewHeight = -playerData.CallPlayerY() - map.MAPCIPSIZE / 2 + playerviewY*CommonMapSize / map.CallMapCipSize();

				for (int i = 0; i < mapwidth; i++)
				{
					for (int j = 0; j < mapheight; j++)
					{
						if (map.CallMap(i, j) == map.WALL || map.CallMap(i, j) == map.FIXWALL)
						{
							sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewWidth, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewHeight);
							sprite.Draw(wall);
						}
						if (map.CallMap(i, j) == 0 || map.CallMap(i, j) == map.STAIR)
						{
							if (map.CallMap(i, j - 1) == map.WALL || map.CallMap(i, j - 1) == map.FIXWALL)
							{
								halfsprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewWidth, (j - 1)*map.MAPCIPSIZE + map.MAPCIPSIZE * 3 / 4 + viewHeight);
								halfsprite.Draw(tuta);
							}
						}

						if (map.CallpMapRoomData(i, j) != 0)
						{
							sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewWidth, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewHeight);
							int num = map.CallpMapRoomData(i, j) % 10;
							mapnumber.SetNum(num, 0);
							sprite.Draw(mapnumber);
						}

						if (map.CallMap(i, j) == map.STAIR)
						{
							sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewWidth, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewHeight);
							sprite.Draw(t_stair);
						}
						if (map.CallClearRoadData(i,j) == true)
						{
							sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewWidth, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewHeight);
							sprite.Draw(fixwall);
						}

						if (eMane.callCharData(i, j, map.CallMapWidth) != 0)
						{
							sprite.SetPos(i*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewWidth, j*map.MAPCIPSIZE + map.MAPCIPSIZE / 2 + viewHeight);
							if (eMane.callCharData(i, j, map.CallMapWidth) == 1)
							{
								sprite.Draw(t_enemy);
							}
							else
							{
								sprite.Draw(t_enemy2);
							}
						}
	
					}
				}

				sprite.SetPos(mapCenterX, mapCenterY);
				sprite.Draw(fuuka);

				if (menuData.CallMenuOpen() == true)
				{
					int a = 0;
					s_menu.SetPos(windowWidth / 2, windowHeight / 2);
					s_menu.Draw(t_menu);
				}
				for (int i = 0; i < map.CallMapWidth(); i++)
				{
					for (int j = 0; j < map.CallMapHeight(); j++)
					{
						if (map.CallMiniMap(i, j) == 1)
						{
							mini.SetPos(i*map.MINIMAPSIZE + map.MINIMAPSIZE / 2, j*map.MINIMAPSIZE + map.MINIMAPSIZE / 2);
							mini.Draw(fixwall);
						}
						else if (map.CallMiniMap(i, j) == 2)
						{
							mini.SetPos(i*map.MINIMAPSIZE + map.MINIMAPSIZE / 2, j*map.MINIMAPSIZE + map.MINIMAPSIZE / 2);
							mini.Draw(ministair);
						}
					}
				}

				d3d.EndScene();

				d3d.Present();
			}
		}
	}

	return 0;

}

void SetPlayer(Player *player, Map *map)
{

	if (map->CallPlayerRoom() != -1)
	{
		player->SetPlayer(map->ChoicePlayerMapX(map->CallPlayerRoom())*map->MAPCIPSIZE,
			map->ChoicePlayerMapY(map->CallPlayerRoom())*map->MAPCIPSIZE);
	}
	else
	{
		player->SetPlayer((rand() % (map->CallMapWidth() - 2) + 1) * map->MAPCIPSIZE,
			(rand() % (map->CallMapHeight() - 2) + 1) * map->MAPCIPSIZE);
	}
}

void Scroll(Player *player, Map *map, int viewX, int viewY, int &scrollX, int &scrollY)
{

	scrollX = -player->CallPlayerX() - map->MAPCIPSIZE / 2 + viewX;
	scrollY = -player->CallPlayerY() - map->MAPCIPSIZE / 2 + viewY;
}