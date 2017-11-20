#include"EnemyManager.h"
#include"Map.h"
#include"Player.h"
#include "time.h"
#include "windows.h"
#include"State.h"

EnemyManager::EnemyManager()
{
	intialEnemy = 0;
	maxEnemy = 0;
	minEnemy = 0;
	currentEnemy = 0;
	spawnEnemy = 0;
	eFloor = 0;
	eSpownPoint = 0;
	flag = false;
}

EnemyManager::~EnemyManager()
{

}
Map map;
Player player;

//----------------------------------------------------------------------------
void EnemyManager::SetEnemy(int mw, int mh, int mRooms)
{
	pCharData = new int[mw * mh];

	for (int y = 0; y < mh; y++)
	{
		for (int x = 0; x < mw; x++)
		{
			//��������������P�ȏ�Ȃ̂Ń^�C�������Z
			if (map.CallpMapRoomData(x, y) != 0)
			{
				eFloor++;
			}
			//������
			pCharData[x + y * mw] = 0;
		}
	}
	intialEnemy = eFloor / 16;//�}�b�v�ɏo�Ă���ŏ��̓G�̐�
	eSpownPoint = 10;//��
	maxEnemy = mRooms * 6;
	minEnemy = mRooms * 1;
}
//----------------------------------------------------------------------------------
//�e�G�l�~�[�̃X�e�[�^�X���쐬
void EnemyManager::SetEStatus(int mw, int mh, int mRooms)
{
	//�z��𓮓I����
	pEnemyData = new struct enemys[maxEnemy];

	//�z��̏�����
	for (int i = 0; i < maxEnemy; i++)
	{
		pEnemyData[i].enemyMoveX = 0;
		pEnemyData[i].enemyMoveY = 0;
		pEnemyData[i].enemyName = 0;
		pEnemyData[i].enemyPx = 0;
		pEnemyData[i].enemyPy = 0;
		pEnemyData[i].enemyRooms = 0;
		pEnemyData[i].enemyRoomflag = false;
	}
	//------------------------------------------------���̎�O�S�G�̏������ƂȂ��Ă���B�B
	
	

	//�G�𐶐����镔����͍� ���łɃL����������
	for (int i = 0; i < intialEnemy; i++)
	{
		while (pEnemyData[i].enemyRoomflag != true)
		{
			pEnemyData[i].enemyRooms = rand() % mRooms + 1;
			if (pEnemyData[i].enemyRooms
				= pEnemyData[i - 1].enemyRooms &&i != 0)
			{
				pEnemyData[i].enemyRoomflag = true;
			}
			if (flag != true)
			{
				flag = true;
				pEnemyData[i].enemyName = 1;
			}
			else
			{
				flag = false;
				pEnemyData[i].enemyName = 2;
			}
		}
	}

	//---------------------------�����ŕ����邱�Ƃ��\
	//�����̒��̍��W�ݒ�
	for (int i = 0; i < intialEnemy; i++)
	{
		Map::rooms room =
			map.GetRoomInfo(pEnemyData[i].enemyRooms);
		while (pEnemyData[i].enemyRoomflag != false)
		{
			//�����̂Ȃ��ŃX�|�[������ꏊ��I��
			pEnemyData[i].enemyPx = rand() % room.roomWidth;
			pEnemyData[i].enemyPy = rand() % room.roomHeight;
			//�S�̂̃}�b�v�̂Ȃ��ł����ɂȂɂ��Ȃ����Ƃ��m�F�@�i�[
			if (map.CallMap((pEnemyData[i].enemyPx + room.roomX),
				(pEnemyData[i].enemyPy + room.roomY)) == 0)
			{
				//�����ŃL�����f�[�^���ł����ɉ������Ȃ����m�F
				if (pCharData[(pEnemyData[i].enemyPx + room.roomX) +
					(pEnemyData[i].enemyPy + room.roomY) * mw] == 0)
				{
					//�͂��K�����ɔ��a
					pEnemyData[i].enemyRoomflag = false;
					pCharData[(pEnemyData[i].enemyPx + room.roomX) + (pEnemyData[i].enemyPy + room.roomY) * mw]
						= pEnemyData[i].enemyName;
				}
			}
		}
	}
	currentEnemy = intialEnemy;
}
//------------------------------------------------------------------------------------------
//�^�[���o�߈ړ��ł��邩�ǂ���

void EnemyManager::moveEnemy(int x, int y)
{


}

//---------------------------------------------------------------------------------------------
void EnemyManager::cleateEnemy(int mw, int mh, int mRooms)
{
	//�G�𐶐����镔����͍�
	currentEnemy++;

	pEnemyData[currentEnemy].enemyRooms = rand() % mRooms + 1;
	pEnemyData[currentEnemy].enemyRoomflag = true;

	Map::rooms room =
		map.GetRoomInfo(pEnemyData[currentEnemy].enemyRooms);
	while (pEnemyData[currentEnemy].enemyRoomflag != false)
	{
		//�����̂Ȃ��ŃX�|�[������ꏊ��I��
		pEnemyData[currentEnemy].enemyPx = rand() % room.roomWidth;
		pEnemyData[currentEnemy].enemyPy = rand() % room.roomHeight;
		//�S�̂̃}�b�v�̂Ȃ��ł����ɂȂɂ��Ȃ����Ƃ��m�F�@�i�[
		if (map.CallMap((pEnemyData[currentEnemy].enemyPx + room.roomX),
			(pEnemyData[currentEnemy].enemyPy + room.roomY)) == 0)
		{
			//�����ŃL�����f�[�^���ł����ɉ������Ȃ����m�F
			if (pCharData[(pEnemyData[currentEnemy].enemyPx + room.roomX) +
				(pEnemyData[currentEnemy].enemyPy + room.roomY) * mw] == 0)
			{
				//�͂��K�����ɔ��a
				pEnemyData[currentEnemy].enemyRoomflag = false;
				pCharData[(pEnemyData[currentEnemy].enemyPx + room.roomX) +
					(pEnemyData[currentEnemy].enemyPy + room.roomY) * mw]
					= pEnemyData[currentEnemy].enemyName;
			}
		}
	}
}