#include "Weapon.h"

Weapon::Weapon() {
	setWeaponType(Sword);
	damage = 1;
	combo = 0;
}
void Weapon::setWeaponType(int type) {
	Motion range;
	rangeArr.clear();
	Weapontype = type;
	switch (Weapontype)
	{
	case Fist:
		damage = 1;
		range.setRect(0, 0, 15, 9, 0);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);
		break;
	case Sword:
		damage = 2;
		range.setSector(0, 0, 36, -120, 0);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);

		range.setSector(0, 0, 36, -30, 30);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);

		range.setRect(0, 0, 30, 1, 0);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);
		break;
	case Arrow:
		range.setZero();
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);
		break;
	}
}

void Weapon::setWeaponPos(float direct, float px, float py)
{
	switch (Weapontype)
	{
	case Fist:
		rangeArr[0].centerX = px + direct * 10;
		rangeArr[0].centerY = py;
		break;
	case Sword:
		rangeArr[0].centerX = px + direct * 10;
		rangeArr[0].centerY = py;
		rangeArr[1].centerX = px + direct * 10;
		rangeArr[1].centerY = py;
		rangeArr[2].centerX = px + direct * rangeArr[2].Hwidth;
		rangeArr[2].centerY = py;
		break;
	case Arrow:
		rangeArr[0].centerX = px;
		rangeArr[0].centerY = py;
		break;
	}
}

int Weapon::getWeaponType()
{
	return Weapontype;
}

float Weapon::getDelay()
{
	return rangeArr[combo].Delay;
}

float Weapon::getAtkSpeed()
{
	return rangeArr[combo].AtkSpeed;
}

float Weapon::getRange()
{
	if (rangeArr[combo].shape == 'r') return rangeArr[combo].Hwidth * 2;
	if (rangeArr[combo].shape == 's') return rangeArr[combo].Radius;
}

Motion Weapon::getMotion()
{
	return rangeArr[combo];
}

void Weapon::addCombo()
{
	combo = (combo + 1) % rangeArr.size();
}