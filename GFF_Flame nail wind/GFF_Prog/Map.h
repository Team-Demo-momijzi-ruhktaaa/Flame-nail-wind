#pragma once

class Map
{
private:

	//マップの高さのうち変動地
	int mapHeightMin;
	//マップの高さのうち固定値
	int mapHeightMax;
	//マップの幅のうち変動地
	int mapWidthMin;
	//マップの幅のうち固定値
	int mapWidthMax;
	//マップの高さ
	int mapHeight;
	//マップの幅
	int mapWidth;
	//マップの総マス数
	int mapSize;
	//マップを格納する動的配列用のポインタ
	int* pMapData;
public:

	//マップの位置マスのサイズ　使わないかも
	const int MAPCIPSIZE;
	//マップのステータス
	const int FLOOR;
	const int WALL;
	const int FIXWALL;

	//部屋のデータ
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
	//部屋の総定数
	int roomNum;
	//部屋の構造体を格納するポインタ
	rooms* pRoomNum;
	//完成した部屋の数
	int cleateRoomNum;

	rooms* pCleateRooms;

};