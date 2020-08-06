#pragma once
#include <Windows.h>
#include <vector>
#include "Proj.h"
using namespace std;
#pragma once

#define EnemyW 20 // 캐릭터 너비
#define EnemyH 39 // 캐릭터 높이
#define ENEMYSPEED 250 // 캐릭터 좌우 속도
#define EJumpP 0.4 // 점프 파워

enum { DOWN, UP, LEFT, RIGHT };

class Enemy
{
public:
	vector<Projectile> Thowable; // 투사체
	int Projnum; // 투사체 숫자
	int jumpNum; // 점프 가능 횟수
	float centerX, centerY; // 중심 좌표
	float vx, vy; // 좌우 벡터
	float MVSpeed; // 캐릭터 좌우 속도
	int Heart; // 체력
	float JumpPower; // 점프 파워
	float AttackSpeed; // 공격 속도
	short YStat; // 위 아래 상태
	short XStat; // 좌 우 상태
public:
	Enemy(int, int);
	float getTop() { return centerY - EnemyH / 2; }
	float getBottom() { return centerY + EnemyH / 2; }
	float getRight() { return centerX + EnemyW / 2; }
	float getLeft() { return centerX - EnemyW / 2; }
};