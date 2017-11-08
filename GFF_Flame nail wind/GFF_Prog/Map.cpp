#include "Map.h"
#include "time.h"
#include "windows.h"
//�R���X�g���N�^
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
//�f�X�g���N�^
Map::~Map()
{
	delete[] pMapData;
	delete[] pRoomNum;
	delete[] pCleateRooms;
}

void Map::CleateMap(int width, int height)
{
	srand((unsigned int)time(NULL));

	mapHeightMax = rand() % height;//0�`height
	mapHeightMin = 20;//��
	mapHeight = mapHeightMax + mapHeightMin;//�}�b�v�S�̂̑傫��(Y�j
	mapWidthMax = rand() % width;//0�`width
	mapWidthMin = 20;//��
	mapWidth = mapWidthMax + mapWidthMin;//�}�b�v�S�̂̑傫���iX�j
	mapSize = mapHeight*mapWidth;//�z��̗v�f������
	pMapData = new int[mapSize];//�z��𓮓I�Ɋm��
	roomNum = rand() % 18 + 2;//2�`12���������쐬
	pRoomNum = new struct rooms[roomNum];//�����̑���

	//�󂳂�Ȃ���
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
	//��������镪����������
	for (int i = 0; i < roomNum; i++)
	{
		pRoomNum[i].roomHeight = 0;
		pRoomNum[i].roomWidth = 0;
		pRoomNum[i].roomX = 0;
		pRoomNum[i].roomY = 0;
		pRoomNum[i].cleateNum = 0;
	}
	//��������
	for (int i = 0; i < roomNum; i++)
	{
		int rx = rand() % (mapWidth - 6) + 1;//+1�͉E���̃}�b�v�����̂��߂ɕK�{���������猇���Ă��܂�
		int ry = rand() % (mapHeight - 6) + 1;
		int rw = rand() % 5 + 5;
		int rh = rand() % 5 + 5;

		//�󂹂Ȃ��ǂ��Ȃ���Ε`��i����p���x�_�ł������`��ł���Ώd�Ȃ낤���`�悷��j
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
			cleateRoomNum++;//�����ɔԍ��U��@���������ɂ�+�P
			SetCleateNum(&pRoomNum[i], cleateRoomNum);
		}
	}
	
	pCleateRooms = new struct rooms[cleateRoomNum];

	//������
	for (int i = 0; i < cleateRoomNum; i++)
	{
		pCleateRooms[i].cleateNum = 0;
		pCleateRooms[i].roomHeight = 0;
		pCleateRooms[i].roomWidth = 0;
		pCleateRooms[i].roomX = 0;
		pCleateRooms[i].roomY = 0;
	}

	//����Ȃ����̂��|�C�̂�
	for (int i = 0; i < roomNum; i++)
	{
		if (pRoomNum[i].cleateNum != 0)
		{
			pCleateRooms[pRoomNum[i].cleateNum - 1] = pRoomNum[i];
		}
	}

	//������������0��1�̎�
	//�啔������
	if (cleateRoomNum == 0 || cleateRoomNum == 1)
	{
		CleateBigRoom();
	}
	else if(cleateRoomNum >= 2)
	{
		//���쐬
		int roadX1 = 0;//�����o�镔���̒��̊�n�_�n�܂�i�����̂��ߏꏊ�͕s���j
		int roadY1 = 0;
		int roadX0 = 0;//���ǂ蒅�������̓��̊�n�_�I�_
		int roadY0 = 0;
		int cleateRoadX = 0;//���̐܂�Ԃ��n�_�̂��߂̕ϐ�
		int cleateRoadY = 0;
		int roadXDelta = 0;//roadX1��roadX0 �̈ʒu�̍��̐�Βl�i���̐��ɂȂ�j
		int roadYDelta = 0;//roadY1��roadY0 �̈ʒu�̍��̐�Βl�i���̐��ɂȂ�j
		for (int i = 1; i < cleateRoomNum; i++)
		{
			roadX1 = rand() % pCleateRooms[i].roomWidth + pCleateRooms[i].roomX;
			roadY1 = rand() % pCleateRooms[i].roomHeight + pCleateRooms[i].roomY;
			roadX0 = rand() % pCleateRooms[i - 1].roomWidth + pCleateRooms[i - 1].roomX;
			roadY0 = rand() % pCleateRooms[i - 1].roomHeight + pCleateRooms[i - 1].roomY;
			//���`��̏���

			//���̍��̐�Βl�擾�iX�j
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
			//���̍��̐�Βl�擾�iY�j
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
				//�����������[�v�i�Е����O�Ȃ̂͊m��Ȃ̂Œ����̒ʘH�������Ȃ�j
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

//�����쐬
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