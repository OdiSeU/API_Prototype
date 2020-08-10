#include <windows.h>
#include <cmath>

#pragma once

#define Gravi 35

enum {Arrowhead = 0, Ball};

class Projectile
{
	float centerX, centerY;
	short Type;
	float Sin, Cos;
	float Distance;
	float vx, vy;
	float pWidth, pHeight;
public:
	Projectile(float, float, float, float, int);
	void setProj(float, float, float, float);
	void Update(HDC hdc, float Deltatime);
	void clear(HDC hdc);
	void draw(HDC hdc);
	float getLeft() { return centerX - pWidth / 2; }
	float getRight() { return centerX + pWidth / 2; }
	float getTop() { return centerY - pHeight / 2; }
	float getBottom() { return centerY + pHeight / 2; }
	float getCentX() { return centerX; }
	float getCentY() { return centerY; }
};