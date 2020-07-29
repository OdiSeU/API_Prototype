#include "Chara.h"

Character::Character(int width, int height)
{
	centerX = 200 + width / 2;
	centerY = 200 + height / 2;
	vx = 10;
	vy = 0;
	Heart = 10;
	Shield = 3;
	JumpPower = 400;
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