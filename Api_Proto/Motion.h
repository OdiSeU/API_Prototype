#include <Windows.h>
#include <cmath>
#pragma once

#define PI 3.14159265358979323846

class Motion
{
public:
	char shape;
	float centerX, centerY; // ������ �߽�
	float Hwidth, Hheight; // �簢�� �ʺ� ������, ���� ������
	float Radius;
	float startAngle, endAngle; // ��ä�� ����, �� ����
	float Tilt; // ���� pi / 180	
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
	void setRect(float X, float Y, float Hw, float Hh, float tilt) // �簢��
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
		Delay = 0;
		AtkSpeed = 0;
	}
	void setSector(float X, float Y, float Rad, float sA, float eA) // ��ä��
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
		Delay = 0;
		AtkSpeed = 0;
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
		Delay = 0;
		AtkSpeed = 0;
	}
};