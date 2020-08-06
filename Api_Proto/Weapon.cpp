#include "Weapon.h"

<<<<<<< Updated upstream
Weapon::Weapon() { Weapontype = Fist; Range.setZero(); Delay = 0.3; Attacked = 1; }
=======
Weapon::Weapon() {
	Weapontype = Fist;
	Motion range;
	range.setZero();
	range.Delay = 0.3;
	range.AtkSpeed = 0.5;
	rangeArr.clear();
	rangeArr.push_back(range);
}
>>>>>>> Stashed changes

void Weapon::setWeaponType(int a, float Cx, float Cy)
{
	Weapontype = a;
	Motion range;
	rangeArr.clear();
	switch (Weapontype)
	{
	case Fist:
		range.setRect(Cx + 10, Cy, 3, 3, 0);
		range.Delay = 0.3;
		range.AtkSpeed = 0.5;
		break;
	case Sword:
		range.setSector(Cx + 10, Cy, 12, 120, 0);
		range.Delay = 0.3;
		range.AtkSpeed = 0.5;
		rangeArr.push_back(range);

		range.setSector(Cx + 10, Cy, 12, 30, -30);
		range.Delay = 0.3;
		range.AtkSpeed = 0.5;
		rangeArr.push_back(range);

		range.setRect(Cx + 10, Cy, 1, 10, 0);
		range.Delay = 0.3;
		range.AtkSpeed = 0.5;
		rangeArr.push_back(range);
		break;
	case Arrow:
		range.setZero();
		range.Delay = 0.3;
		range.AtkSpeed = 0.5;
		rangeArr.push_back(range);
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
