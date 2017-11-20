#include "Menu.h"

Menu::Menu()
{
	menuOpen = false;
	cursorSelect = 0;
}

Menu::~Menu()
{

}

void Menu::CleateMenu()
{
	menuOpen = true;
}

void Menu::ClauseMenu()
{
	menuOpen = false;
}