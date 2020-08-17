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
    int CharaW; // 캐릭터 너비
    int CharaH; // 캐릭터 높이
    int CHARACTERSPEED; // 캐릭터 좌우 속도 고정치
    int Gravity; // 중력
    vector<Projectile> Thowable; // 투사체
    int Projnum; // 투사체 숫자
    int jumpNum; // 점프 가능 횟수
    float bfLeft, bfTop, bfBottom, bfRight; // 이전 좌표
    float centerX, centerY; // 중심 좌표
    float vx, vy; // 좌우 벡터
    float MVSpeed; // 캐릭터 좌우 속도
    int Heart; // 체력
    int Shield; // 방어막
    int JumpPower; // 점프 파워
    float delay; // 
    Weapon weapon;
    short YStat; // 위 아래 상태
    short XStat; // 좌 우 상태
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