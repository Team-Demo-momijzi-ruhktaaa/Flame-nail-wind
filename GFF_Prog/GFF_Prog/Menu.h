#pragma once

class Menu
{
private:
	//���݃��j���[���J���Ă��邩
	bool menuOpen;
	//�J�[�\���̑I���ʒu
	int cursorSelect;
public:
	Menu();
	~Menu();
	void CleateMenu();
	void ClauseMenu();
	int CallMenuOpen() { return menuOpen; }
};