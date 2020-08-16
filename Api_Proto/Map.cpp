#include <time.h>
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
	mapSizeNow.x = getWidth(mapId); // �ش� ���� �ʺ�
	mapSizeNow.y = getHeight(mapId); // �ش� ���� ����
	MaxSize = { 100, 100, (winRect.right - winRect.left - 100), (winRect.bottom - winRect.top - 100) };
	update();
	stageMoveSpeed = 0;
}

void Map::update()
{
	MAX_MAP_START_POINT_X = MaxSize.left;
	MAX_MAP_START_POINT_Y = MaxSize.top;
	SIZE_OF_MAPWIDTH = (MaxSize.right - MaxSize.left) / MAX_OF_MAPWIDTH;
	SIZE_OF_MAPHEIGHT = (MaxSize.bottom - MaxSize.top) / MAX_OF_MAPHEIGHT;

	MAP_START_POINT_X = MaxSize.left + ((MaxSize.right - MaxSize.left) - (SIZE_OF_MAPWIDTH * mapSizeNow.x)) / 2;
	MAP_START_POINT_Y = MaxSize.top + ((MaxSize.bottom - MaxSize.top) - (SIZE_OF_MAPHEIGHT * mapSizeNow.y)) / 2;

	maxBorderX = MAX_OF_MAPWIDTH * SIZE_OF_MAPWIDTH + MAX_MAP_START_POINT_X; // �Ҵ� �� ��ü �ʺ�
	maxBorderY = MAX_OF_MAPHEIGHT * SIZE_OF_MAPHEIGHT + MAX_MAP_START_POINT_Y; // �ִ� �� ��ü ����
	borderX = mapSizeNow.x * SIZE_OF_MAPWIDTH + MAP_START_POINT_X; // �� ��ü �ʺ�
	borderY = mapSizeNow.y * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y; // �� ��ü ����
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

void drawBackground(HDC hdc, RECT border, RECT winRect)
{
	HBRUSH NewB, OldB;
	NewB = (HBRUSH)GetStockObject(WHITE_BRUSH);
	OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, NULL_PEN);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);

	Rectangle(hdc, winRect.left, winRect.top, winRect.right, border.top); //���
	Rectangle(hdc, winRect.left, border.top, border.left, border.bottom); //����
	Rectangle(hdc, border.right, border.top, winRect.right, border.bottom); //����
	Rectangle(hdc, winRect.left, border.bottom, winRect.right, winRect.bottom); //����

	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // �� ����
}

void Map::drawBorder(HDC hdc)
{
	HBRUSH NewB, OldB;
	NewB = (HBRUSH)GetStockObject(WHITE_BRUSH);
	OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, NULL_PEN);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);

	int width = getWidth(mapId) * SIZE_OF_MAPWIDTH + MAP_START_POINT_X;
	int height = getHeight(mapId) * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y;
	Rectangle(hdc, MAX_MAP_START_POINT_X, MAX_MAP_START_POINT_Y, MaxSize.right, MaxSize.bottom);
	Rectangle(hdc, MAP_START_POINT_X, MAP_START_POINT_Y, borderX, borderY);

	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // �� ����
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
				rgb = RGB(255, 0, 0);
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
				rgb = RGB(0, 100, 0);
				drawRect(hdc, i, j, rgb);
				break;

			default:
				break;
			}
		}
	}
}

void Map::drawRect(HDC hdc, int x, int y, COLORREF rgb)
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
	DeleteObject(NewPen); // �� ����
}

