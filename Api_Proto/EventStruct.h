#pragma once
#include "Weapon.h"

typedef struct _EventStruct
{
	float px, py;    //�÷��̾� ��ġ
	float mx, my;    //���콺 ��ġ
	float leftTime; //���۱��� �����ð�
	float progTime;    //���� �ð�
	Weapon weapon;    //���� ��ü
}EventStruct;