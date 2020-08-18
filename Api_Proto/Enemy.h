#pragma once
#include <Windows.h>
#include <vector>
#include "Proj.h"
using namespace std;
#pragma once

#define EnemyW 20 // ĳ���� �ʺ�
#define EnemyH 39 // ĳ���� ����
#define ENEMYSPEED 250 // ĳ���� �¿� �ӵ�
#define EJumpP 0.4 // ���� �Ŀ�

enum { DOWN, UP, LEFT, RIGHT };

class Enemy
{
public:
	vector<Projectile> Thowable; // ����ü
	int Projnum; // ����ü ����
	int jumpNum; // ���� ���� Ƚ��
	float centerX, centerY; // �߽� ��ǥ
	float vx, vy; // �¿� ����
	float MVSpeed; // ĳ���� �¿� �ӵ�
	int Heart; // ü��
	float JumpPower; // ���� �Ŀ�
	float AttackSpeed; // ���� �ӵ�
	short YStat; // �� �Ʒ� ����
	short XStat; // �� �� ����
public:
	Enemy(int, int);
	float getTop() { return centerY - EnemyH / 2; }
	float getBottom() { return centerY + EnemyH / 2; }
	float getRight() { return centerX + EnemyW / 2; }
	float getLeft() { return centerX - EnemyW / 2; }
};