void Map::Collision(Character* Player)
{
	if (MAP_START_POINT_X > Player->getLeft()) // ���� �� ����
	{
		Player->centerX = MAP_START_POINT_X + Player->CharaW / 2;
	}
	if (MAP_START_POINT_Y > Player->getTop()) // õ�� ����
	{
		Player->centerY = MAP_START_POINT_Y + Player->CharaH / 2;
		Player->vy = 0;
	}
	if (borderX < Player->getRight()) // ������ �� ����
	{
		Player->centerX = borderX - Player->CharaW / 2;
	}
	if (borderY < Player->getBottom()) // �ٴ� ����
	{
		Player->centerY = borderY - Player->CharaH / 2;
		Player->vy = 0;
		Player->jumpNum = 2;
	}
	
	if (Player->YStat == UP) // ���� �̵�
	{
		int bfRow = yToRow(Player->bfTop); // ���� ĳ���� ������ǥ ���
		int nowRow = yToRow(Player->getTop()); // ���� ĳ���� ������ǥ ���
		int nowCol = xToCol(Player->centerX); // ���� ĳ���� �߽���ǥ ���
		int nowLeftCol = xToCol(Player->getLeft()); // ���� ĳ���� ������ǥ ���
		int nowRightCol = xToCol(Player->getRight()); // ���� ĳ���� ��������ǥ ���

		if (bfRow != nowRow) // ĳ���� ������ǥ ��Ͽ� ��ȭ�� ���� ���
		{
			//���� ĳ���� ������ǥ ��Ϻ��� ������ǥ ��ϱ��� ���� �ִ��� �˻� ��
			for (int row = bfRow - 1; row >= nowRow; row--)
			{
				// �� �߰� �ÿ� �÷��̾� ��ǥ ���� & �÷��̾� VectorY ����
				if (getBlockType(row, nowLeftCol) == NonPassFloor ||
					getBlockType(row, nowRightCol) == NonPassFloor)
				{
					Player->centerY = getBlockBottom(row, nowCol) + Player->CharaH / 2 + 0.1;
					Player->vy = 0;
					break;
				}
			}
		}
	}
	if (Player->YStat == DOWN) // �Ʒ��� �̵�
	{
		int bfRow = yToRow(Player->bfBottom);
		int nowRow = yToRow(Player->getBottom());

		int nowCol = xToCol(Player->centerX);
		int nowLeftCol = xToCol(Player->getLeft());
		int nowRightCol = xToCol(Player->getRight());

		if (bfRow != nowRow)
		{
			for (int row = bfRow + 1; row <= nowRow; row++)
			{
				// �� �߰� �ÿ� �÷��̾� ��ǥ ���� & �÷��̾� VectorY ���� & �÷��̾� ���� Ƚ�� �ʱ�ȭ
				if (getBlockType(row, nowLeftCol) == NonPassFloor || getBlockType(row, nowLeftCol) == PassFloor ||
					getBlockType(row, nowRightCol) == NonPassFloor || getBlockType(row, nowRightCol) == PassFloor
					)
				{
					Player->centerY = getBlockTop(row, nowCol) - Player->CharaH / 2 - 0.1;
					Player->vy = 0;
					Player->jumpNum = 2;
					break;
				}
			}
		}
	}
	if (Player->XStat == LEFT) // ���� �̵�
	{
		int bfCol = xToCol(Player->bfLeft); // ���� ĳ���� ������ǥ ���
		int nowCol = xToCol(Player->getLeft()); // ���� ĳ���� ������ǥ ���
		int nowRow = yToRow(Player->centerY); // ���� ĳ���� �߽���ǥ ���
		int nowTopRow = yToRow(Player->getTop()); // ���� ĳ���� ������ǥ ���
		int nowBottomRow = yToRow(Player->getBottom()); // ���� ĳ���� �Ʒ��� ���

		if (bfCol != nowCol) // ĳ���� ������ǥ ��Ͽ� ��ȭ�� ���� ���
		{
			//���� ĳ���� ������ǥ ��Ϻ��� ������ǥ ��ϱ��� ���� �ִ��� �˻� ��
			for (int col = bfCol - 1; col >= nowCol; col--)
			{
				// �� �߰� �ÿ� �÷��̾� ��ǥ ����
				if (getBlockType(nowTopRow, col) == NonPassFloor ||
					getBlockType(nowBottomRow, col) == NonPassFloor)
				{
					Player->centerX = getBlockRight(nowRow, col) + Player->CharaW / 2 + 0.1;
					break;
				}
			}
		}
	}
	if (Player->XStat == RIGHT) // ������ �̵�
	{
		int bfCol = xToCol(Player->bfRight);
		int nowCol = xToCol(Player->getRight());
		int nowRow = yToRow(Player->centerY);
		int nowTopRow = yToRow(Player->getTop());
		int nowBottomRow = yToRow(Player->getBottom());

		if (bfCol != nowCol)
		{
			for (int col = bfCol + 1; col <= nowCol; col++)
			{
				if (getBlockType(nowTopRow, col) == NonPassFloor ||
					getBlockType(nowBottomRow, col) == NonPassFloor)
				{
					Player->centerX = getBlockLeft(nowRow, col) - Player->CharaW / 2 - 0.1;
					break;
				}
			}
		}
	}
}

