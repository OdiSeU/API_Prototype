#include <Windows.h>

#pragma once
#define CharaW 30
#define CharaH 30
#define CHARACTERSPEED 200
#define JUMPHEIGHT 50

#define JUMPUP 1
#define JUMPDOWN 0

static float JumpedY = 10000; // а║гагр y

class Character
{
public:
	float centerX, centerY;
	float vx, vy;
	float MVSpeed;
	int Heart;
	int Shield;
	float JumpPower;
	float AttackSpeed;
	int Weapon;
	short MVStat;

public:
	Character(int, int);
	//void is_valid(RECT* point);
	float getTop() { return centerY - CharaH / 2; }
	float getBottom() { return centerY + CharaH / 2; }
	float getRight() { return centerX + CharaW / 2; }
	float getLeft() { return centerX - CharaW / 2; }
	void draw(HDC hdc);
	void MVRight(HDC hdc);
	void MVLeft(HDC hdc);
	void clear(HDC hdc);
	void update(HDC hdc, float delta);
	void Jump(HDC hdc, float delta);
};