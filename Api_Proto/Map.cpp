#include <Windows.h>
#include<time.h>
#include "Map.h"

Map::Map(RECT winRect)
{
	mapId = 0;
	stageCount = 1;
	Reset(winRect);
}
void Map::Reset(RECT winRect)
{
	can_NextStage = false;
	changedAnime = false;
	mapSizeNow.x = getWidth(mapId); // 해당 맵의 너비
	mapSizeNow.y = getHeight(mapId); // 해당 맵의 높이
	MaxSize = { 100, 100, (winRect.right - winRect.left - 100), (winRect.bottom - winRect.top - 100) };
	MAP_START_POINT_X = MaxSize.left;
	MAP_START_POINT_Y = MaxSize.top;
	SIZE_OF_MAPWIDTH = (MaxSize.right - MaxSize.left) / mapSizeNow.x;
	SIZE_OF_MAPHEIGHT = (MaxSize.bottom - MaxSize.top) / mapSizeNow.y;
	borderX = mapSizeNow.x * SIZE_OF_MAPWIDTH + MAP_START_POINT_X; // 맵 전체 너비
	borderY = mapSizeNow.y * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y; // 맵 전체 높이
	stageMoveSpeed = 0;
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
	COLORREF rgb;
	for (int i = 0; i < mapSizeNow.y; i++)
	{
		for (int j = 0; j < mapSizeNow.x; j++)
		{
			switch (matrix[mapId][i][j])
			{
			case Deadlock:

				break;

			case PassFloor:
				rgb = RGB(255,0,0);
				drawRect(hdc, i, j, rgb);
				break;
				
			case NonPassFloor:
				rgb = RGB(0, 0, 255);
				drawRect(hdc, i, j, rgb);
				break;

			case DoorOpen:
				rgb = RGB(0, 255, 0);
				drawRect(hdc, i, j, rgb);
				break;

			case DoorClose:

				break;

			default:
				break;
			}
		}
	}
}

void Map::drawRect(HDC hdc,int x, int y, COLORREF rgb)
{
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, rgb);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);
	Rectangle(hdc, MAP_START_POINT_X + y * SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + x * SIZE_OF_MAPHEIGHT,
		MAP_START_POINT_X + y * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + x * SIZE_OF_MAPHEIGHT + SIZE_OF_MAPHEIGHT);
	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // 펜 해제
}

