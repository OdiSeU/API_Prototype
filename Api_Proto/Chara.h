#include <Windows.h>
#include "Weapon.h"

#pragma once
#define CharaW 20 // ĳ���� �ʺ�
#define CharaH 39 // ĳ���� ����
#define CHARACTERSPEED 250 // ĳ���� �¿� �ӵ�
#define Gravity 1.9 // �߷�
#define JumpP 0.8 // ���� �Ŀ�

enum {DOWN, UP, LEFT, RIGHT};

class Character
{
public:
	int jumpNum; // ���� ���� Ƚ��
	float centerX, centerY; // �߽� ��ǥ
	float vx, vy; // �¿� ����
	float MVSpeed; // ĳ���� �¿� �ӵ�
	int Heart; // ü��
	int Shield; // ��
	float JumpPower; // ���� �Ŀ�
	float AttackSpeed; // ���� �ӵ�
	Weapon weapon; //����
	short YStat; // �� �Ʒ� ����
	short XStat; // �� �� ����

	// �� ����
	bool isAttack; //���� ���� üũ
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

	// �� ����
	void attackStart(HWND hwnd, unsigned int mx, unsigned int my);
	void CALLBACK attacking(HWND hwnd, unsigned int mx, unsigned int my);
	void attackEnd(HWND hwnd, unsigned int mx, unsigned int my);
	void throwStart(HWND hwnd, unsigned int mx, unsigned int my);
	void CALLBACK throwing(HWND hwnd, unsigned int mx, unsigned int my);
	void throwEnd(HWND hwnd, unsigned int mx, unsigned int my);
	// �� ����
};