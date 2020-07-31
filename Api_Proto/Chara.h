#include <Windows.h>
#include <iostream>

#pragma once
#define CharaW 30
#define CharaH 30
#define CHARACTERSPEED 200
#define Gravity 0.23

class Character
{
public:
	int jumpNum;
	bool jumped;
	float centerX, centerY;
	float vx, vy;
	float MVSpeed;
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
	void MVRight(HDC hdc);
	void MVLeft(HDC hdc);
	void clear(HDC hdc);
	void update(HDC hdc, float delta);
	void Jump(HDC hdc, float delta);
};