#include <vector>
#include "Chara.h"
#include "Proj.h"
using namespace std;

Character::Character(float x, float y)
{
	CharaW = 20; // ĳ���� �ʺ�
	CharaH = 39; // ĳ���� ����
	CHARACTERSPEED = 250; // ĳ���� �¿� �ӵ�
	Gravity = 35; // �߷�
	JumpP = 10; // ���� �Ŀ�
	Projnum = 3;
	jumpNum = 2;
	centerX = x + CharaW / 2;
	centerY = y + CharaH / 2;
	bfLeft = getLeft();
	bfTop = getTop();
	bfBottom = getBottom();
	bfRight = getRight();
	vx = 0;
	vy = 0;
	Heart = 10;
	Shield = 3;
	JumpPower = JumpP;
	delay = 0;
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
		//clear(hdc);
		vx += MVSpeed;
		centerX = centerX + vx;
		XStat = RIGHT;
		vx = 0;
}

void Character::MVLeft(HDC hdc)
{
	//clear(hdc);
	vx -= MVSpeed;
	centerX = centerX + vx;
	XStat = LEFT;
	vx = 0;
}

void Character::MVJump(HDC hdc)
{
	vy = -JumpPower;
	jumpNum--;
	YStat = UP;
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