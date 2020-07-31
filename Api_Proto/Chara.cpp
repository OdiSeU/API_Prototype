#include "Chara.h"

Character::Character(int width, int height)
{
	jumpNum = 2;
	jumped = false;
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
	DeleteObject(NewPen); // Ææ ÇØÁ¦
}

void Character::Jump(HDC hdc, float delta) // ¹Ì¿Ï
{	
	if (jumped == true)
	{
		if (JumpPower > 0)
		{
			JumpPower = JumpPower - 0.23 * delta;
			clear(hdc);
			centerY = centerY - JumpPower;
		}
		if (JumpPower <= 0)
		{
			update(hdc, delta);
		}
	}
}

void Character::update(HDC hdc, float delta)
{
	clear(hdc);
	vy = vy + Gravity * delta;
	centerY = centerY + vy;
}