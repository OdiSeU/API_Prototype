#include "Chara.h"
#include "WindowScreen.h"
#include "math.h"

#pragma once
// ¸Ê »ç°¢Çü °³¼ö
#define MAX_OF_MAPWIDTH 40
#define MAX_OF_MAPHEIGHT 40

#define STAGE_MOVE_SPEED 3.14/2

class Map
{
public:
	int buff;
	float stageMoveSpeed;
	bool changedAnime;
	// ¸Ê »ç°¢Çü Å©±â
	int SIZE_OF_MAPWIDTH;
	int SIZE_OF_MAPHEIGHT;
	// ¸Ê ±×¸®±â ½ÃÀÛ ÁÂÇ¥ (left, top)
	int MAP_START_POINT_X;
	int MAP_START_POINT_Y;
	RECT MaxSize; //¸ðµç ¸ÊÀº ¿©±â¿¡ ¸ÂÃç¼­ ±×·ÁÁü
	int mapId;
	int borderX;
	int borderY;
	int stageCount;
	bool can_NextStage;
	POINT mapSizeNow;
	enum { Deadlock = 1, PassFloor, NonPassFloor, DoorOpen, DoorClose };
	unsigned char matrix[5][MAX_OF_MAPHEIGHT][MAX_OF_MAPWIDTH] =
	{ 
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0},
		{0,0,0,0,0,0,3,3,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{5,0,0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
		{10,}
		},

		{
		{0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,3,3,3,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,2,2,2,2,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{5,0,0,0,0,0,0,0,0,5},
		{10,}
		},

		{
		{0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,3,3,3,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,3,3,2,2,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{5,0,0,0,0,0,0,0,0,5},
		{10,}
		}
	};
public:
	Map(RECT winRect);
	int getWidth(int num)
	{
		int count = 0;
		for (int i = 0; matrix[num][0][i] != 10; i++)
		{
			count++;
		}
		return count;
	}
	int getHeight(int num)
	{
		int count = 0;
		for (int i = 0; matrix[num][i][0] != 10; i++)
		{
			count++;
		}
		return count;
	}
	void drawRect(HDC hdc,int x, int y, COLORREF rgb);
	void drawMap(HDC hdc, int);
	void drawBorder(HDC hdc);
	void drawObject(HDC hdc);
	void Collision(Character*);
	void openNextStage();
	void changer(RECT winRect);
	void changeAnimetion(HDC hdc, RECT winRect, float delta);
	void Reset(RECT winRect);

};

