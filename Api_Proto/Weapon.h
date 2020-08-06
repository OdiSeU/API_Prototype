#include <vector>
#include "Motion.h"

#pragma once

enum { Fist = 0, Sword, Arrow };

class Weapon
{
public:
	int Weapontype;
<<<<<<< Updated upstream
	Shape Range;
	float Delay;
	float Attacked;
=======
	vector<Motion> rangeArr;
	int combo;
public:
>>>>>>> Stashed changes
	Weapon();
	void setWeaponType(int a, float Cx, float Cy);
	int getWeaponType();
	float getDealy();
	float getAtkSpeed();
	Motion getMotion();
};