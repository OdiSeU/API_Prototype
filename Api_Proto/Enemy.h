#include <Windows.h>
#include <vector>
#include "Chara.h"
#include "Pathfinder.h"
using namespace std;

#pragma once

#define Stages 5
#define MAX_MOBARY 20

typedef struct EnemWithPath
{
	Character foe;
	vector<BrickInfo> Result;
};

class Enemy
{
public:
	int idCnt;
	vector<Character> WaitingEnem; // 적 대기자
	vector<EnemWithPath> EnemyList; // 활동 중인 적
	Pathfinder Way; // 길찾기 알고리즘
	Map* MapWithE; // 맵 포인터
	Enemy(Map* MapWithE, int JumpP = 2);
	void PushEnemy(float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb);
	void StackEnemy(float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb);
	void KillEnemy(vector<EventStruct>* eventList);
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
		for (int i = 0; i < Gnode_size; i++)
		{
			Way.GnodeReset(i);
		}
		Way.makeNode(MapWithE); Way.autoLink(MapWithE, JumpP);
	}
	void GetPath(POINT CharainMap);
	void ShowNode(HDC bufferDC)
	{
		POINT pnt, pnt2;
		vector<int>* linker;
		for (int i = 0; i < Way.Size; i++)
		{
			pnt = Way.Epath[i].getCord();
			Ellipse(bufferDC, pnt.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X, pnt.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y,
				(pnt.x + 1) * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X, (pnt.y + 1) * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y);

			linker = Way.Epath[i].getWalklist();
			for (int i = 0; i < linker->size(); i++)
			{
				int result = linker->at(i);
				pnt2 = Way.Epath[result].getCord();
				MoveToEx(bufferDC, pnt.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X + MapWithE->SIZE_OF_MAPWIDTH / 2, pnt.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y + MapWithE->SIZE_OF_MAPHEIGHT / 2, NULL);
				LineTo(bufferDC, pnt2.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X + MapWithE->SIZE_OF_MAPWIDTH / 2, pnt2.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y + MapWithE->SIZE_OF_MAPHEIGHT / 2);
			}

			linker = Way.Epath[i].getJumplist();
			for (int i = 0; i < linker->size(); i++)
			{
				int result = linker->at(i);
				pnt2 = Way.Epath[result].getCord();
				MoveToEx(bufferDC, pnt.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X + MapWithE->SIZE_OF_MAPWIDTH / 2, pnt.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y + MapWithE->SIZE_OF_MAPHEIGHT / 2 - 10, NULL);
				LineTo(bufferDC, pnt2.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X + MapWithE->SIZE_OF_MAPWIDTH / 2, pnt2.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y + MapWithE->SIZE_OF_MAPHEIGHT / 2 - 10);
			}

			linker = Way.Epath[i].getDroplist();
			for (int i = 0; i < linker->size(); i++)
			{
				int result = linker->at(i);
				pnt2 = Way.Epath[result].getCord();
				MoveToEx(bufferDC, pnt.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X + MapWithE->SIZE_OF_MAPWIDTH / 2, pnt.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y + MapWithE->SIZE_OF_MAPHEIGHT / 2 + 10, NULL);
				LineTo(bufferDC, pnt2.x * MapWithE->SIZE_OF_MAPWIDTH + MapWithE->MAP_START_POINT_X + MapWithE->SIZE_OF_MAPWIDTH / 2, pnt2.y * MapWithE->SIZE_OF_MAPHEIGHT + MapWithE->MAP_START_POINT_Y + MapWithE->SIZE_OF_MAPHEIGHT / 2 + 10);
			}
		}
	}
	bool StacktoPush(int x, int y);
	void FillEnem(int Stagenum, Character MobAry[][MAX_MOBARY], int col, int row); // 대기 중인 적 리스트에 해당 스테이지 적 모두 추가

};