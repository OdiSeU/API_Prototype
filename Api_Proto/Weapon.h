#include "Motion.h"
#include <vector>
#pragma once

enum {Fist = 0, Sword, Arrow};

class Weapon
{
public:
	int Weapontype;
	int damage;
	std::vector<Motion> rangeArr;
	float tilt;
	int combo;

	Weapon();
	void setWeaponType(int type);
	void setWeaponPos(float direct, float px, float py);
	int getWeaponType();
	float getDelay();
	float getAtkSpeed();
	float getRange();
	Motion getMotion();
	void addCombo();
};