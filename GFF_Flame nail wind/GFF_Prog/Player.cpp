#include"Player.h"
#include"DirectInput.h"



player::player()
{
	playerx = 0;
	playery = 0;
}

player::~player()
{
}

void player::moveplayer(int x, int y)
{
	playerx += x;
	playery += y;
}