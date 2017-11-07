#pragma once

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
public:

	//�}�b�v�̈ʒu�}�X�̃T�C�Y�@�g��Ȃ�����
	const int MAPCIPSIZE;
	//�}�b�v�̃X�e�[�^�X
	const int FLOOR;
	const int WALL;
	const int FIXWALL;

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
	int CallMap(int width, int height) { return pMapData[mapWidth*height + width]; }
	int CallMapCipSize() { return MAPCIPSIZE; }
	int CallMapHeight() { return mapHeight; }
	int CallMapWidth() { return mapWidth; }

private:
	//�����̑��萔
	int roomNum;
	//�����̍\���̂��i�[����|�C���^
	rooms* pRoomNum;
	//�������������̐�
	int cleateRoomNum;

	rooms* pCleateRooms;

};