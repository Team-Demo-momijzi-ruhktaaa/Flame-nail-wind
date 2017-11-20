#include "Map.h"
#include "time.h"
#include "windows.h"
#include "Player.h"

Map::Map() :MAPCIPSIZE(16), FLOOR(0), WALL(1), FIXWALL(-1), STAIR(2), MINIMAPSIZE(8)
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
	floorNum = 0;
	downStairX = 0;
	downStairY = 0;
	cleatePlayerRoom = 0;
	changeNextStair = false;
}

Map::~Map()
{
	Release();
}

void Map::Release()
{
	delete[] pMapData;
	pMapData = nullptr;

	delete[] pRoomNum;
	pRoomNum = nullptr;

	delete[] pCleateRooms;
	pCleateRooms = nullptr;

	delete[] pMapRoomData;
	pMapRoomData = nullptr;

	delete[] pRoadData;
	pRoadData = nullptr;

	delete[] pMiniMapData;
	pMiniMapData = nullptr;
}

//部屋生成
void Map::CleateRoom(struct rooms *room, int x, int y, int width, int height)
{
	int mapnumdata = 0;
	room->roomX = x;
	room->roomY = y;
	room->roomWidth = width;
	room->roomHeight = height;
	for (int i = 0; i < room->roomHeight; i++)
	{
		for (int j = 0; j <room->roomWidth; j++)
		{
			mapnumdata = j + room->roomX + (i + room->roomY)*mapWidth;
			pMapData[mapnumdata] = FLOOR;
		}
	}
}

