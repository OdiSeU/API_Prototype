#include <Windows.h>
#include "Map.h"

Map::Map()
{
	mapId = 0;
	mapSizeNow.x = getWidth(mapId); // 해당 맵의 너비
	mapSizeNow.y = getHeight(mapId); // 해당 맵의 높이
	borderX = mapSizeNow.x * SIZE_OF_MAPWIDTH + MAP_START_POINT_X; // 맵 전체 너비
	borderY = mapSizeNow.y * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y; // 맵 전체 높이
}

void Map::drawMap(HDC hdc, int mapId)
{
	for (int i = 0; i < mapSizeNow.y; i++)
	{
		for (int j = 0; j < mapSizeNow.x; j++)
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
	for (int i = 0; i < mapSizeNow.y; i++)
	{
		for (int j = 0; j < mapSizeNow.x; j++)
		{
			if (matrix[mapId][i][j] == 2)
			{
				HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
				HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
				Rectangle(hdc, MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT,
					MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT + SIZE_OF_MAPHEIGHT);
				SelectObject(hdc, OldB);
				DeleteObject(NewB);
			}
			else if (matrix[mapId][i][j] == 3)
			{
				HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
				HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
				Rectangle(hdc, MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT,
					MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT + SIZE_OF_MAPHEIGHT);
				SelectObject(hdc, OldB);
				DeleteObject(NewB);
			}
		}
	}
}