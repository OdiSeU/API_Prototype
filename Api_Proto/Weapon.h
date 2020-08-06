#include "Motion.h"
#include <vector>
#pragma once

enum {Fist = 0, Sword, Arrow};

class Weapon
{
public:
	int Weapontype;
	std::vector<Motion> rangeArr;
	int combo;

	Weapon();
	void setWeaponType(int a, float Cx, float Cy);
	int getWeaponType();
	float getDealy();
	float getAtkSpeed();
	Motion getMotion();
};