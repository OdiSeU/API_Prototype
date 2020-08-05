#include <Windows.h>
#include <vector>
#include "Proj.h"
using namespace std;

#pragma once

#define CharaW 20 // ĳ���� �ʺ�
#define CharaH 39 // ĳ���� ����
#define CHARACTERSPEED 250 // ĳ���� �¿� �ӵ�
#define Gravity 1.9 // �߷�
#define JumpP 0.6 // ���� �Ŀ�

enum {DOWN, UP, LEFT, RIGHT};

class Character
{
public:
	vector<Projectile> Thowable; // ����ü
	int Projnum; // ����ü ����
	int jumpNum; // ���� ���� Ƚ��
	float centerX, centerY; // �߽� ��ǥ
	float vx, vy; // �¿� ����
	float MVSpeed; // ĳ���� �¿� �ӵ�
	int Heart; // ü��
	int Shield; // ��
	float JumpPower; // ���� �Ŀ�
	float AttackSpeed; // ���� �ӵ�
	int Weapon;
	short YStat; // �� �Ʒ� ����
	short XStat; // �� �� ����
public:
	Character(int, int);
	float getTop() { return centerY - CharaH / 2; }
	float getBottom() { return centerY + CharaH / 2; }
	float getRight() { return centerX + CharaW / 2; }
	float getLeft() { return centerX - CharaW / 2; }
	void draw(HDC hdc);
	void MVRight(HDC hdc);
	void MVLeft(HDC hdc);
	void MVJump(HDC hdc);
	void clear(HDC hdc);
	void Grav(HDC hdc, float delta);
	void UpdateProj(HDC hdc, float delta);
	void NextStagePosition(int x, int y);
};