#pragma once
// ¸Ê »ç°¢Çü °³¼ö
#define MAX_OF_MAPWIDTH 40
#define MAX_OF_MAPHEIGHT 40

// ¸Ê »ç°¢Çü Å©±â
#define SIZE_OF_MAPWIDTH 50
#define SIZE_OF_MAPHEIGHT 50

// ¸Ê ±×¸®±â ½ÃÀÛ ÁÂÇ¥ (left, top)
#define MAP_START_POINT_X 100
#define MAP_START_POINT_Y 100

class Map
{
public:
	int charX;
	int charY;
	int mapId;
	int borderX;
	int borderY;
	POINT mapSizeNow;
	enum { Deadlock = 1, Floor };
	unsigned char matrix[5][MAX_OF_MAPHEIGHT][MAX_OF_MAPWIDTH] =
	{ {
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,3,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,2,2,2,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
		},

		{
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,2,0,0,0,0,0,0,0,1},
		{1,0,0,2,2,2,2,2,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,2,2,2,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
		}
	};
public:
	Map();
	int getWidth(int num)
	{
		int count = 0;
		for (int i = 0; matrix[num][0][i] == 1; i++)
		{
			count++;
		}
		return count;
	}
	int getHeight(int num)
	{
		int count = 0;
		for (int i = 0; matrix[num][i][0] == 1; i++)
		{
			count++;
		}
		return count;
	}
	void drawMap(HDC hdc, int);
	void drawBorder(HDC hdc);
	void drawObject(HDC hdc);
};

