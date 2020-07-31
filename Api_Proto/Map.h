#pragma once
// 맵 사각형 개수
#define MAX_OF_MAPWIDTH 40
#define MAX_OF_MAPHEIGHT 40

// 맵 사각형 크기
#define SIZE_OF_MAPWIDTH 50
#define SIZE_OF_MAPHEIGHT 50

// 맵 그리기 시작 좌표 (left, top)
#define MAP_START_POINT_X 100
#define MAP_START_POINT_Y 100

class Map
{
	enum { Deadlock = 1, Floor };
	unsigned char matrix[5][MAX_OF_MAPHEIGHT][MAX_OF_MAPWIDTH] =
	{ { 
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
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
		{1,0,0,0,0,0,0,0,0,1},
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
	void drawBorder(HDC hdc, int);
	void drawObject(HDC hdc, int Mapnum);
	/*
	unsigned char checker(int mapnum, float x, float y, int *a, int *b) // 캐릭터 좌표 어디 속해있는지 체크
	{
		int indexX = (x - MAP_START_POINT_X);
		int indexY = (y - MAP_START_POINT_Y);
		if (indexX % SIZE_OF_MAPWIDTH == 0)
		{
			indexX = indexX / SIZE_OF_MAPWIDTH - 1;
		}
		else
		{
			indexX = indexX / SIZE_OF_MAPWIDTH;
		}
		a = &indexX;
		if (indexY % MAX_OF_MAPHEIGHT == 0)
		{
			indexY = indexY / MAX_OF_MAPHEIGHT - 1;
		}
		else
		{
			indexY = indexY / MAX_OF_MAPHEIGHT;
		}
		b = &indexY;
		return matrix[mapnum][indexY][indexX];
	}
	void getCenter(int a, int b)
	{
	
	}
	*/
};

