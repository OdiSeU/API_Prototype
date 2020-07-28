#pragma once
// ¸Ê »ç°¢Çü °³¼ö
#define COUNT_OF_MAPWIDTH 33
#define COUNT_OF_MAPHEIGHT 16

// ¸Ê »ç°¢Çü Å©±â
#define SIZE_OF_MAPWIDTH 50
#define SIZE_OF_MAPHEIGHT 50

// ¸Ê ±×¸®±â ½ÃÀÛ ÁÂÇ¥ (left, top)
#define MAP_START_POINT_X 100
#define MAP_START_POINT_Y 100

class Map
{
	enum { Deadlock = 1, Floor };
	int Height;
	int Width;
	unsigned char matrix[5][COUNT_OF_MAPHEIGHT][COUNT_OF_MAPWIDTH] =
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
	Map(int Height, int Width);
};

