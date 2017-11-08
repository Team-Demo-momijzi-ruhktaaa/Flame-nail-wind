#pragma once

class Map
{
private:

	//マップの高さの最低の変動地
	int mapHeightMin;
	//マップの高さのうち固定最大値
	int mapHeightMax;
	//マップの幅のうち最低の変動地
	int mapWidthMin;
	//マップの幅のうち固定最大値
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

	//マップの一マスのサイズ　使わないかも
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
	//部屋の自動生成
	void CleateRoom(struct rooms *room, int x, int y,int width, int height);
	//マップの自動生成
	void CleateMap(int width, int height);
	//大部屋作成
	void CleateBigRoom();
	//完成した部屋に番号振る
	void SetCleateNum(struct rooms *room, int num) { room->cleateNum = num; }
	//マップ呼び出し
	int CallMap(int width, int height) { return pMapData[mapWidth*height + width]; }
	//マップ一つのサイズの呼び出し
	int CallMapCipSize() { return MAPCIPSIZE; }
	//マップ最大高さ
	int CallMapHeight() { return mapHeight; }
	//マップの最大横幅
	int CallMapWidth() { return mapWidth; }

private:
	//部屋の想定数
	int roomNum;
	//部屋の構造体を格納するポインタ
	rooms* pRoomNum;
	//完成した部屋の数
	int cleateRoomNum;
	//部屋生成のポインタ
	rooms* pCleateRooms;

};