#include <vector>
#include "Chara.h"
#include "Proj.h"
using namespace std;

Character::Character(int width, int height)
{
	Projnum = 3;
	jumpNum = 2;
	centerX = 700 + width / 2;
	centerY = 500 + height / 2;
	vx = 10;
	vy = 0;
	Heart = 10;
	Shield = 3;
	JumpPower = JumpP;
	AttackSpeed = 1;
	YStat = NULL;
	XStat = NULL;
	MVSpeed = NULL;
}

void Character::draw(HDC hdc)
{
	Rectangle(hdc, getLeft(), getTop(), getRight(), getBottom());
	for (int i = 0;i < Thowable.size();i++)
	{
		Thowable[i].draw(hdc);
	}
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
	DeleteObject(NewPen); // 펜 해제
}

void Character::Grav(HDC hdc, float delta) // 중력
{
	vy = vy + Gravity * delta; // 중력 가속도
	if (vy >= 0)
	{
		YStat = DOWN;
	}
	//clear(hdc);
	centerY = centerY + vy;
}

void Character::UpdateProj(HDC hdc, float delta)
{
	for (int i = 0;i < Thowable.size();i++)
	{
		Thowable[i].Update(hdc, delta);
	}
}

void Character::NextStagePosition(int x, int y)
{
	centerX = x;
	centerY = y;
}