//マップ生成
void Map::CleateMap(int width, int height)
{

	srand((unsigned int)time(NULL));

	mapHeightMax = rand() % height;
	mapHeightMin = 50;//仮
	mapHeight = mapHeightMax + mapHeightMin;
	mapWidthMax = rand() % width;
	mapWidthMin = 50;//仮
	mapWidth = mapWidthMax + mapWidthMin;
	mapSize = mapHeight*mapWidth;
	pMapData = new int[mapSize];
	roomNum = rand() % 30 + 20;//仮
	pRoomNum = new struct rooms[roomNum];

	//最初に確保したマップを壁で埋める
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

	//部屋の想定最大数を初期化する
	for (int i = 0; i < roomNum; i++)
	{
		pRoomNum[i].cleateNum = 0;
		pRoomNum[i].roomHeight = 0;
		pRoomNum[i].roomWidth = 0;
		pRoomNum[i].roomX = 0;
		pRoomNum[i].roomY = 0;
	}

	//部屋が置けるかどうかの判別
	for (int i = 0; i < roomNum; i++)
	{
		int rx = rand() % (mapWidth - 6) + 1;
		int ry = rand() % (mapHeight - 6) + 1;
		int w = rand() % 5 + 5;
		int h = rand() % 5 + 5;
		bool roomflag = true;
		if ((rx + w) + (ry + h)*mapWidth < mapSize)
		{
			if (rx + w < mapWidth && ry + h < mapHeight)
			{
				for (int i = -1; i < w + 1; i++)
				{
					for (int j = -1; j < h + 1; j++)
					{
						if (pMapData[(rx + i) + (ry + j)*mapWidth] == FLOOR)
						{
							//部屋が生成条件を満たさない
							roomflag = false;
						}
					}
				}
				//部屋生成成功
				if (roomflag == true)
				{
					cleateRoomNum++;
					SetCleateNum(&pRoomNum[i], cleateRoomNum);
					CleateRoom(&pRoomNum[i], rx, ry, w, h);
					roomflag = false;
				}
			}
		}
	}

	//生成された部屋の数だけ動的に配列取得
	pCleateRooms = new struct rooms[cleateRoomNum];

	//生成された部屋の初期化
	for (int i = 0; i < cleateRoomNum; i++)
	{
		pCleateRooms[i].cleateNum = 0;
		pCleateRooms[i].roomHeight = 0;
		pCleateRooms[i].roomWidth = 0;
		pCleateRooms[i].roomX = 0;
		pCleateRooms[i].roomY = 0;
	}

	//部屋のデータの振り直し
	for (int i = 0; i < roomNum; i++)
	{
		if (pRoomNum[i].cleateNum != 0)
		{
			pCleateRooms[pRoomNum[i].cleateNum - 1] = pRoomNum[i];
		}
	}

	//マップと同じ大きさの動的配列
	pMapRoomData = new int[mapSize];

	for (int i = 0; i < mapSize; i++)
	{
		pMapRoomData[i] = 0;
	}

	//部屋番号を配列に振る
	for (int i = 0; i < roomNum; i++)
	{
		if (pRoomNum[i].cleateNum != 0)
		{
			for (int n = 0; n < pRoomNum[i].roomHeight; n++)
			{
				for (int m = 0; m < pRoomNum[i].roomWidth; m++)
				{
					pMapRoomData[m + pRoomNum[i].roomX + (n + pRoomNum[i].roomY)*mapWidth] = pRoomNum[i].cleateNum;
				}
			}
		}
	}


	//部屋生成数が0か1の時
	//大部屋生成
	if (cleateRoomNum == 0 || cleateRoomNum == 1)
	{
		CleateBigRoom();
		downStairX = rand() % (mapWidth - 2) + 1;
		downStairY = rand() % (mapHeight - 2) + 1;
		pMapData[downStairX + downStairY*mapWidth] = STAIR;
		cleatePlayerRoom = -1;
	}
	//通常の部屋生成
	else if(cleateRoomNum >= 2)
	{
		int roadX1 = 0;//基準になる部屋の中の道の基準になるX地点
		int roadY1 = 0;//基準になる部屋の中の道の基準になるY地点
		int roadX0 = 0;//道の目的になる部屋の中の道の基準になるX地点
		int roadY0 = 0;//道の目的になる部屋の中の道の基準になるY地点
		int cleateRoadX = 0;//差によってどちらに動くか
		int cleateRoadY = 0;//差によってどちらに動くか
		int roadXDelta = 0;//差、常に正になる
		int roadYDelta = 0;//差、常に正になる
		for (int i = 1; i < cleateRoomNum; i++)
		{
			roadX1 = rand() % pCleateRooms[i].roomWidth + pCleateRooms[i].roomX;
			roadY1 = rand() % pCleateRooms[i].roomHeight + pCleateRooms[i].roomY;
			roadX0 = rand() % pCleateRooms[i - 1].roomWidth + pCleateRooms[i - 1].roomX;
			roadY0 = rand() % pCleateRooms[i - 1].roomHeight + pCleateRooms[i - 1].roomY;
			//道描画の処理

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
			else
			{
				cleateRoadX = 0;
				roadXDelta = 0;
			}

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
			else
			{
				cleateRoadY = 0;
				roadYDelta = 0;
			}

			//曲がらない道
			if (roadXDelta == 0 || roadYDelta == 0)
			{
				int Delta = roadXDelta + roadYDelta;
				for (int i = 0; i < Delta; i++)
				{
					pMapData[roadX1 + (i*cleateRoadX) + ((i*cleateRoadY) + roadY1)*mapWidth] = FLOOR;
				}
			}

			//横に伸びる道
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
			//縦に伸びる道
			else
			{
				for (int i = 0; i < roadYDelta / 2; i++)
				{
					pMapData[roadX1 + (i*cleateRoadY + roadY1) * mapWidth] = FLOOR;
				}
				for (int i = 0; i < roadXDelta; i++)
				{
					pMapData[roadX1 + (i*cleateRoadX) + ((roadYDelta / 2)*cleateRoadY + roadY1) * mapWidth] = FLOOR;
				}
				for (int i = 0; i < roadYDelta / 2 + roadYDelta % 2; i++)
				{
					pMapData[roadX1 + (roadXDelta*cleateRoadX) + ((roadYDelta / 2 + i)*cleateRoadY  + roadY1) * mapWidth] = FLOOR;
				}
			}
		}
		//階段生成
		int downstairroom = 0;
		downstairroom = rand() % cleateRoomNum;
		downStairX = rand() % pCleateRooms[downstairroom].roomWidth + pCleateRooms[downstairroom].roomX;
		downStairY = rand() % pCleateRooms[downstairroom].roomHeight + pCleateRooms[downstairroom].roomY;
		pMapData[downStairX + downStairY*mapWidth] = STAIR;
		//プレイヤー配置
		cleatePlayerRoom = rand() % cleateRoomNum;
		//要らない道を埋め立て
		ClearRoad();
		//ミニマップ作製
		CleateMiniMap();
	}
}

