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
			//部屋があったら１以上なのでタイル数加算
			if (map.CallpMapRoomData(x, y) != 0)
			{
				eFloor++;
			}
			//初期化
			pCharData[x + y * mw] = 0;
		}
	}
	intialEnemy = eFloor / 16;//マップに出てくる最初の敵の数
	eSpownPoint = 10;//仮
	maxEnemy = mRooms * 6;
	minEnemy = mRooms * 1;
}
//----------------------------------------------------------------------------------
//各エネミーのステータスを作成
void EnemyManager::SetEStatus(int mw, int mh, int mRooms)
{
	//配列を動的生成
	pEnemyData = new struct enemys[maxEnemy];

	//配列の初期化
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
	//------------------------------------------------この手前全敵の初期化となっており。。
	
	

	//敵を生成する部屋を模索 ついでにキャラも決定
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

	//---------------------------ここで分けることも可能
	//部屋の中の座標設定
	for (int i = 0; i < intialEnemy; i++)
	{
		Map::rooms room =
			map.GetRoomInfo(pEnemyData[i].enemyRooms);
		while (pEnemyData[i].enemyRoomflag != false)
		{
			//部屋のなかでスポーンする場所を選択
			pEnemyData[i].enemyPx = rand() % room.roomWidth;
			pEnemyData[i].enemyPy = rand() % room.roomHeight;
			//全体のマップのなかでそこになにもないことを確認　格納
			if (map.CallMap((pEnemyData[i].enemyPx + room.roomX),
				(pEnemyData[i].enemyPy + room.roomY)) == 0)
			{
				//ここでキャラデータ内でそこに何もいないか確認
				if (pCharData[(pEnemyData[i].enemyPx + room.roomX) +
					(pEnemyData[i].enemyPy + room.roomY) * mw] == 0)
				{
					//はい幸せついに爆誕
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
//ターン経過移動できるかどうか

void EnemyManager::moveEnemy(int x, int y)
{


}

//---------------------------------------------------------------------------------------------
void EnemyManager::cleateEnemy(int mw, int mh, int mRooms)
{
	//敵を生成する部屋を模索
	currentEnemy++;

	pEnemyData[currentEnemy].enemyRooms = rand() % mRooms + 1;
	pEnemyData[currentEnemy].enemyRoomflag = true;

	Map::rooms room =
		map.GetRoomInfo(pEnemyData[currentEnemy].enemyRooms);
	while (pEnemyData[currentEnemy].enemyRoomflag != false)
	{
		//部屋のなかでスポーンする場所を選択
		pEnemyData[currentEnemy].enemyPx = rand() % room.roomWidth;
		pEnemyData[currentEnemy].enemyPy = rand() % room.roomHeight;
		//全体のマップのなかでそこになにもないことを確認　格納
		if (map.CallMap((pEnemyData[currentEnemy].enemyPx + room.roomX),
			(pEnemyData[currentEnemy].enemyPy + room.roomY)) == 0)
		{
			//ここでキャラデータ内でそこに何もいないか確認
			if (pCharData[(pEnemyData[currentEnemy].enemyPx + room.roomX) +
				(pEnemyData[currentEnemy].enemyPy + room.roomY) * mw] == 0)
			{
				//はい幸せついに爆誕
				pEnemyData[currentEnemy].enemyRoomflag = false;
				pCharData[(pEnemyData[currentEnemy].enemyPx + room.roomX) +
					(pEnemyData[currentEnemy].enemyPy + room.roomY) * mw]
					= pEnemyData[currentEnemy].enemyName;
			}
		}
	}
}