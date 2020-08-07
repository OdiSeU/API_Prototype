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
	void setWeaponType(int type);
	void setWeaponPos(float x, float y);
	int getWeaponType();
	float getDealy();
	float getAtkSpeed();
	Motion getMotion();
	void addCombo();
};