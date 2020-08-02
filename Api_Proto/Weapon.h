#include "Shape.h"
#include "Chara.h"

#pragma once

enum { Fist = 0, Sword, Arrow };

class Weapon : public Shape
{
    int Weapontype;
    Shape Range;
public:
    Weapon() { Weapontype = Fist; Range.setZero(); }
    int setWeapontype(int a, Character player, int combo = 0)
    {
        Weapontype = a;
        if (Weapontype == Fist)
        {
            Range.setRect(player.centerX + 10, player.centerY, 3, 3, 0);
        }
        else if (Weapontype == Sword)
        {
            if (combo == 0)
            {
                Range.setSector(player.centerX + 10, player.centerY, 12, 120, 0);
            }
            else if (combo == 1)
            {
                Range.setSector(player.centerX + 10, player.centerY, 12, 30, -30);
            }
            else if (combo == 2)
            {
                Range.setRect(player.centerX + 10, player.centerY, 1, 10, 0);
            }
        }
        else if (Weapontype == Arrow)
        {
            Range.setZero();
        }
    }
    int getWeapon()
    {
        return Weapontype;
    }
    Shape getShape()
    {
        return Range;
    }
};