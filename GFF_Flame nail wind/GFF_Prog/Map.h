#pragma once

class Map
{
private:

	//�}�b�v�̍����̍Œ�̕ϓ��n
	int mapHeightMin;
	//�}�b�v�̍����̂����Œ�ő�l
	int mapHeightMax;
	//�}�b�v�̕��̂����Œ�̕ϓ��n
	int mapWidthMin;
	//�}�b�v�̕��̂����Œ�ő�l
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

	//�}�b�v�̈�}�X�̃T�C�Y�@�g��Ȃ�����
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
	//�����̎�������
	void CleateRoom(struct rooms *room, int x, int y,int width, int height);
	//�}�b�v�̎�������
	void CleateMap(int width, int height);
	//�啔���쐬
	void CleateBigRoom();
	//�������������ɔԍ��U��
	void SetCleateNum(struct rooms *room, int num) { room->cleateNum = num; }
	//�}�b�v�Ăяo��
	int CallMap(int width, int height) { return pMapData[mapWidth*height + width]; }
	//�}�b�v��̃T�C�Y�̌Ăяo��
	int CallMapCipSize() { return MAPCIPSIZE; }
	//�}�b�v�ő卂��
	int CallMapHeight() { return mapHeight; }
	//�}�b�v�̍ő剡��
	int CallMapWidth() { return mapWidth; }

private:
	//�����̑z�萔
	int roomNum;
	//�����̍\���̂��i�[����|�C���^
	rooms* pRoomNum;
	//�������������̐�
	int cleateRoomNum;
	//���������̃|�C���^
	rooms* pCleateRooms;

};