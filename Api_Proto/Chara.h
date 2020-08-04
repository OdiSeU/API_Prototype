#include <Windows.h>
#include "Weapon.h"

#pragma once
#define CharaW 20 // 캐릭터 너비
#define CharaH 39 // 캐릭터 높이
#define CHARACTERSPEED 250 // 캐릭터 좌우 속도
#define Gravity 1.9 // 중력
#define JumpP 0.8 // 점프 파워

enum {DOWN, UP, LEFT, RIGHT};

class Character
{
public:
	int jumpNum; // 점프 가능 횟수
	float centerX, centerY; // 중심 좌표
	float vx, vy; // 좌우 벡터
	float MVSpeed; // 캐릭터 좌우 속도
	int Heart; // 체력
	int Shield; // 방어막
	float JumpPower; // 점프 파워
	float AttackSpeed; // 공격 속도
	Weapon weapon; //무기
	short YStat; // 위 아래 상태
	short XStat; // 좌 우 상태

	// 진 수정
	bool isAttack; //공격 상태 체크
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

	// 진 수정
	void attackStart(HWND hwnd, unsigned int mx, unsigned int my);
	void CALLBACK attacking(HWND hwnd, unsigned int mx, unsigned int my);
	void attackEnd(HWND hwnd, unsigned int mx, unsigned int my);
	void throwStart(HWND hwnd, unsigned int mx, unsigned int my);
	void CALLBACK throwing(HWND hwnd, unsigned int mx, unsigned int my);
	void throwEnd(HWND hwnd, unsigned int mx, unsigned int my);
	// 진 수정
};