//大部屋生成
void Map::CleateBigRoom()
{
	for (int i = 0; i < mapSize; i++)
	{
		if (i>mapWidth && i<mapSize - mapWidth && i%mapWidth != 0 && i%mapWidth != mapWidth - 1)
		{
			pMapData[i] = FLOOR;
			pMapRoomData[i] = 1;
		}
	}
}

//プレイヤーの配置に使う関数
int Map::ChoicePlayerMapX(int playerRoom)
{
	return rand() % pCleateRooms[playerRoom].roomWidth + pCleateRooms[playerRoom].roomX;
}

int Map::ChoicePlayerMapY(int playerRoom)
{
	return rand() % pCleateRooms[playerRoom].roomHeight + pCleateRooms[playerRoom].roomY;
}

//新しいマップ作製
void Map::NewMapCleate()
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
	floorNum = 0;
	downStairX = 0;
	downStairY = 0;
	cleatePlayerRoom = 0;
	Release();
	CleateMap(20, 20);
	changeNextStair = false;
}

//要らない道を消す
void Map::ClearRoad()
{
	//消去する道のデータを格納する配列
	pRoadData = new bool[mapSize];
	for (int i = 0; i < mapSize; i++)
	{
		pRoadData[i] = false;
	}
	int roadCount = 0;
	//壁を伸ばして埋め立てる
	for (int i = 1; i < mapHeight - 2; i++)
	{
		for (int j = 1; j < mapWidth - 2; j++)
		{
			if ((pMapData[i*mapWidth + j] == WALL || pMapData[i*mapWidth + j] == FIXWALL) || pRoadData[i*mapWidth + j] == true)
			{
				if ((pMapData[(i + 1)*mapWidth + (j - 1)] == FLOOR || pMapData[(i + 1)*mapWidth + (j - 1)] == STAIR) &&
					(pMapData[(i + 1)*mapWidth + j] == FLOOR || pMapData[(i + 1)*mapWidth + j] == STAIR) &&pMapRoomData[(i + 1)*mapWidth + j] == 0 &&
					(pMapData[(i + 1)*mapWidth + (j + 1)] == FLOOR || pMapData[(i + 1)*mapWidth + (j + 1)] == STAIR) )
				{
					if ((pMapData[(i + 2)*mapWidth + (j - 1)] == FLOOR || pMapData[(i + 2)*mapWidth + (j - 1)] == STAIR) && pRoadData[(i + 2)*mapWidth + (j - 1)] == false &&
						(pMapData[(i + 2)*mapWidth + j] == FLOOR || pMapData[(i + 2)*mapWidth + j] == STAIR) && pRoadData[(i + 2)*mapWidth + j] == false &&
						(pMapData[(i + 2)*mapWidth + (j + 1)] == FLOOR || pMapData[(i + 2)*mapWidth + (j + 1)] == STAIR) && pRoadData[(i + 2)*mapWidth + (j + 1)] == false)
					{
						pRoadData[(i + 1)*mapWidth + j] = true;
					}
				}
				if ((pMapData[(i - 1)*mapWidth + (j + 1)] == FLOOR || pMapData[(i - 1)*mapWidth + (j + 1)] == STAIR) &&
					(pMapData[i*mapWidth + (j + 1)] == FLOOR || pMapData[i*mapWidth + (j + 1)] == STAIR) && pMapRoomData[i*mapWidth + (j + 1)] == 0 &&
					(pMapData[(i + 1)*mapWidth + (j + 1)] == FLOOR || pMapData[(i + 1)*mapWidth + (j + 1)] == STAIR))
				{
					if ((pMapData[(i - 1)*mapWidth + (j + 2)] == FLOOR || pMapData[(i - 1)*mapWidth + (j + 2)] == STAIR) && pRoadData[(i - 1)*mapWidth + (j + 2)] == false &&
						(pMapData[i*mapWidth + (j + 2)] == FLOOR || pMapData[i*mapWidth + (j + 2)] == STAIR) && pRoadData[i*mapWidth + (j + 2)] == false &&
						(pMapData[(i + 1)*mapWidth + (j + 2)] == FLOOR || pMapData[(i + 1)*mapWidth + (j + 2)] == STAIR) && pRoadData[(i + 1)*mapWidth + (j + 2)] == false)
					{
						pRoadData[i*mapWidth + (j + 1)] = true;
					}
				}
			}
		}
	}
	for (int i = mapHeight - 1; i > 2 ; i--)
	{
		for (int j = mapWidth - 1; j > 2 ; j--)
		{
			if ((pMapData[i*mapWidth + j] == WALL || pMapData[i*mapWidth + j] == FIXWALL) || pRoadData[i*mapWidth + j] == true)
			{
				if ((pMapData[(i - 1)*mapWidth + (j - 1)] == FLOOR || pMapData[(i - 1)*mapWidth + (j - 1)] == STAIR) &&
					(pMapData[(i - 1)*mapWidth + j] == FLOOR || pMapData[(i - 1)*mapWidth + j] == STAIR) && pMapRoomData[(i - 1)*mapWidth + j] == 0 &&
					(pMapData[(i - 1)*mapWidth + (j + 1)] == FLOOR || pMapData[(i - 1)*mapWidth + (j + 1)] == STAIR))
				{
					if ((pMapData[(i - 2)*mapWidth + (j - 1)] == FLOOR || pMapData[(i - 2)*mapWidth + (j - 1)] == STAIR) && pRoadData[(i - 2)*mapWidth + (j - 1)] == false &&
						(pMapData[(i - 2)*mapWidth + j] == FLOOR || pMapData[(i - 2)*mapWidth + j] == STAIR) && pRoadData[(i - 2)*mapWidth + j] == false &&
						(pMapData[(i - 2)*mapWidth + (j + 1)] == FLOOR || pMapData[(i - 2)*mapWidth + (j + 1)] == STAIR) && pRoadData[(i - 2)*mapWidth + (j + 1)] == false)
					{
						pRoadData[(i - 1)*mapWidth + j] = true;
					}
				}
				if ((pMapData[(i - 1)*mapWidth + (j - 1)] == FLOOR || pMapData[(i - 1)*mapWidth + (j - 1)] == STAIR) &&
					(pMapData[i*mapWidth + (j - 1)] == FLOOR || pMapData[i*mapWidth + (j - 1)] == STAIR) && pMapRoomData[i*mapWidth + (j - 1)] == 0 &&
					(pMapData[(i + 1)*mapWidth + (j - 1)] == FLOOR || pMapData[(i + 1)*mapWidth + (j - 1)] == STAIR))
				{
					if ((pMapData[(i - 1)*mapWidth + (j - 2)] == FLOOR || pMapData[(i - 1)*mapWidth + (j - 2)] == STAIR) && pRoadData[(i - 1)*mapWidth + (j - 2)] == false &&
						(pMapData[i*mapWidth + (j - 2)] == FLOOR || pMapData[i*mapWidth + (j - 2)] == STAIR) && pRoadData[i*mapWidth + (j - 2)] == false &&
						(pMapData[(i + 1)*mapWidth + (j - 2)] == FLOOR || pMapData[(i + 1)*mapWidth + (j - 2)] == STAIR) && pRoadData[(i + 1)*mapWidth + (j - 2)] == false)
					{
						pRoadData[i*mapWidth + (j - 1)] = true;
					}
				}
			}
		}
	}

	for (int i = 1; i < mapHeight - 1; i++)
	{
		for (int j = 1; j < mapWidth - 1; j++)
		{
			if (pMapRoomData[i*mapWidth + j] == 0 && pMapData[i*mapWidth + j] == FLOOR && pRoadData[i*mapWidth + j] == false)
			{
				for (int m = -1; m <= 1; m++)
				{
					for (int n = -1; n <= 1; n++)
					{
						//要らない角を埋め立てる
						if ((pMapData[(i + m)*mapWidth + (j + n)] == FLOOR || pMapData[(i + m)*mapWidth + (j + n)] == STAIR) && pRoadData[(i + m)*mapWidth + (j + n)] == false &&
							(pMapData[i*mapWidth + (j + n)] == FLOOR || pMapData[i*mapWidth + (j + n)] == STAIR) && pRoadData[i*mapWidth + (j + n)] == false &&
							(pMapData[(i + m)*mapWidth + j] == FLOOR || pMapData[(i + m)*mapWidth + j] == STAIR) && pRoadData[(i + m)*mapWidth + j] == false &&
							((pMapData[(i - m)*mapWidth + (j - n)] != FLOOR && pMapData[(i - m)*mapWidth + (j - n)] != STAIR) || pRoadData[(i - m)*mapWidth + (j - n)] == true) &&
							((pMapData[i*mapWidth + (j - n)] != FLOOR && pMapData[i*mapWidth + (j - n)] != STAIR) || pRoadData[i*mapWidth + (j - n)] == true) &&
							((pMapData[(i - m)*mapWidth + j] != FLOOR && pMapData[(i - m)*mapWidth + j] != STAIR) || pRoadData[(i - m)*mapWidth + j] == true) &&
							m != 0 && n != 0)
						{
							pRoadData[i*mapWidth + j] = true;
						}
					}
				}
			}
			if (pMapRoomData[i*mapWidth + j] == 0 && pMapData[i*mapWidth + j] == FLOOR)
			{
				for (int m = -1; m <= 1; m++)
				{
					for (int n = -1; n <= 1; n++)
					{
						if ((pMapData[(i + m)*mapWidth + (j + n)] == FLOOR || pMapData[(i + m)*mapWidth + (j + n)] == STAIR) && pRoadData[(i + m)*mapWidth + (j + n)] == false)
						{
							roadCount++;
						}
					}
				}
			}
			//要らない空間を埋め立てる
			if (roadCount == 9)
			{
				pRoadData[i*mapWidth + j] = true;
			}
			roadCount = 0;
		}
	}

	//行き止まりを埋め立てる
	for (int i = 1; i < mapHeight - 1; i++)
	{
		for (int j = 1; j < mapWidth - 1; j++)
		{
			if (pMapData[i*mapWidth + j] == FLOOR && pMapRoomData[i*mapWidth + j] == 0)
			{
				if (pMapData[(i - 1)*mapWidth + j] == STAIR ||
					pMapData[i*mapWidth + (j - 1)] == STAIR ||
					pMapData[i*mapWidth + (j + 1)] == STAIR ||
					pMapData[(i + 1)*mapWidth + j] == STAIR)
				{
					break;
				}
				if ((pMapData[(i - 1)*mapWidth + j] == FLOOR && pRoadData[(i - 1)*mapWidth + j] != true))
				{
					roadCount++;
				}
				if ((pMapData[i*mapWidth + (j - 1)] == FLOOR && pRoadData[i*mapWidth + (j - 1)] != true))
				{
					roadCount++;
				}
				if ((pMapData[i*mapWidth + (j + 1)] == FLOOR && pRoadData[i*mapWidth + (j + 1)] != true))
				{
					roadCount++;
				}
				if ((pMapData[(i + 1)*mapWidth + j] == FLOOR && pRoadData[(i + 1)*mapWidth + j] != true))
				{
					roadCount++;
				}
				if (roadCount == 1 || roadCount == 0)
				{
					pRoadData[i*mapWidth + j] = true;
				}
				roadCount = 0;
			}
		}
	}
	roadCount = 0;
	for (int i = mapHeight - 1; i > 1; i--)
	{
		for (int j = mapWidth - 1; j > 1; j--)
		{
			if (pMapData[i*mapWidth + j] == FLOOR && pMapRoomData[i*mapWidth + j] == 0)
			{
				if (pMapData[(i - 1)*mapWidth + j] == STAIR ||
					pMapData[i*mapWidth + (j - 1)] == STAIR ||
					pMapData[i*mapWidth + (j + 1)] == STAIR ||
					pMapData[(i + 1)*mapWidth + j] == STAIR)
				{
					break;
				}
				if ((pMapData[(i - 1)*mapWidth + j] == FLOOR && pRoadData[(i - 1)*mapWidth + j] != true))
				{
					roadCount++;
				}
				if ((pMapData[i*mapWidth + (j - 1)] == FLOOR && pRoadData[i*mapWidth + (j - 1)] != true))
				{
					roadCount++;
				}
				if ((pMapData[i*mapWidth + (j + 1)] == FLOOR && pRoadData[i*mapWidth + (j + 1)] != true))
				{
					roadCount++;
				}
				if ((pMapData[(i + 1)*mapWidth + j] == FLOOR && pRoadData[(i + 1)*mapWidth + j] != true))
				{
					roadCount++;
				}
				if (roadCount == 1 || roadCount == 0)
				{
					pRoadData[i*mapWidth + j] = true;
				}
				roadCount = 0;
			}
		}
	}

	//埋め立て予定地を埋め立てる
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (pRoadData[i*mapWidth + j] == true)
			{
				pMapData[i*mapWidth + j] = WALL;
			}
		}
	}
}

