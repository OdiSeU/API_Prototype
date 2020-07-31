#include "Chara.h"
#include "Map.h"
Character::Character(int width, int height)
{
	jumpNum = 2;
	centerX = 500 + width / 2;
	centerY = 500 + height / 2;
	vx = 10;
	vy = 0;
	Heart = 10;
	Shield = 3;
	JumpPower = JumpP;
	AttackSpeed = 1;
	Weapon = 0;
	MVStat = NULL;
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
	}
}

void Character::MVLeft(HDC hdc)
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		clear(hdc);
		centerX = centerX - MVSpeed;
	}
}

void Character::clear(HDC hdc)
{
	HPEN NewPen, OldPen;
	NewPen = CreatePen(PS_NULL, 3, RGB(255, 255, 255));
	OldPen = (HPEN)SelectObject(hdc, NewPen);

	Rectangle(hdc, getLeft() - 1, getTop() - 1, getRight() + 1, getBottom() + 1);

	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // ∆Ê «ÿ¡¶
}

void Character::update(HDC hdc, float delta)
{
	clear(hdc);
	vy = vy + Gravity * delta;
	centerY = centerY + vy;
}