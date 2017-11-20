#pragma once


class EnemyManager
{
private:

	//初期のスポーンする敵
	int intialEnemy;
	//エネミーの出現最大数と最小数(0にはならない)
	int maxEnemy;
	int minEnemy;
	//上の数から乱数で決定　この数からスポーン確率を決める
	int spawnEnemy;
	//現在の出現数
	int currentEnemy;
	//次にスポーンするまでのポイント
	int eSpownPoint;
	//スポーン用に部屋のある場所の床の量
	int eFloor;

	bool flag;

	struct enemys
	{
		//現在座標
		int enemyPx;
		int enemyPy;
		//名前
		int enemyName;
		//移動用
		int enemyMoveX;
		int enemyMoveY;
		//スポーンする部屋　また現在の部屋
		int enemyRooms;
		bool enemyRoomflag;
	};
public:

	EnemyManager();
	~EnemyManager();

	//マップ生成時の出現する敵の数決定する関数　マップ生成の最後に呼ぶべし
	void SetEnemy(int mx, int my, int mRooms);

	//各敵の座標を設定　上の後に呼べーい
	void SetEStatus(int mw, int mh, int mRooms);

	//敵の移動
	void moveEnemy(int x, int y);

	//新しく敵を生成
	void cleateEnemy(int mw, int mh, int mRooms);


	int callCharData(int width, int height, int mapWidth) { return pCharData[width + height * mapWidth]; }

private:
	//敵の構造体を格納するポインタ
	enemys* pEnemyData;

	int* pCharData;


};


