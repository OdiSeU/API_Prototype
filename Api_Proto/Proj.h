#include <windows.h>
#include <cmath>

#pragma once
class Projectile
{
	POINT Tcord;
	short Type;
	float Distance;
	float Height;
	float vx, vy;
public:
	Projectile(short type, POINT player, POINT mouse);
	void Update(float Deltatime);
};