void Map::openNextStage()
{
	can_NextStage = true; // ������ ���� �׻� ���»��·� ����
	if (can_NextStage)//true�� ����
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

	mapId = rand() % 2;//rand
	stageCount++;
	can_NextStage = false;
	mapSizeNow.x = getWidth(mapId); // �ش� ���� �ʺ�
	mapSizeNow.y = getHeight(mapId); // �ش� ���� ����
	MaxSize = { maxBorderX, MAX_MAP_START_POINT_Y, 2 * maxBorderX - MAX_MAP_START_POINT_X, (winRect.bottom - winRect.top - MAX_MAP_START_POINT_Y) };
	update();
}

void Map::changeAnimetion(HDC hdc, RECT winRect, float delta)
{
	stageMoveSpeed += delta * STAGE_MOVE_SPEED;
	buff = sin(stageMoveSpeed) * (maxBorderX - MAX_MAP_START_POINT_X);
	MaxSize.left -= buff;
	MaxSize.right -= buff;

	update();
	drawBorder(hdc);
	drawObject(hdc);
	MaxSize.left += buff;
	MaxSize.right += buff;

	if (sin(stageMoveSpeed) >= 0.9999)
	{
		changedAnime = false;
		Reset(winRect);
	}
}

void Map::ProjColl(HDC hdc, Character* Player)
{
	for (int i = 0; i < Player->Thowable.size(); i++)
	{
		int MindexY = (Player->Thowable[i].getCentY() - MAP_START_POINT_Y - 1) / SIZE_OF_MAPHEIGHT;
		int MindexX = (Player->Thowable[i].getCentX() - MAP_START_POINT_X - 1) / SIZE_OF_MAPWIDTH;
		if (MAP_START_POINT_X > Player->Thowable[i].getLeft()
			|| MAP_START_POINT_Y > Player->Thowable[i].getTop()
			|| borderX < Player->Thowable[i].getRight()
			|| borderY < Player->Thowable[i].getBottom())
		{
			//Player->Thowable[i].clear(hdc);
			Player->Thowable.erase(Player->Thowable.begin() + i--);
			Player->Projnum++;
		}
		else if (matrix[mapId][MindexY][MindexX] == 3)
		{
			Player->Thowable.erase(Player->Thowable.begin() + i--);
			Player->Projnum++;
		}
	}
}

int Map::getBlockLeft(int row, int col)
{
	return MAP_START_POINT_X + col * SIZE_OF_MAPWIDTH;
}
int Map::getBlockTop(int row, int col)
{
	return MAP_START_POINT_Y + row * SIZE_OF_MAPHEIGHT;
}
int Map::getBlockBottom(int row, int col)
{
	return MAP_START_POINT_Y + (row + 1) * SIZE_OF_MAPHEIGHT;
}
int Map::getBlockRight(int row, int col)
{
	return MAP_START_POINT_X + (col + 1) * SIZE_OF_MAPWIDTH;
}

int Map::getBlckCenterX(int col)
{
	return MAP_START_POINT_X + col * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH / 2;
}

int Map::getBlockType(int row, int col)
{
	return matrix[mapId][row][col];
}

int Map::xToCol(float x)
{
	return ((int)x - MAP_START_POINT_X) / SIZE_OF_MAPWIDTH;
}

int Map::yToRow(float y)
{
	return ((int)y - MAP_START_POINT_Y) / SIZE_OF_MAPHEIGHT;
}