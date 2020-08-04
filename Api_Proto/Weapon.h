#include "Shape.h"

#pragma once

enum { Fist = 0, Sword, Arrow };

class Weapon
{
	int Weapontype;
	Shape Range;
	float Delay;
	float Attacked;
public:
	Weapon();
	void setWeapontype(int a, float Cx, float Cy, int combo = 0);
	int getWeapon()
	{
		return Weapontype;
	}
	Shape getShape();
};