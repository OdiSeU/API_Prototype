#include <Windows.h>

#pragma once
#define CharaW 100
#define CharaH 100

class Character
{
public:
	float centerX, centerY;
	float vx, vy;
	int MVSpeed;
	int Heart;
	int Shield;
	float JumpPower;
	float AttackSpeed;
	int Weapon;
public:
	Character(int, int);
	//void is_valid(RECT* point);
	float getTop() { return centerY - CharaH / 2; }
	float getBottom() { return centerY + CharaH / 2; }
	float getRight() { return centerX + CharaW / 2; }
	float getLeft() { return centerX - CharaW / 2; }
	void draw(HDC hdc);
};