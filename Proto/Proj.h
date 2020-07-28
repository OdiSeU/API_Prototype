#pragma once

class Projectile
{
	POINT Tcord;
	short Type;
	float Distance;
	float Height;
	float vx, vy;
public:
	Projectile(short type, POINT player, POINT mouse)
	{
		Type = type;
		if (Type == 1)
		{
			Tcord.x = player.x;
			Tcord.y = player.y;
			Distance = sqrt(pow(player.x - mouse.x, 2) + pow(player.y - mouse.y, 2));
			Height = mouse.y - player.y;
			vx = 100;
			vy = Height / Distance;
		}
		else
		{

		}
	}
	void Update()
	{
		/*
		LARGE_INTEGER tTime;
		QueryPerformanceCounter(&tTime);
		g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
		g_tTime = tTime;
		*/
		Tcord.x = Tcord.x + vx * g_fDeltatime;
		Tcord.y = Tcord.y + vy * g_fDeltatime;
	}
};
