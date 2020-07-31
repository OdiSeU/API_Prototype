#include <Windows.h>

#pragma once
#define CharaW 30
#define CharaH 30
#define CHARACTERSPEED 200 // 캐릭 좌우 속도
#define Gravity 0.23 // 중력
#define JumpP 0.07 // 점프 파워

#define UP 1
#define DOWN 0

class Character
{
public:
	int jumpNum;
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
	float getTop() { return centerY - CharaH / 2; }
	float getBottom() { return centerY + CharaH / 2; }
	float getRight() { return centerX + CharaW / 2; }
	float getLeft() { return centerX - CharaW / 2; }
	void draw(HDC hdc);
	void MVRight(HDC hdc);
	void MVLeft(HDC hdc);
	void clear(HDC hdc);
	void update(HDC hdc, float delta);
};