#pragma once

class Player;

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
	//階層番号
	int floorNum;

	////上り階段の座標
	//int upStairX;
	//int upStairY;
	//下り階段の座標
	int downStairX;
	int downStairY;

	int cleatePlayerRoom;

	bool changeNextStair;

public:

	//マップの一マスのサイズ　使わないかも
	const int MAPCIPSIZE;
	//マップのステータス
	const int FLOOR;
	const int WALL;
	const int FIXWALL;
	const int STAIR;
	const int MINIMAPSIZE;

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
	//部屋の想定数
	int roomNum;

	//部屋の構造体を格納するポインタ
	rooms* pRoomNum;

	//完成した部屋の数
	int cleateRoomNum;

	rooms* pCleateRooms;

	int* pMapRoomData;

	bool* pRoadData;

	int* pMiniMapData;

};