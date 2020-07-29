#include "Chara.h"

Character::Character(int width, int height)
{
	centerX = 200 + width / 2;
	centerY = 200 + height / 2;
	vx = 10;
	vy = 0;
	Heart = 10;
	Shield = 3;
	JumpPower = 30;
	AttackSpeed = 1;
	Weapon = 0;
}

/*
void Character::is_valid(RECT *point)
{
	point->left = centerX - CharaW;
	point->right = centerX + CharaW;
	point->bottom = centerY + CharaH;
	point->top = centerY - CharaH;
}
*/

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
	DeleteObject(NewPen); // 펜 해제
}

void Character::Jump(HDC hdc, float delta) // 미완
{	
	//JumpPower = JumpPower - 0.1 * delta;
	JumpPower = 400 * delta;
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		JumpedY = centerY - 90; // 90이 점프 높이 결정
	}
	if (JumpedY < centerY)
	{
		clear(hdc);
		centerY = centerY - JumpPower;
		vy = 0;
	}
	if (JumpedY >= centerY)
	{
		JumpedY = 10000;
		update(hdc, delta);
	}
}

void Character::update(HDC hdc, float delta)
{
	clear(hdc);
	vy = vy + 0.098 * delta;
	centerY = centerY + vy;
}