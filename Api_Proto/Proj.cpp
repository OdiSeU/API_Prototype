#include "Proj.h"

Projectile::Projectile(short type, POINT player, POINT mouse)
{
	Type = type;
	if (Type == 1)
	{
		Tcord.x = player.x;
		Tcord.y = player.y;
		Distance = sqrt(pow(player.x - mouse.x, 2) + pow(player.y - mouse.y, 2));
		Height = (int)mouse.y - player.y;
		vx = 100;
		vy = Height / Distance;
	}
	else
	{

	}
}

void Projectile::Update(float Deltatime)
{
	Tcord.x = Tcord.x + vx * Deltatime;
	Tcord.y = Tcord.y + vy * Deltatime;
}