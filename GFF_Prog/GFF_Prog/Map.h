#pragma once

class Player;

class Map
{
private:

	//�}�b�v�̍����̂����ϓ��n
	int mapHeightMin;
	//�}�b�v�̍����̂����Œ�l
	int mapHeightMax;
	//�}�b�v�̕��̂����ϓ��n
	int mapWidthMin;
	//�}�b�v�̕��̂����Œ�l
	int mapWidthMax;
	//�}�b�v�̍���
	int mapHeight;
	//�}�b�v�̕�
	int mapWidth;
	//�}�b�v�̑��}�X��
	int mapSize;
	//�}�b�v���i�[���铮�I�z��p�̃|�C���^
	int* pMapData;
	//�K�w�ԍ�
	int floorNum;

	////���K�i�̍��W
	//int upStairX;
	//int upStairY;
	//����K�i�̍��W
	int downStairX;
	int downStairY;

	int cleatePlayerRoom;

	bool changeNextStair;

public:

	//�}�b�v�̈�}�X�̃T�C�Y�@�g��Ȃ�����
	const int MAPCIPSIZE;
	//�}�b�v�̃X�e�[�^�X
	const int FLOOR;
	const int WALL;
	const int FIXWALL;
	const int STAIR;
	const int MINIMAPSIZE;

	//�����̃f�[�^
	struct rooms
	{
		int roomHeight;
		int roomWidth;
		int roomX;
		int roomY;
		int cleateNum;
	};

	Map();
	~Map();

	void CleateRoom(struct rooms *room, int x, int y,int width, int height);
	void CleateMap(int width, int height);
	void CleateBigRoom();
	void SetCleateNum(struct rooms *room, int num) { room->cleateNum = num; }
	void trueNextStage() { changeNextStair = true; }
	void falseNextStage() { changeNextStair = false; }
	int CallMap(int width, int height) { return pMapData[mapWidth*height + width]; }
	int CallMapCipSize() { return MAPCIPSIZE; }
	int CallMapHeight() { return mapHeight; }
	int CallMapWidth() { return mapWidth; }
	int CallpMapRoomData(int width, int height) { return pMapRoomData[width + height*mapWidth]; }
	int ChoicePlayerMapX(int playerRoom);
	int ChoicePlayerMapY(int playerRoom);
	int CallPlayerRoom() { return cleatePlayerRoom; }
	int CallNextStair() { return changeNextStair; }
	int CallClearRoadData(int width, int height) { return pRoadData[width + height*mapWidth]; }
	int CallCleateRoomNum(){ return cleateRoomNum; }
	void Release();
	void NewMapCleate();
	void ClearRoad();
	void CleateMiniMap();
	void KnowMiniMap(Player *player);
	int CallMiniMap(int width, int height) { return pMiniMapData[width + height*mapWidth]; }
	//void SetMapRoomData(int width, int height, int num) { pMapRoomData[width + height*mapWidth] = num; }
	rooms GetRoomInfo(int roomNum) {
		return pCleateRooms[roomNum];
	}
private:
	//�����̑z�萔
	int roomNum;

	//�����̍\���̂��i�[����|�C���^
	rooms* pRoomNum;

	//�������������̐�
	int cleateRoomNum;

	rooms* pCleateRooms;

	int* pMapRoomData;

	bool* pRoadData;

	int* pMiniMapData;

};