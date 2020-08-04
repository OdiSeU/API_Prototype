#include "Weapon.h"
#include "Shape.h"

Weapon::Weapon() { Weapontype = Fist; Range.setZero(); }

void Weapon::setWeapontype(int a, float Cx, float Cy, int combo)
{
	Weapontype = a;
	if (Weapontype == Fist)
	{
		Range.setRect(Cx + 10, Cy, 3, 3, 0);
		Delay = 0.3; Attacked = 0.5;
	}
	else if (Weapontype == Sword)
	{
		if (combo == 0)
		{
			Range.setSector(Cx + 10, Cy, 12, 120, 0);
			Delay = 0.3; Attacked = 0.5;
		}
		else if (combo == 1)
		{
			Range.setSector(Cx + 10, Cy, 12, 30, -30);
			Delay = 0.3; Attacked = 0.5;
		}
		else if (combo == 2)
		{
			Range.setRect(Cx + 10, Cy, 1, 10, 0);
			Delay = 0.3; Attacked = 0.5;
		}
	}
	else if (Weapontype == Arrow)
	{
		Range.setZero();
		Delay = 0.3; Attacked = 0.5;
	}
}

Shape Weapon::getShape()
{
	return Range;
}
