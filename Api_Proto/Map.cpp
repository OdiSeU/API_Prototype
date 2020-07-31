#include <Windows.h>
#include "Map.h"

Map::Map()
{
	mapId = 0;
	mapSizeNow.x = getWidth(mapId);
	mapSizeNow.y = getHeight(mapId);
	borderX = mapSizeNow.x * SIZE_OF_MAPWIDTH + MAP_START_POINT_X;
	borderY = mapSizeNow.y * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y;
}

void Map::drawMap(HDC hdc, int mapId)
{
	for (int i = 0; i < getHeight(mapId); i++)
	{
		for (int j = 0; j < getWidth(mapId); j++)
		{
			Rectangle(hdc, MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT,
				MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT + SIZE_OF_MAPHEIGHT);
		}
	}
}

void Map::drawBorder(HDC hdc)
{
	HBRUSH NewB, OldB;
	NewB = (HBRUSH)GetStockObject(NULL_BRUSH);
	OldB = (HBRUSH)SelectObject(hdc, NewB);
	
	int width = getWidth(mapId) * SIZE_OF_MAPWIDTH + MAP_START_POINT_X;
	int height = getHeight(mapId) * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y;
	Rectangle(hdc, MAP_START_POINT_X, MAP_START_POINT_Y, width, height);

	SelectObject(hdc, OldB);
	DeleteObject(NewB);
}

void Map::drawObject(HDC hdc)
{
	HBRUSH NewB, OldB;
	NewB = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	OldB = (HBRUSH)SelectObject(hdc, NewB);

	for (int i = 0; i < getHeight(mapId); i++)
	{
		for (int j = 0; j < getWidth(mapId); j++)
		{
			if (matrix[mapId][i][j] == 2)
			{
				Rectangle(hdc, MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT,
					MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT + SIZE_OF_MAPHEIGHT);
			}
		}
	}

	SelectObject(hdc, OldB);
	DeleteObject(NewB);
}