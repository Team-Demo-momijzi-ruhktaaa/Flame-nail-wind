#pragma once

//アイテムに櫃ヨナ要素
/*
値段value
効果state
名前name
画像graphic
所持個数num
ソート用のナンバーsortnum
*/

//基本クラス
class Item
{
protected:
	int itemValue;
	int itemState;
	char *itemName;
	int itemGraphic;
	int num;
	int sortnum;

public:
	Item();
	~Item();


};

class Item_heal_base : public virtual Item
{
	void itemEffect_heal(int effectnum);
};