//ミニマップ作製
void Map::CleateMiniMap()
{
	pMiniMapData = new int[mapSize];
	for (int i = 0; i < mapSize; i++)
	{
		if (pMapData[i] == FLOOR || pMapData[i] == STAIR)
		{
			pMiniMapData[i] = 0;
		}
		else
		{
			pMiniMapData[i] = -1;
		}
	}
}

//ミニマップを見えた状態にする
void Map::KnowMiniMap(Player *player)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i + (player->CallPlayerY() / MAPCIPSIZE + j)*mapWidth] == 0 || pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i + (player->CallPlayerY() / MAPCIPSIZE + j)*mapWidth] == 1)
			{
				pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i + (player->CallPlayerY() / MAPCIPSIZE + j)*mapWidth] = 1;
				if (pMapData[player->CallPlayerX() / MAPCIPSIZE + i + (player->CallPlayerY() / MAPCIPSIZE + j)*mapWidth] == STAIR)
				{
					pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i + (player->CallPlayerY() / MAPCIPSIZE + j)*mapWidth] = 2;
				}
				if (i + j != 0 && i != j)
				{
					for (int n = 1; n <= 2; n++)
					{
						if (pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i * n + (player->CallPlayerY() / MAPCIPSIZE + j * n)*mapWidth] == 0)
						{
							pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i * n + (player->CallPlayerY() / MAPCIPSIZE + j * n)*mapWidth] = 1;
							if (pMapData[player->CallPlayerX() / MAPCIPSIZE + i * n + (player->CallPlayerY() / MAPCIPSIZE + j * n)*mapWidth] == STAIR)
							{
								pMiniMapData[player->CallPlayerX() / MAPCIPSIZE + i * n + (player->CallPlayerY() / MAPCIPSIZE + j * n)*mapWidth] = 2;
							}
						}
					}
				}
			}
		}
	}
	int floordata = pMapRoomData[player->CallPlayerX() / MAPCIPSIZE + (player->CallPlayerY() / MAPCIPSIZE)*mapWidth];
	if (floordata != 0)
	{
		for(int i = -1; i < pCleateRooms[floordata - 1].roomWidth + 1; i++)
		{
			for (int j = -1; j < pCleateRooms[floordata - 1].roomHeight + 1; j++)
			{
				if (pMiniMapData[pCleateRooms[floordata - 1].roomX + i + (pCleateRooms[floordata - 1].roomY + j)*mapWidth] == 0)
				{
					pMiniMapData[pCleateRooms[floordata - 1].roomX + i + (pCleateRooms[floordata - 1].roomY + j)*mapWidth] = 1;
					if (pMapData[pCleateRooms[floordata - 1].roomX + i + (pCleateRooms[floordata - 1].roomY + j)*mapWidth] == STAIR)
					{
						pMiniMapData[pCleateRooms[floordata - 1].roomX + i + (pCleateRooms[floordata - 1].roomY + j)*mapWidth] = 2;
					}
				}
			}
		}
	}
}