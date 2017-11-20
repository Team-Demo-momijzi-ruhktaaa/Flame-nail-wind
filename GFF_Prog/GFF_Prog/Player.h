#pragma once

class Map;
class DirectInput;

class Player 
{
private:
	//�v���C���[��X���W
	int playerx;
	//�v���C���[��Y���W
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

	//�v���C���[�̍��W�Ăяo��
	int CallPlayerX() { return playerx; }
	int CallPlayerY() { return playery; }
	//void showplayer();

};