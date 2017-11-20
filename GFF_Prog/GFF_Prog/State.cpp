#include"State.h"


State::State() :HP_UP(1), MP_UP(1), ATK_UP(1), DEF_UP(1)
{

}
State::State(int hpup, int mpup, int atkup, int defup) :
	HP_UP(hpup), MP_UP(mpup), ATK_UP(atkup), DEF_UP(defup)
{
	Lv = 1;
	EXP = 0;
	HP = 0;
	MP = 0;
	ATK = 0;
	DEF = 0;
	SPEED = 0;
}
State::~State()
{
	Lv = 1;
	EXP = 2;
	HP = 5;
	MP = 5;
	ATK = 5;
	DEF = 5;
	SPEED = 8;
}
Enemy1::Enemy1(int hpup, int mpup, int atkup, int defup) :State(1, 0, 1, 1)
{
	Lv = 1;
	EXP = 3;
	HP = 6;
	MP = 0;
	ATK = 2;
	DEF = 0;
	SPEED = 8;
}
Enemy1::~Enemy1()
{

}

Enemy2::Enemy2(int hpup, int mpup, int atkup, int defup) :State(2, 2, 2, 2)
{
	Lv = 1;
	EXP = 10;
	HP = 15;
	MP = 0;
	ATK = 5;
	DEF = 3;
	SPEED = 8;
}
Enemy2::~Enemy2()
{

}

PlayerState::PlayerState(int hpup, int mpup, int atkup, int defup) :State(1, 1, 1, 1)
{
	Lv = 1;
	EXP = 2;
	HP = 4;
	MP = 0;
	ATK = 2;
	DEF = 3;
	SPEED = 3;
}

