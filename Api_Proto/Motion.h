#include <Windows.h>
#include <cmath>
#pragma once

#define PI 3.14159265358979323846

class Motion
{
public:
<<<<<<< Updated upstream:Api_Proto/Shape.h
    float centerX, centerY; // 도형의 중심
    float Hwidth, Hheight; // 사각형 너비 반지름, 높이 반지름
    float Radius;
    float startAngle, endAngle; // 부채꼴 시작, 끝 라디안
    float Tilt; // 라디안 pi / 180
    Shape()
    {
        centerX = 0;
        centerY = 0;
        Hwidth = 0;
        Hheight = 0;
        Radius = 0;
        startAngle = 0;
        endAngle = 0;
        Tilt = 0;
    }
    void setRect(float X, float Y, float Hw, float Hh, float tilt) // 사각형
    {
        centerX = X;
        centerY = Y;
        Hwidth = Hw;
        Hheight = Hh;
        Radius = 0;
        startAngle = 0;
        endAngle = 0;
        Tilt = (tilt * PI) / 180;
    }
    void setSector(float X, float Y, float Rad, float sA, float eA) // 부채꼴
    {
        centerX = X;
        centerY = Y;
        Hwidth = 0;
        Hheight = 0;
        Radius = Rad;
        startAngle = (sA * PI) / 180;
        endAngle = (eA * PI) / 180;
        Tilt = 0;
    }
    void setZero()
    {
        centerX = 0;
        centerY = 0;
        Hwidth = 0;
        Hheight = 0;
        Radius = 0;
        startAngle = 0;
        endAngle = 0;
        Tilt = 0;
    }
=======
	char shape;
	float centerX, centerY; // 도형의 중심
	float Hwidth, Hheight; // 사각형 너비 반지름, 높이 반지름
	float Radius;
	float startAngle, endAngle; // 부채꼴 시작, 끝 라디안
	float Tilt; // 라디안 pi / 180	
	float Delay;
	float AtkSpeed;
	Motion()
	{
		shape = ' ';
		centerX = 0;
		centerY = 0;
		Hwidth = 0;
		Hheight = 0;
		Radius = 0;
		startAngle = 0;
		endAngle = 0;
		Tilt = 0;
		Delay = 0;
		AtkSpeed = 0;
	}
	void setRect(float X, float Y, float Hw, float Hh, float tilt) // 사각형
	{
		shape = 'r';
		centerX = X;
		centerY = Y;
		Hwidth = Hw;
		Hheight = Hh;
		Radius = 0;
		startAngle = 0;
		endAngle = 0;
		Tilt = (tilt * PI) / 180;
	}
	void setSector(float X, float Y, float Rad, float sA, float eA) // 부채꼴
	{
		shape = 's';
		centerX = X;
		centerY = Y;
		Hwidth = 0;
		Hheight = 0;
		Radius = Rad;
		startAngle = (sA * PI) / 180;
		endAngle = (eA * PI) / 180;
		Tilt = 0;
	}
	void setZero()
	{
		shape = ' ';
		centerX = 0;
		centerY = 0;
		Hwidth = 0;
		Hheight = 0;
		Radius = 0;
		startAngle = 0;
		endAngle = 0;
		Tilt = 0;
	}
>>>>>>> Stashed changes:Api_Proto/Motion.h
};