#include "Chara.h"
#include "Map.h"
#include <time.h>
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
	weapon = Weapon();
	YStat = NULL;
	XStat = NULL;
	MVSpeed = NULL;

<<<<<<< Updated upstream
	// 진 수정
	isAttack = false;
	// 진 수정
=======
	isAttack = false;
>>>>>>> Stashed changes
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
	DeleteObject(NewPen); // 펜 해제
}

void Character::Grav(HDC hdc, float delta) // 중력
{
	vy = vy + Gravity * delta; // 중력 가속도
	if (vy >= 0)
	{
		YStat = DOWN;
	}
	clear(hdc);
	centerY = centerY + vy;
}

// 진 수정
void Character::attackStart(HWND hwnd, unsigned int mx, unsigned int my) {
	if (!isAttack) {
		SetTimer(hwnd, 1, weapon->Delay, attacking);
		//animation
	}
}
<<<<<<< Updated upstream
void CALLBACK Character::attacking(HWND hwnd, unsigned int mx, unsigned int my) {

}
void Character::attackEnd(HWND hwnd, unsigned int mx, unsigned int my) {
	
}
void Character::throwStart(HWND hwnd, unsigned int mx, unsigned int my) {
	
}
void Character::throwing(HWND hwnd, unsigned int mx, unsigned int my) {

}
void Character::throwEnd(HWND hwnd, unsigned int mx, unsigned int my) {

}
// 진 수정
=======

void Character::attackStart(HWND hwnd, unsigned int mx, unsigned int my) 
{
	if (!isAttack) {
		isAttack = true;
		SetTimer(hwnd, 1, weapon.Delay, (TIMERPROC)timerProc(hwnd, NULL, NULL, NULL, ));
		//animation
	}
}

void Character::attackEnd(HWND hwnd, unsigned int mx, unsigned int my) 
{
	isAttack = false;
}


void CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime, Character target)
{

}
>>>>>>> Stashed changes
