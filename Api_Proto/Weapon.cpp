#include "Weapon.h"

Weapon::Weapon() {
	setWeaponType(Sword);
}
void Weapon::setWeaponType(int type) {
	Motion range;
	rangeArr.clear();
	Weapontype = type;
	switch (Weapontype)
	{
	case Fist:
		range.setRect(0, 0, 3, 3, 0);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);
		break;
	case Sword:
		range.setSector(0, 0, 12, -120, 0);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);

		range.setSector(0, 0, 12, -30, 30);
		range.Delay = 0.1;
		range.AtkSpeed = 0.3;
		rangeArr.push_back(range);

		range.setRect(0, 0, 10, 1, 0);
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

void Weapon::setWeaponPos(float x, float y)
{
	switch (Weapontype)
	{
	case Fist:
		rangeArr[0].centerX = x+10;
		rangeArr[0].centerY = y;
		break;
	case Sword:
		rangeArr[0].centerX = x+10;
		rangeArr[0].centerY = y;
		rangeArr[1].centerX = x+10;
		rangeArr[1].centerY = y;
		rangeArr[2].centerX = x+10;
		rangeArr[2].centerY = y;
		break;
	case Arrow:
		rangeArr[0].centerX = x;
		rangeArr[0].centerY = y;
		break;
	}
}

int Weapon::getWeaponType()
{
	return Weapontype;
}

float Weapon::getDealy()
{
	return rangeArr[combo].Delay;
}

float Weapon::getAtkSpeed()
{
	return rangeArr[combo].AtkSpeed;
}

Motion Weapon::getMotion()
{
	return rangeArr[combo];
}

void Weapon::addCombo()
{
	combo = (combo + 1) % rangeArr.size();
}