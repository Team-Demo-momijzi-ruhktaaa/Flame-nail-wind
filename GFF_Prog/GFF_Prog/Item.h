#pragma once

//�A�C�e���ɟC���i�v�f
/*
�l�ivalue
����state
���Oname
�摜graphic
������num
�\�[�g�p�̃i���o�[sortnum
*/

//��{�N���X
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