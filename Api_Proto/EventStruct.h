#pragma once
#include "Weapon.h"

typedef struct _EventStruct
{
	float px, py;    //플레이어 위치
	float mx, my;    //마우스 위치
	float leftTime; //시작까지 남은시간
	float progTime;    //진행 시간
	Weapon weapon;    //무기 객체
}EventStruct;