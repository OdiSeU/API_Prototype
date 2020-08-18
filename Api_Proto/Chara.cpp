#include "Chara.h"
using namespace std;

Character::Character(float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb)
{
	CharaW = 20; // 캐릭터 너비
	CharaH = 39; // 캐릭터 높이
	CHARACTERSPEED = speed; // 캐릭터 좌우 속도
	Gravity = 35; // 중력
	Projnum = 3;
	jumpNum = jumpnum;
	centerX = x + CharaW / 2;
	centerY = y + CharaH / 2;
	bfLeft = getLeft();
	bfTop = getTop();
	bfBottom = getBottom();
	bfRight = getRight();
	vx = 0;
	vy = 0;
	MaxHeart = 6; // 처음 생명력
	CurHeart = MaxHeart;
	Shield = 2;
	JumpPower = jumppower; // 점프 파워
	delay = 0;
	YStat = NULL;
	XStat = NULL;
	MVSpeed = NULL;
	Color = rgb;
}

/*
Character::Character(float x, float y)
{
	CharaW = 20; // 캐릭터 너비
	CharaH = 39; // 캐릭터 높이
	CHARACTERSPEED = 250; // 캐릭터 좌우 속도
	Gravity = 35; // 중력
	JumpP = 10; // 점프 파워
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
*/

void Character::draw(HDC hdc)
{
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(Color);
	HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, Color);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);
	Rectangle(hdc, getLeft(), getTop(), getRight(), getBottom());
	for (int i = 0;i < Thowable.size();i++)
	{
		Thowable[i].draw(hdc);
	}
	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen);
}

void Character::KeyMVment(HDC bufferDC)
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		MVLeft(bufferDC); // 왼쪽
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		MVRight(bufferDC); // 오른쪽	
	}
	if ((GetAsyncKeyState(VK_SPACE) & 0x0001))
	{
		MVJump(bufferDC); // 점프
	}
}

void Character::MVRight(HDC hdc)
{
	vx += MVSpeed;
	centerX = centerX + vx;
	XStat = RIGHT;
	vx = 0;
}

void Character::MVLeft(HDC hdc)
{
	vx -= MVSpeed;
	centerX = centerX + vx;
	XStat = LEFT;
	vx = 0;
}

void Character::MVJump(HDC hdc)
{
	if (jumpNum >= 1)
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

void Character::SetSpawn(float x, float y)
{
	centerX = x + CharaW / 2;
	centerY = y + CharaH / 2;
}

void Character::SetSpec(int speed, int jumppower, int jumpnum, int heart, COLORREF rgb)
{
	CharaW = 20; // 캐릭터 너비
	CharaH = 39; // 캐릭터 높이
	CHARACTERSPEED = speed; // 캐릭터 좌우 속도
	Gravity = 35; // 중력
	Projnum = 3;
	jumpNum = jumpnum;
	bfLeft = getLeft();
	bfTop = getTop();
	bfBottom = getBottom();
	bfRight = getRight();
	vx = 0;
	vy = 0;
	MaxHeart = heart; // 체력
	Shield = 0;
	JumpPower = jumppower; // 점프 파워
	delay = 0;
	YStat = NULL;
	XStat = NULL;
	MVSpeed = NULL;
	Color = rgb;
}

void Character::PastSaves() // 전 좌표 저장 (충돌처리)
{
	bfLeft = getLeft();
	bfTop = getTop();
	bfBottom = getBottom();
	bfRight = getRight();
}

void Character::TakeADamage(int getDamage) // 데미지를 받을 조건을 만족했을 때 이 함수를 불러오기
{
	if (Shield > 0)
	{
		Shield -= 1;
	}
	else
	{
		CurHeart -= getDamage;
	}
}