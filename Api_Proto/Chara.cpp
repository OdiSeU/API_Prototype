#include "Chara.h"
#include "Map.h"
Character::Character(int width, int height)
{
	jumpNum = 2;
	centerX = 700 + width / 2;
	centerY = 500 + height / 2;
	vx = 10;
	vy = 0;
	Heart = 10;
	Shield = 3;
	JumpPower = JumpP;
	AttackSpeed = 1;
	Weapon = 0;
	YStat = NULL;
	XStat = NULL;
	MVSpeed = NULL;
}

void Character::draw(HDC hdc)
{
	Rectangle(hdc, getLeft(), getTop(), getRight(), getBottom());
}

void Character::MVRight(HDC hdc)
{
	if (GetAsyncKeyState('D') & 0x8000)
	{
		clear(hdc);
		centerX = centerX + MVSpeed;
		XStat = RIGHT;
	}
}

void Character::MVLeft(HDC hdc)
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		clear(hdc);
		centerX = centerX - MVSpeed;
		XStat = LEFT;
	}
}

void Character::MVJump(HDC hdc)
{
	if ((GetAsyncKeyState(VK_SPACE) & 0x0001) && jumpNum >= 1)
	{
		vy = -JumpPower;
		jumpNum--;
		YStat = UP;
	}
}

void Character::clear(HDC hdc)
{
	HPEN NewPen, OldPen;
	NewPen = CreatePen(PS_NULL, 3, RGB(255, 255, 255));
	OldPen = (HPEN)SelectObject(hdc, NewPen);

	Rectangle(hdc, getLeft() - 1, getTop() - 1, getRight() + 1, getBottom() + 1);

	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // �� ����
}

void Character::Grav(HDC hdc, float delta) // �߷�
{
	vy = vy + Gravity * delta; // �߷� ���ӵ�
	if (vy >= 0)
	{
		YStat = DOWN;
	}
	clear(hdc);
	centerY = centerY + vy;
}
//�Źμ� �߰�
void Character::NextStagePosition(int x, int y)
{
	centerX = x;
	centerY = y;
}