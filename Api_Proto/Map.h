#include "Chara.h"
#include "WindowScreen.h"
#include <cmath>

#pragma once
// ¸Ê »ç°¢Çü °³¼ö
#define MAX_OF_MAPWIDTH 40
#define MAX_OF_MAPHEIGHT 20
#define Map_vary 5

#define STAGE_MOVE_SPEED 3.14159265358979323846

void drawBackground(HDC, RECT, RECT);

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
	int MAX_MAP_START_POINT_X;
	int MAX_MAP_START_POINT_Y;
	int MAP_START_POINT_X;
	int MAP_START_POINT_Y;
	RECT MaxSize; //¸ðµç ¸ÊÀº ¿©±â¿¡ ¸ÂÃç¼­ ±×·ÁÁü
	int mapId;
	// ¸Ê ÃÖ´ëÅ©±â ¿Ü°û
	int maxBorderX;
	int maxBorderY;
	// ¸Ê Å©±â
	int borderX;
	int borderY;
	int stageCount;
	bool can_NextStage;
	POINT mapSizeNow;
	enum { Deadlock = 1, PassFloor, NonPassFloor, DoorOpen, DoorClose };
	unsigned char matrix[Map_vary][MAX_OF_MAPHEIGHT][MAX_OF_MAPWIDTH] =
	{
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,0,3,0,0,3,0,0},
		{0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0},
		{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,0,0,0,0},
		{5,0,0,0,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
		{10,}
		},
		
		{
		{0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,3,3,3,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,3,0,0,0,3,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,3,3,3,0,0,3,0},
		{0,0,3,0,0,0,0,0,0,0},
		{5,0,0,0,0,0,0,0,0,5},
		{10,}
		},

		{
		{0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,3,0,0},
		{0,0,0,0,3,3,3,0,0,0},
		{0,0,0,3,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,3,3,3,3,0,0,0,0},
		{0,0,0,0,0,3,3,0,0,0},
		{0,0,0,0,0,0,0,3,0,0},
		{5,0,0,0,0,0,0,0,0,5},
		{10,}
		}
	};
public:
	Map(RECT winRect);
	int getWidth(int num);
	int getHeight(int num);
	void drawRect(HDC hdc, int x, int y, COLORREF rgb);
	void drawMap(HDC hdc, int);
	void drawBorder(HDC hdc);
	void drawObject(HDC hdc);
	void Collision(Character*);
	//void CollisionCheck(Character* Player, int bfRow, int bfCol, int nowRow, int nowCol);
	void openNextStage();
	void changer(RECT winRect);
	void changeAnimetion(HDC hdc, RECT winRect, float delta);
	void Reset(RECT winRect);
	void update();
	void ProjColl(HDC hdc, Character* Player);
	int getBlockLeft(int row, int col);
	int getBlockTop(int row, int col);
	int getBlockBottom(int row, int col);
	int getBlockRight(int row, int col);
	int getBlockCenterX(int col);
	int getBlockCenterY(int row);
	int getBlockType(int row, int col);
	int xToCol(float x);
	int yToRow(float y);
};

/*
{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 10},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,3,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0},
		{0,0,0,0,3,3,3,3,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{5,0,0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
		{10,}
		},
*/