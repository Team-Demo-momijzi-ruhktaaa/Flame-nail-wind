#include "Map.h"
#include "time.h"
#include "windows.h"

Map::Map() :MAPCIPSIZE(16), FLOOR(0), WALL(1), FIXWALL(-1)
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

Map::~Map()
{
	delete[] pMapData;
	delete[] pRoomNum;
	delete[] pCleateRooms;
}

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

void Map::CleateMap(int width, int height)
{

	srand((unsigned int)time(NULL));

	mapHeightMax = rand() % height;
	mapHeightMin = 20;//‰¼
	mapHeight = mapHeightMax + mapHeightMin;
	mapWidthMax = rand() % width;
	mapWidthMin = 20;//‰¼
	mapWidth = mapWidthMax + mapWidthMin;
	mapSize = mapHeight*mapWidth;
	pMapData = new int[mapSize];
	roomNum = rand() % 10 + 2;
	pRoomNum = new struct rooms[roomNum];

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

	for (int i = 0; i < roomNum; i++)
	{
		pRoomNum[i].cleateNum = 0;
		pRoomNum[i].roomHeight = 0;
		pRoomNum[i].roomWidth = 0;
		pRoomNum[i].roomX = 0;
		pRoomNum[i].roomY = 0;
	}

	for (int i = 0; i < roomNum; i++)
	{
		int rx = rand() % (mapWidth - 6) + 1;
		int ry = rand() % (mapHeight - 6) + 1;
		int w = rand() % 5 + 5;
		int h = rand() % 5 + 5;
		if (pMapData[rx + ry*mapWidth] != FLOOR &&
			pMapData[rx + (ry + h)*mapWidth] != FLOOR &&
			pMapData[rx + w + ry*mapWidth] != FLOOR &&
			pMapData[rx + w + (ry + h)*mapWidth] != FLOOR &&
			pMapData[rx + w / 2 + (ry + h / 2)*mapWidth] != FLOOR &&
			rx + w < mapWidth && ry + h < mapHeight)
		{
			CleateRoom(&pRoomNum[i], rx, ry, w, h);
			cleateRoomNum++;
			SetCleateNum(&pRoomNum[i], cleateRoomNum);
		}
	}

	pCleateRooms = new struct rooms[cleateRoomNum];

	for (int i = 0; i < cleateRoomNum; i++)
	{
		pCleateRooms[i].cleateNum = 0;
		pCleateRooms[i].roomHeight = 0;
		pCleateRooms[i].roomWidth = 0;
		pCleateRooms[i].roomX = 0;
		pCleateRooms[i].roomY = 0;
	}

	for (int i = 0; i < roomNum; i++)
	{
		if (pRoomNum[i].cleateNum != 0)
		{
			pCleateRooms[pRoomNum[i].cleateNum - 1] = pRoomNum[i];
		}
	}

	//•”‰®¶¬”‚ª0‚©1‚ÌŽž
	//‘å•”‰®¶¬
	if (cleateRoomNum == 0 || cleateRoomNum == 1)
	{
		CleateBigRoom();
	}
	else if(cleateRoomNum >= 2)
	{
		int roadX1 = 0;
		int roadY1 = 0;
		int roadX0 = 0;
		int roadY0 = 0;
		int cleateRoadX = 0;
		int cleateRoadY = 0;
		int roadXDelta = 0;
		int roadYDelta = 0;
		for (int i = 1; i < cleateRoomNum; i++)
		{
			roadX1 = rand() % pCleateRooms[i].roomWidth + pCleateRooms[i].roomX;
			roadY1 = rand() % pCleateRooms[i].roomHeight + pCleateRooms[i].roomY;
			roadX0 = rand() % pCleateRooms[i - 1].roomWidth + pCleateRooms[i - 1].roomX;
			roadY0 = rand() % pCleateRooms[i - 1].roomHeight + pCleateRooms[i - 1].roomY;
			//“¹•`‰æ‚Ìˆ—

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
				int Delta = roadXDelta + roadYDelta;
				for (int i = 0; i < Delta; i++)
				{
					pMapData[roadX1 + (i*cleateRoadX) + ((i*cleateRoadY) + roadY1)*mapWidth] = FLOOR;
				}
			}
			if (roadXDelta >= roadYDelta)
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