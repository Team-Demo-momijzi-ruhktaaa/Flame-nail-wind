#pragma once

//�v���C���[�̃p�����[�^
/*
���x���@LV
�o���l�@EXP

�㏸�l�@�\�͖�_UP

�̗́@HP
���́@MP
�U���@ATK
�h��@DEF
//���U�@���
//���h�@���

�X�e�[�^�X���
�K���Z��
�w�͒l����邩�H
�Ⴆ�ΓG�L������EXP�ȊO�ɓ���o���l��t����
���̃��x���A�b�v�܂łɂ��܂�������o���l����ԑ����p�����[�^��+1(HP��+3)
(����o���l�������Ƃ��̓����_���A�Ⴆ��HP��10,MP��10,ATK��10�Ń��x���A�b�v��������HP,MP,ATK�̂ǂꂩ����㏸)
����̗��_��LV100(MAX)�ɂȂ�����͌o���l�����܂邲�Ƃɓ���o���l�̃X�e�[�^�X���Z�����ɂ����
�X�Ȃ��肱�ݗv�f�ɂȂ�
���_�͓����G�΂����|���Γ����^�̃X�e�[�^�X�ɂȂ�̂Ńo�����X�������\��������
*/

//�S�ẴX�e�[�^�X�̊�b�N���X
class State
{
protected:

	int Lv;
	int EXP;
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPEED;//8���ʏ푬�x //�{���@�S�@�O�{�H�@���S�{��
	const int HP_UP;
	const int MP_UP;
	const int ATK_UP;
	const int DEF_UP;

public:
	State();
	State(int hpup, int mpup, int atkup, int defup);
	virtual~State();
};

//�G�l�~�[�̃X�e�[�^�X
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
