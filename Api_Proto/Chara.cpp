#include "Chara.h"
using namespace std;

Character::Character(int id, float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb)
{
	this->id = id;
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
	Heart = heart;
	Shield = 0;
	JumpPower = jumppower; // 점프 파워
	delay = 0;
	YStat = NULL;
	XStat = NULL;
	MVSpeed = NULL;
	Color = rgb;
	isInvincible = false;
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

void Character::SetSpec(int id, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb)
{
	this->id = id;
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
	Heart = heart; // 체력
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

void Character::newAttackEvent(float mx, float my, vector<EventStruct>* eventList)
{
	if (delay > 0) return;
	delay = weapon.getDelay() + weapon.getAtkSpeed();
	weapon.tilt = atan2(mx - centerX, my - centerY);
	EventStruct e;
	e.id = id;
	e.eType = ATTACK;
	e.angle = atan2(mx - centerX, my - centerY);
	e.leftTime = weapon.getDelay();
	e.progTime = weapon.getAtkSpeed();
	e.subject = this;
	weapon.addCombo();
	eventList->push_back(e);
}

void Character::newDamagedEvent(int dmg, vector<EventStruct>* eventList)
{
	if (isInvincible) return;
	Heart -= dmg;
	delay = 0.5;
	EventStruct e;
	e.id = id;
	e.eType = DAMAGED;
	e.angle = 0;
	e.leftTime = 0;
	e.progTime = 0.5;
	e.subject = this;
	eventList->push_back(e);
}

float Character::getDistance(Character target)
{
	float dx = target.centerX - centerX;
	float dy = target.centerY - centerY;
	  
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

bool Character::isCollideWith(Character target)
{
	Motion atkMotion = weapon.getMotion();
	if (atkMotion.shape == 'r')
	{
		if (atkMotion.centerX - atkMotion.Hwidth < target.getRight() &&
			atkMotion.centerX + atkMotion.Hwidth > target.getLeft() &&
			atkMotion.centerY - atkMotion.Hheight < target.getBottom() &&
			atkMotion.centerY + atkMotion.Hheight > target.getTop())
			return true;
		else
			return false;
	}
	if (atkMotion.shape == 's')
	{
		float cX, cY, startX, startY, midX, midY, endX, endY;
		if (sin(weapon.tilt) < 0)
		{
			startX = -cos(atkMotion.endAngle);
			startY = sin(atkMotion.endAngle);
			midX = -cos(atkMotion.endAngle + atkMotion.startAngle);
			midY = sin(atkMotion.endAngle + atkMotion.startAngle);
			endX = -cos(atkMotion.startAngle);
			endY = sin(atkMotion.startAngle);
		}
		else
		{
			startX = cos(atkMotion.startAngle);
			startY = sin(atkMotion.startAngle);
			midX = cos(atkMotion.endAngle + atkMotion.startAngle);
			midY = sin(atkMotion.endAngle + atkMotion.startAngle);
			endX = cos(atkMotion.endAngle);
			endY = sin(atkMotion.endAngle);
		}
		cX = atkMotion.centerX;
		cY = atkMotion.centerY;
		startX = atkMotion.centerX + atkMotion.Radius * startX;
		startY = atkMotion.centerY - atkMotion.Radius * startY;
		midX = atkMotion.centerX + atkMotion.Radius * midX;
		midY = atkMotion.centerX + atkMotion.Radius * midY;
		endX = atkMotion.centerX + atkMotion.Radius * endX;
		endY = atkMotion.centerY - atkMotion.Radius * endY;

		if (target.isDotinMe(cX, cY)) return true;
		else if (target.isDotinMe(startX, startY)) return true;
		else if (target.isDotinMe(midX, midY)) return true;
		else if (target.isDotinMe(endX, endY)) return true;
		//else if ()
	}
}

bool Character::isDotinMe(float x, float y)
{
	if (getLeft() < x && x < getRight() &&
		getTop() < y && y < getBottom())
	{
		return true;
	}
	else
		return false;
}