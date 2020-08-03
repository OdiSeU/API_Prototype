#include <windows.h>
#include <cmath>
#include "Proj.h"

Projectile::Projectile(float Cx, float Cy, float Mx, float My, int type)
{
	Type = type;
	if (Type == Arrowhead)
	{
		pWidth = 8; pHeight = 8;
	}
	else if (Type == Ball)
	{
		pWidth = 15; pHeight = 15;
	}
	centerX = Cx; centerY = Cy;
	Distance = sqrt(pow(centerX - Mx, 2) + pow(centerY - My, 2));
	float Height = My - centerY;
	float Width = Mx - centerX;
	Sin = Height / Distance;
	Cos = Width / Distance;
	vx = Distance * Cos;
	vy = Distance * 0.005 * Sin;
}

void Projectile::setProj(float Cx, float Cy, float Mx, float My)
{
	centerX = Cx; centerY = Cy;
	Distance = sqrt(pow(centerX - Mx, 2) + pow(centerY - My, 2));
	float Height = My - centerY;
	float Width = Mx - centerX;
	Sin = Height / Distance;
	Cos = Width / Distance;
	vx = Distance * Cos;
	vy = Distance * 0.005 * Sin;
}

void Projectile::clear(HDC hdc)
{
	HPEN NewPen, OldPen;
	NewPen = CreatePen(PS_NULL, 3, RGB(255, 255, 255));
	OldPen = (HPEN)SelectObject(hdc, NewPen);
	Rectangle(hdc, centerX - 5, centerY - 5, centerX + 5, centerY + 5);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // ∆Ê «ÿ¡¶
}

void Projectile::Update(HDC hdc, float Deltatime)
{
	//clear(hdc);
	vy = vy + Gravi * Deltatime;
	centerY = (centerY + vy);
	centerX = centerX + vx * Deltatime;
}

void Projectile::draw(HDC hdc)
{
	Rectangle(hdc, centerX - pWidth/2, centerY - pHeight / 2, centerX + pWidth / 2, centerY + pHeight / 2);
}