#include <Windows.h>
#include <vector>
#include "Chara.h"
#include "Pathfinder.h"
using namespace std;

#pragma once

typedef struct _EandP
{
	Character foe;
	vector<BrickInfo> Result;
}EnemWithPath;

class Enemy
{
public:
	vector<EnemWithPath> EnemyList;
	Pathfinder Way;
	Map* MapWithE;
	Enemy(Map* PlayGround, int JumpP = 2);
	void PushEnemy(float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb);
	void KillEnemy();
	void UpdatePath(HDC bufferDC, POINT CharainMap);
	void Collision_E()
	{
		for (int i = 0; i < EnemyList.size(); i++)
		{
			MapWithE->Collision(&EnemyList[i].foe);
		}
	}
	void Draw_E(HDC buffer)
	{
		for (int i = 0; i < EnemyList.size(); i++)
		{
			EnemyList[i].foe.draw(buffer);
		}
	}
	void PastSaves()
	{
		for (int i = 0; i < EnemyList.size(); i++)
		{
			EnemyList[i].foe.bfLeft = EnemyList[i].foe.getLeft();
			EnemyList[i].foe.bfTop = EnemyList[i].foe.getTop();
			EnemyList[i].foe.bfBottom = EnemyList[i].foe.getBottom();
			EnemyList[i].foe.bfRight = EnemyList[i].foe.getRight();
		}
	}
	void Grav(HDC buffer, float FIX)
	{
		for (int i = 0; i < EnemyList.size(); i++)
		{
			EnemyList[i].foe.Grav(buffer, FIX);
		}
	}
	void SpeedSet(float FIX)
	{
		for (int i = 0; i < EnemyList.size(); i++)
		{
			EnemyList[i].foe.MVSpeed = EnemyList[i].foe.CHARACTERSPEED * FIX;
		}
	}
	void NodeChanger(int JumpP = 2)
	{
		Way.makeNode(MapWithE); Way.autoLink(MapWithE, JumpP);
	}
	void GetPath(POINT CharainMap);
};