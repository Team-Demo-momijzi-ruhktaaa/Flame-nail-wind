#pragma once

class Map;
class DirectInput;

class Player 
{
private:
	//プレイヤーのX座標
	int playerx;
	//プレイヤーのY座標
	int playery;

	int playerMoveX;
	int playerMoveY;
	bool moveX;
	bool moveY;
	int movecount;
	bool justInput;
	const int MOVESPEED;

public:

	Player();
	~Player();

	void SetPlayer(int x, int y);
	void moveplayer(int x, int y);
	void MoveControl(Map *map, DirectInput *pDi, int &scrollwidth, int &scrollheight, int viewX,int viewY);

	//プレイヤーの座標呼び出し
	int CallPlayerX() { return playerx; }
	int CallPlayerY() { return playery; }
	//void showplayer();

};