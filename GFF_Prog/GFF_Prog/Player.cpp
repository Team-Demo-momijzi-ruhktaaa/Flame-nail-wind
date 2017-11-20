#include "Player.h"
#include "Map.h"
#include "DirectInput.h"

Player::Player() :MOVESPEED(4)
{
	playerx = 0;
	playery = 0;
	playerMoveX = 0;
	playerMoveY = 0;
	moveX = false;
	moveY = false;
	movecount = 0;
	justInput = false;
}

Player::~Player()
{
}

void Player::SetPlayer(int x, int y)
{
	playerx = x;
	playery = y;
}

void Player::moveplayer(int x, int y)
{
	playerx += x;
	playery += y;
}

void Player::MoveControl(Map *map, DirectInput *pDi, int &scrollwidth, int &scrollheight, int viewX, int viewY)
{
	if (scrollwidth%map->MAPCIPSIZE == 0 && scrollheight%map->MAPCIPSIZE == 0)
	{
		if (pDi->KeyState(DIK_UP) && (map->CallMap(CallPlayerX() / map->MAPCIPSIZE, CallPlayerY() / map->MAPCIPSIZE - 1) == map->FLOOR ||
			map->CallMap(CallPlayerX() / map->MAPCIPSIZE, CallPlayerY() / map->MAPCIPSIZE - 1) == map->STAIR))
		{
			playerMoveY = -MOVESPEED;
			moveY = true;
			justInput = true;
		}
		if (pDi->KeyState(DIK_DOWN) && (map->CallMap(CallPlayerX() / map->MAPCIPSIZE, CallPlayerY() / map->MAPCIPSIZE + 1) == map->FLOOR ||
			map->CallMap(CallPlayerX() / map->MAPCIPSIZE, CallPlayerY() / map->MAPCIPSIZE + 1) == map->STAIR))
		{
			playerMoveY = MOVESPEED;
			moveY = true;
			justInput = true;
		}
		if (pDi->KeyState(DIK_LEFT) && (map->CallMap(CallPlayerX() / map->MAPCIPSIZE - 1, CallPlayerY() / map->MAPCIPSIZE) == map->FLOOR ||
			map->CallMap(CallPlayerX() / map->MAPCIPSIZE - 1, CallPlayerY() / map->MAPCIPSIZE) == map->STAIR))
		{
			playerMoveX = -MOVESPEED;
			moveX = true;
			justInput = true;
		}
		if (pDi->KeyState(DIK_RIGHT) && (map->CallMap(CallPlayerX() / map->MAPCIPSIZE + 1, CallPlayerY() / map->MAPCIPSIZE) == map->FLOOR ||
			map->CallMap(CallPlayerX() / map->MAPCIPSIZE + 1, CallPlayerY() / map->MAPCIPSIZE) == map->STAIR))
		{
			playerMoveX = MOVESPEED;
			moveX = true;
			justInput = true;
		}
	}
	if (moveX == true && moveY == true && justInput == true)
	{
		if ((map->CallMap(CallPlayerX() / map->MAPCIPSIZE + playerMoveX / MOVESPEED, CallPlayerY() / map->MAPCIPSIZE + playerMoveY / MOVESPEED) != map->FLOOR &&
			map->CallMap(CallPlayerX() / map->MAPCIPSIZE + playerMoveX / MOVESPEED, CallPlayerY() / map->MAPCIPSIZE + playerMoveY / MOVESPEED) != map->STAIR))
		{
			playerMoveX = 0;
			playerMoveY = 0;
			moveX = false;
			moveY = false;
		}
	}
	if (moveX == true || moveY == true)
	{
		moveplayer(playerMoveX, playerMoveY);
		scrollwidth = -CallPlayerX() - map->MAPCIPSIZE / 2 + viewX;
		scrollheight = -CallPlayerY() - map->MAPCIPSIZE / 2 + viewY;
		movecount++;
		if (justInput == true)
		{
			justInput = false;
		}
		if (movecount == map->MAPCIPSIZE / MOVESPEED)
		{
			playerMoveX = 0;
			playerMoveY = 0;
			moveX = false;
			moveY = false;
			movecount = 0;
			if (map->CallMap(CallPlayerX() / map->MAPCIPSIZE, CallPlayerY() / map->MAPCIPSIZE) == map->STAIR && map->CallNextStair() == false)
			{
				map->trueNextStage();
			}
			else
			{
				map->falseNextStage();
			}
		}
	}
	map->KnowMiniMap(this);
}