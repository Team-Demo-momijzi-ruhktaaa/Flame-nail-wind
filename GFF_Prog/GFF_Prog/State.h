#pragma once

//プレイヤーのパラメータ
/*
レベル　LV
経験値　EXP

上昇値　能力名_UP

体力　HP
魔力　MP
攻撃　ATK
防御　DEF
//魔攻　後に
//魔防　後に

ステータス状態
習得技種
努力値を作るか？
例えば敵キャラにEXP以外に特殊経験値を付けて
次のレベルアップまでにたまった特殊経験値が一番多いパラメータに+1(HPは+3)
(特殊経験値が同じときはランダム、例えばHPに10,MPに10,ATKに10でレベルアップした時はHP,MP,ATKのどれか一つが上昇)
これの利点はLV100(MAX)になった後は経験値がたまるごとに特殊経験値のステータス加算だけにすれば
更なるやりこみ要素になる
欠点は同じ敵ばっか倒せば特化型のステータスになるのでバランスが崩れる可能性がある
*/

//全てのステータスの基礎クラス
class State
{
protected:

	int Lv;
	int EXP;
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPEED;//8が通常速度 //倍速　４　三倍？　次４倍ｗ
	const int HP_UP;
	const int MP_UP;
	const int ATK_UP;
	const int DEF_UP;

public:
	State();
	State(int hpup, int mpup, int atkup, int defup);
	virtual~State();
};

//エネミーのステータス
class Enemy1 :public State
{
private:

public:
	Enemy1(int hpup, int mpup, int atkup, int defup);
	~Enemy1();

};

class Enemy2 :public State
{
private:

public:
	Enemy2(int hpup, int mpup, int atkup, int defup);
	~Enemy2();

};


class PlayerState :public State
{
private:

public:
	PlayerState(int hpup, int mpup, int atkup, int defup);
	~PlayerState();

};
