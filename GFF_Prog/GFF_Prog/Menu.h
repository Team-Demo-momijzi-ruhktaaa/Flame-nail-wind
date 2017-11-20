#pragma once

class Menu
{
private:
	//現在メニューが開いているか
	bool menuOpen;
	//カーソルの選択位置
	int cursorSelect;
public:
	Menu();
	~Menu();
	void CleateMenu();
	void ClauseMenu();
	int CallMenuOpen() { return menuOpen; }
};