#pragma once

class player 
{
private:
	//�v���C���[��X���W
	int playerx;
	//�v���C���[��Y���W
	int playery;

public:

	player();
	~player();

	void moveplayer(int x, int y);
	//void showplayer();

};