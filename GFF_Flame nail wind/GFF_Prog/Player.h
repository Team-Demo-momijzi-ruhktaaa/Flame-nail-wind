#pragma once

class player 
{
private:
	//プレイヤーのX座標
	int playerx;
	//プレイヤーのY座標
	int playery;

public:

	player();
	~player();

	void moveplayer(int x, int y);
	//void showplayer();

};