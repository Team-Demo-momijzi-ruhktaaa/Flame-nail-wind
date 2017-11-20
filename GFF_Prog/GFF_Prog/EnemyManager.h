#pragma once


class EnemyManager
{
private:

	//�����̃X�|�[������G
	int intialEnemy;
	//�G�l�~�[�̏o���ő吔�ƍŏ���(0�ɂ͂Ȃ�Ȃ�)
	int maxEnemy;
	int minEnemy;
	//��̐����痐���Ō���@���̐�����X�|�[���m�������߂�
	int spawnEnemy;
	//���݂̏o����
	int currentEnemy;
	//���ɃX�|�[������܂ł̃|�C���g
	int eSpownPoint;
	//�X�|�[���p�ɕ����̂���ꏊ�̏��̗�
	int eFloor;

	bool flag;

	struct enemys
	{
		//���ݍ��W
		int enemyPx;
		int enemyPy;
		//���O
		int enemyName;
		//�ړ��p
		int enemyMoveX;
		int enemyMoveY;
		//�X�|�[�����镔���@�܂����݂̕���
		int enemyRooms;
		bool enemyRoomflag;
	};
public:

	EnemyManager();
	~EnemyManager();

	//�}�b�v�������̏o������G�̐����肷��֐��@�}�b�v�����̍Ō�ɌĂԂׂ�
	void SetEnemy(int mx, int my, int mRooms);

	//�e�G�̍��W��ݒ�@��̌�ɌĂׁ[��
	void SetEStatus(int mw, int mh, int mRooms);

	//�G�̈ړ�
	void moveEnemy(int x, int y);

	//�V�����G�𐶐�
	void cleateEnemy(int mw, int mh, int mRooms);


	int callCharData(int width, int height, int mapWidth) { return pCharData[width + height * mapWidth]; }

private:
	//�G�̍\���̂��i�[����|�C���^
	enemys* pEnemyData;

	int* pCharData;


};