/*
void Map::Collision(Character *Player)
{
	int MindexY = (Player->getBottom() - MAP_START_POINT_Y - 1) / SIZE_OF_MAPHEIGHT;
	int MindexX = (Player->getLeft() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
	if (Player->YStat == DOWN && (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) <= Player->getBottom())
	{
		if (matrix[mapId][MindexY + 1][(int)((Player->getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 2
			|| matrix[mapId][MindexY + 1][(int)((Player->getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 2)
		{
			Player->centerY = (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) - CharaH / 2;
			Player->vy = 0;
			Player->jumpNum = 2;
		}
		else if (matrix[mapId][MindexY + 1][(int)((Player->getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3
			|| matrix[mapId][MindexY + 1][(int)((Player->getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3)
		{
			Player->centerY = (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) - CharaH / 2;
			Player->vy = 0;
			Player->jumpNum = 2;
		}
	}
	else if (Player->YStat == UP && (MAP_START_POINT_Y + (MindexY) * SIZE_OF_MAPHEIGHT) >= Player->getTop()) // 블록 3은 안올라가짐
	{
		if (matrix[mapId][MindexY - 1][(int)((Player->getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3
			|| matrix[mapId][MindexY - 1][(int)((Player->getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3)
		{
			Player->centerY = (MAP_START_POINT_Y + (MindexY) * SIZE_OF_MAPHEIGHT) + CharaH / 2;
			Player->vy = 0;
		}
	}
	else if (Player->XStat == LEFT && (MAP_START_POINT_X + (MindexX) * SIZE_OF_MAPWIDTH) <= Player->getLeft())
	{
		if (matrix[mapId][(int)((Player->getTop() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3
			|| matrix[mapId][(int)((Player->getBottom() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3)
		{
			Player->centerX = (MAP_START_POINT_X + (MindexX + 1) * SIZE_OF_MAPWIDTH) + CharaW / 2;
		}
	}
	else if (Player->XStat == RIGHT && (MAP_START_POINT_X + (MindexX + 2) * SIZE_OF_MAPWIDTH) >= Player->getRight())
	{
		MindexX = (Player->getRight() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
		if (matrix[mapId][(int)((Player->getTop() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3
			|| matrix[mapId][(int)((Player->getBottom() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3)
		{
			Player->centerX = (MAP_START_POINT_X + (MindexX) * SIZE_OF_MAPWIDTH) - CharaW / 2;
		}
	}
}
*/
void Map::Collision(Character* Player)
{
	int MindexY = (Player->getBottom() - MAP_START_POINT_Y - 1) / SIZE_OF_MAPHEIGHT;
	int MindexX = (Player->getLeft() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
	if (Player->YStat == DOWN && (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) <= Player->getBottom())
	{
		if (matrix[mapId][MindexY + 1][(int)((Player->getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 2
			|| matrix[mapId][MindexY + 1][(int)((Player->getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 2)
		{
			Player->centerY = (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) - CharaH / 2;
			Player->vy = 0;
			Player->jumpNum = 2;
		}
		else if (matrix[mapId][MindexY + 1][(int)((Player->getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3
			|| matrix[mapId][MindexY + 1][(int)((Player->getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3)
		{
			Player->centerY = (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) - CharaH / 2;
			Player->vy = 0;
			Player->jumpNum = 2;
		}
		if (Player->XStat == LEFT && (MAP_START_POINT_X + (MindexX)*SIZE_OF_MAPWIDTH) <= Player->getLeft())
		{
			if (matrix[mapId][MindexY][MindexX] == 3)
			{
				Player->centerX = (MAP_START_POINT_X + (MindexX + 1) * SIZE_OF_MAPWIDTH) + CharaW / 2;
			}
		}
		else if (Player->XStat == RIGHT && (MAP_START_POINT_X + (MindexX + 2) * SIZE_OF_MAPWIDTH) >= Player->getRight())
		{
			MindexX = (Player->getRight() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
			if (matrix[mapId][MindexY][MindexX] == 3)
			{
				Player->centerX = (MAP_START_POINT_X + (MindexX)*SIZE_OF_MAPWIDTH) - CharaW / 2;
			}
		}
	}
	else if (Player->YStat == UP && (MAP_START_POINT_Y + (MindexY)*SIZE_OF_MAPHEIGHT) <= Player->getTop()) // 블록 3은 안올라가짐
	{
		MindexX = (Player->getLeft() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
		if (matrix[mapId][MindexY - 1][(int)((Player->getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3
			|| matrix[mapId][MindexY - 1][(int)((Player->getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 3)
		{
			if ((MAP_START_POINT_Y + (MindexY)*SIZE_OF_MAPHEIGHT) == (int)Player->getTop())
			{
				Player->centerY = (MAP_START_POINT_Y + (MindexY)*SIZE_OF_MAPHEIGHT) + CharaH / 2;
				Player->vy = 0;
			}
		}
		if (Player->XStat == LEFT && (MAP_START_POINT_X + (MindexX)*SIZE_OF_MAPWIDTH) <= Player->getLeft())
		{
			if (matrix[mapId][(int)((Player->getTop() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3
				|| matrix[mapId][(int)((Player->getBottom() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3)
			{
				Player->centerX = (MAP_START_POINT_X + (MindexX + 1) * SIZE_OF_MAPWIDTH) + CharaW / 2;
			}
		}
		else if (Player->XStat == RIGHT && (MAP_START_POINT_X + (MindexX + 2) * SIZE_OF_MAPWIDTH) >= Player->getRight())
		{
			MindexX = (Player->getRight() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
			if (matrix[mapId][(int)((Player->getTop() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3
				|| matrix[mapId][(int)((Player->getBottom() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3)
			{
				Player->centerX = (MAP_START_POINT_X + (MindexX)*SIZE_OF_MAPWIDTH) - CharaW / 2;
			}
		}
	}
	else if (Player->XStat == LEFT && (MAP_START_POINT_X + (MindexX)*SIZE_OF_MAPWIDTH) <= Player->getLeft())
	{
		if (matrix[mapId][(int)((Player->getTop() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3
			|| matrix[mapId][(int)((Player->getBottom() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3)
		{
			Player->centerX = (MAP_START_POINT_X + (MindexX + 1) * SIZE_OF_MAPWIDTH) + CharaW / 2;
		} 
	}
	else if (Player->XStat == RIGHT && (MAP_START_POINT_X + (MindexX + 2) * SIZE_OF_MAPWIDTH) >= Player->getRight())
	{
		MindexX = (Player->getRight() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
		if (matrix[mapId][(int)((Player->getTop() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3
			|| matrix[mapId][(int)((Player->getBottom() - (float)MAP_START_POINT_Y) / (float)SIZE_OF_MAPHEIGHT)][MindexX] == 3)
		{
			Player->centerX = (MAP_START_POINT_X + (MindexX)*SIZE_OF_MAPWIDTH) - CharaW / 2;
		}
	}
}

void Map::openNextStage()
{
	can_NextStage = true; // 실험을 위해 항상 오픈상태로 만듦
	if (can_NextStage)//true면 실행
	{
		matrix[mapId][mapSizeNow.y - 1][mapSizeNow.x - 1] = DoorOpen;
	}
	else
	{
		matrix[mapId][mapSizeNow.y - 1][mapSizeNow.x - 1] = DoorClose;
	}
}

void Map::changer(RECT winRect)
{
	srand((int)time(NULL));

	mapId = rand()%2;//rand
	stageCount++;
	can_NextStage = false;
	mapSizeNow.x = getWidth(mapId); // 해당 맵의 너비
	mapSizeNow.y = getHeight(mapId); // 해당 맵의 높이
	MaxSize = { borderX, 100, 2 * borderX - 100, (winRect.bottom - winRect.top - 100) };
	MAP_START_POINT_X = MaxSize.left;
	MAP_START_POINT_Y = MaxSize.top;
	SIZE_OF_MAPWIDTH = (MaxSize.right - MaxSize.left) / mapSizeNow.x;
	SIZE_OF_MAPHEIGHT = (MaxSize.bottom - MaxSize.top) / mapSizeNow.y;
	borderX = mapSizeNow.x * SIZE_OF_MAPWIDTH + MAP_START_POINT_X; // 맵 전체 너비
	borderY = mapSizeNow.y * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y; // 맵 전체 높이
}

void Map::changeAnimetion(HDC hdc, RECT winRect, float delta)
{
	stageMoveSpeed += delta * STAGE_MOVE_SPEED;
	buff = sin(stageMoveSpeed) * (borderX - MAP_START_POINT_X);
	MaxSize.left -= buff;
	MaxSize.right -= buff;

	MAP_START_POINT_X = MaxSize.left;
	MAP_START_POINT_Y = MaxSize.top;
	SIZE_OF_MAPWIDTH = (MaxSize.right - MaxSize.left) / mapSizeNow.x;
	SIZE_OF_MAPHEIGHT = (MaxSize.bottom - MaxSize.top) / mapSizeNow.y;
	borderX = mapSizeNow.x * SIZE_OF_MAPWIDTH + MAP_START_POINT_X; // 맵 전체 너비
	borderY = mapSizeNow.y * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y; // 맵 전체 높이
	drawBorder(hdc);
	drawObject(hdc);
	MaxSize.left += buff;
	MaxSize.right += buff;

	if (sin(stageMoveSpeed) >= 0.99)
	{
		changedAnime = false;
		Reset(winRect);
	}
}