#include "Map.h"
#include "time.h"
#include "windows.h"
//コンストラクタ
Map::Map() :MAPCIPSIZE(64), FLOOR(0), WALL(1), FIXWALL(-1)
{
	mapHeightMax = 0;
	mapHeightMin = 0;
	mapHeight = 0;
	mapWidthMax = 0;
	mapWidthMin = 0;
	mapWidth = 0;
	mapSize = 0;
	cleateRoomNum = 0;
	roomNum = 0;
}
//デストラクタ
Map::~Map()
{
	delete[] pMapData;
	delete[] pRoomNum;
	delete[] pCleateRooms;
}

void Map::CleateMap(int width, int height)
{
	srand((unsigned int)time(NULL));

	mapHeightMax = rand() % height;//0～height
	mapHeightMin = 20;//仮
	mapHeight = mapHeightMax + mapHeightMin;//マップ全体の大きさ(Y）
	mapWidthMax = rand() % width;//0～width
	mapWidthMin = 20;//仮
	mapWidth = mapWidthMax + mapWidthMin;//マップ全体の大きさ（X）
	mapSize = mapHeight*mapWidth;//配列の要素数決定
	pMapData = new int[mapSize];//配列を動的に確保
	roomNum = rand() % 18 + 2;//2～12こ部屋を作成
	pRoomNum = new struct rooms[roomNum];//部屋の総数

	//壊されない壁
	for (int i = 0; i < mapSize; i++)
	{
		if (i<mapWidth || i>mapSize - mapWidth || i%mapWidth == 0 || i%mapWidth == mapWidth - 1)
		{
			pMapData[i] = FIXWALL;
		}
		else
		{
			pMapData[i] = WALL;
		}
	}
	//部屋を作る分だけ初期化
	for (int i = 0; i < roomNum; i++)
	{
		pRoomNum[i].roomHeight = 0;
		pRoomNum[i].roomWidth = 0;
		pRoomNum[i].roomX = 0;
		pRoomNum[i].roomY = 0;
		pRoomNum[i].cleateNum = 0;
	}
	//部屋生成
	for (int i = 0; i < roomNum; i++)
	{
		int rx = rand() % (mapWidth - 6) + 1;//+1は右下のマップ生成のために必須無かったら欠けてしまう
		int ry = rand() % (mapHeight - 6) + 1;
		int rw = rand() % 5 + 5;
		int rh = rand() % 5 + 5;

		//壊せない壁がなければ描画（左上角が支点でそこが描画できれば重なろうが描画する）
		if (pMapData[(rx - 1) + (ry - 1)*mapWidth] != FLOOR &&
			pMapData[(rx - 1) + (ry + rh + 1)*mapWidth] != FLOOR &&
			pMapData[(rx + rw + 1) + (ry - 1)*mapWidth] != FLOOR &&
			pMapData[(rx + rw + 1) + (ry + rh + 1)*mapWidth] != FLOOR &&
			pMapData[rx + rw / 2 + (ry - 1)*mapWidth] != FLOOR &&
			pMapData[rx + rw / 2 + (ry + rh + 1)*mapWidth] != FLOOR &&
			pMapData[(rx - 1) + (ry + rh / 2)*mapWidth] != FLOOR &&
			pMapData[(rx + rw + 1) + (ry + rh / 2)*mapWidth] != FLOOR &&
			pMapData[rx + rw / 2 + (ry + rh / 2)*mapWidth] != FLOOR &&
			rx + rw < mapWidth && ry + rh < mapHeight /*&&*/
			/*cleateRoomNum  != 5*/)
		{
			CleateRoom(&pRoomNum[i], rx, ry, rw, rh);
			cleateRoomNum++;//部屋に番号振り　部屋生成につき+１
			SetCleateNum(&pRoomNum[i], cleateRoomNum);
		}
	}
	
	pCleateRooms = new struct rooms[cleateRoomNum];

	//初期化
	for (int i = 0; i < cleateRoomNum; i++)
	{
		pCleateRooms[i].cleateNum = 0;
		pCleateRooms[i].roomHeight = 0;
		pCleateRooms[i].roomWidth = 0;
		pCleateRooms[i].roomX = 0;
		pCleateRooms[i].roomY = 0;
	}

	//いらないものをポイ捨て
	for (int i = 0; i < roomNum; i++)
	{
		if (pRoomNum[i].cleateNum != 0)
		{
			pCleateRooms[pRoomNum[i].cleateNum - 1] = pRoomNum[i];
		}
	}

	//部屋生成数が0か1の時
	//大部屋生成
	if (cleateRoomNum == 0 || cleateRoomNum == 1)
	{
		CleateBigRoom();
	}
	else if(cleateRoomNum >= 2)
	{
		//道作成
		int roadX1 = 0;//道が出る部屋の中の基準地点始まり（乱数のため場所は不明）
		int roadY1 = 0;
		int roadX0 = 0;//たどり着く部屋の道の基準地点終点
		int roadY0 = 0;
		int cleateRoadX = 0;//道の折り返し地点のための変数
		int cleateRoadY = 0;
		int roadXDelta = 0;//roadX1とroadX0 の位置の差の絶対値（正の数になる）
		int roadYDelta = 0;//roadY1とroadY0 の位置の差の絶対値（正の数になる）
		for (int i = 1; i < cleateRoomNum; i++)
		{
			roadX1 = rand() % pCleateRooms[i].roomWidth + pCleateRooms[i].roomX;
			roadY1 = rand() % pCleateRooms[i].roomHeight + pCleateRooms[i].roomY;
			roadX0 = rand() % pCleateRooms[i - 1].roomWidth + pCleateRooms[i - 1].roomX;
			roadY0 = rand() % pCleateRooms[i - 1].roomHeight + pCleateRooms[i - 1].roomY;
			//道描画の処理

			//道の差の絶対値取得（X）
			if (roadX1>roadX0)
			{
				cleateRoadX = -1;
				roadXDelta = roadX1 - roadX0;
			}
			else if (roadX1<roadX0)
			{
				cleateRoadX = 1;
				roadXDelta = roadX0 - roadX1;
			}
			//道の差の絶対値取得（Y）
			if (roadY1>roadY0)
			{
				cleateRoadY = -1;
				roadYDelta = roadY1 - roadY0;
			}
			else if (roadY1<roadY0)
			{
				cleateRoadY = 1;
				roadYDelta = roadY0 - roadY1;
			}

			if (roadXDelta == 0 || roadYDelta == 0)
			{
				//差分だけループ（片方が０なのは確定なので直線の通路が完成なり）
				int Delta = roadXDelta + roadYDelta;
				for (int i = 0; i < Delta; i++)
				{
					pMapData[roadX1 + (i*cleateRoadX) + ((i*cleateRoadY) + roadY1)*mapWidth] = FLOOR;
				}
			}
			else if (roadXDelta >= roadYDelta)
			{
				for (int i = 0; i < roadXDelta / 2; i++)
				{
					pMapData[roadX1 + (i*cleateRoadX) + roadY1 * mapWidth] = FLOOR;
				}
				for (int i = 0; i < roadYDelta; i++)
				{
					pMapData[roadX1 + (roadXDelta / 2 * cleateRoadX) + (i*cleateRoadY + roadY1) * mapWidth] = FLOOR;
				}
				for (int i = 0; i < roadXDelta / 2 + roadXDelta % 2; i++)
				{
					pMapData[roadX1 + ((roadXDelta / 2 + i)*cleateRoadX) + (roadYDelta*cleateRoadY + roadY1) * mapWidth] = FLOOR;
				}
			}
			else
			{
				for (int i = 0; i < roadYDelta / 2; i++)
				{
					pMapData[roadX1 + (i*cleateRoadY + roadY1) * mapWidth] = FLOOR;
				}
				for (int i = -0; i < roadXDelta; i++)
				{
					pMapData[roadX1 + (i*cleateRoadX) + ((roadYDelta / 2)*cleateRoadY + roadY1) * mapWidth] = FLOOR;
				}
				for (int i = 0; i < roadYDelta / 2 + roadYDelta % 2; i++)
				{
					pMapData[roadX1 + (roadXDelta*cleateRoadX) + ((roadYDelta / 2 + i)*cleateRoadY  + roadY1) * mapWidth] = FLOOR;
				}
			}
			
		}
	}
}

//部屋作成
void Map::CleateRoom(struct rooms *room, int x, int y, int width, int height)
{
	room->roomX = x;
	room->roomY = y;
	room->roomWidth = width;
	room->roomHeight = height;
	for (int i = 0; i < room->roomHeight; i++)
	{
		for (int j = 0; j <room->roomWidth; j++)
		{
			pMapData[j + room->roomX + (i + room->roomY)*mapWidth] = FLOOR;
		}
	}
}

//
void Map::CleateBigRoom()
{
	for (int i = 0; i < mapSize; i++)
	{
		if (i>mapWidth && i<mapSize - mapWidth && i%mapWidth != 0 && i%mapWidth != mapWidth - 1)
		{
			pMapData[i] = FLOOR;
		}
	}
}