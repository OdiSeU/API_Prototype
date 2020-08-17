#include <Windows.h>
#include <vector>
#include "Weapon.h"
#include "Proj.h"
using namespace std;

#pragma once

enum {DOWN, UP, LEFT, RIGHT};

class Character
{
public:
    int CharaW; // ĳ���� �ʺ�
    int CharaH; // ĳ���� ����
    int CHARACTERSPEED; // ĳ���� �¿� �ӵ� ����ġ
    int Gravity; // �߷�
    vector<Projectile> Thowable; // ����ü
    int Projnum; // ����ü ����
    int jumpNum; // ���� ���� Ƚ��
    float bfLeft, bfTop, bfBottom, bfRight; // ���� ��ǥ
    float centerX, centerY; // �߽� ��ǥ
    float vx, vy; // �¿� ����
    float MVSpeed; // ĳ���� �¿� �ӵ�
    int Heart; // ü��
    int Shield; // ��
    int JumpPower; // ���� �Ŀ�
    float delay; // 
    Weapon weapon;
    short YStat; // �� �Ʒ� ����
    short XStat; // �� �� ����
    COLORREF Color;
public:
    Character(float x = 500, float y = 500, int speed = 100, int jumppower = 10, int jumpnum = 1, int heart = 5, COLORREF rgb = RGB(0, 0, 0));
    float getTop() { return centerY - CharaH / 2; }
    float getBottom() { return centerY + CharaH / 2; }
    float getRight() { return centerX + CharaW / 2; }
    float getLeft() { return centerX - CharaW / 2; }
    void draw(HDC hdc);
    void MVRight(HDC hdc);
    void MVLeft(HDC hdc);
    void MVJump(HDC hdc);
    void KeyMVment(HDC bufferDC);
    void clear(HDC hdc);
    void Grav(HDC hdc, float delta);
    void UpdateProj(HDC hdc, float delta);
    void NextStagePosition(int x, int y);
    void SetSpawn(float x, float y);
    void SetSpec(int speed, int jumppower, int jumpnum, int heart, COLORREF rgb);
    void PastSaves();
};