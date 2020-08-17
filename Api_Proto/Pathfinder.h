#include "Chara.h"
#include "Map.h"
#include <algorithm>

#pragma once

#define Gnode_size 100
enum { WALK, JUMP, DROP };

class Gnode // 가중치는 실제 좌표대로 계산
{
	POINT Mapcord;
	vector<int> Jumplist;
	vector<int> Droplist;
	vector<int> Walklist;
public:
	Gnode(int x = -1, int y = -1) { Mapcord.x = x; Mapcord.y = y; }
	POINT getCord() { return Mapcord; }
	void setCord(int x, int y) { Mapcord.x = x; Mapcord.y = y; }
	vector<int>* getJumplist() { return &Jumplist; }
	vector<int>* getDroplist() { return &Droplist; }
	vector<int>* getWalklist() { return &Walklist; }
};

class BrickInfo
{
	int F, G, H;
	int Cur, Parent;
	short State;
public:
	BrickInfo(int buffer, int parent, int g, int h, short k) { Cur = buffer; Parent = parent; G = g; H = h; F = G + H; State = k; }
	int getCur() { return Cur; }
	int getParent() { return Parent; }
	int getF() { return F; }
	int getG() { return G; }
	int getH() { return H; }
	short getState() { return State; }
	void modInfo(int parent, int g, short s) { Parent = parent; G = g; F = G + H; State = s; }
};

inline bool operator<(BrickInfo a, BrickInfo b)
{
	return a.getF() < b.getF();
}

class Pathfinder
{
public:
	int Size;
	Gnode Epath[Gnode_size];
	Pathfinder() {}
	Pathfinder(Map playground, int jumpP) 
	{ 
		makeNode(&playground); autoLink(&playground, jumpP); 
	}
	void GnodeReset(int);
	Gnode* getGnode(int index);
	int getG(POINT Cur, POINT a)
	{
		return abs(a.x - Cur.x) * 5 + abs(a.y - Cur.y) * 10;
	}
	int getH(POINT Cur, POINT End)
	{
		return abs(End.x - Cur.x) * 5 + abs(End.y - Cur.y) * 10;
	}
	bool is_Closed(int index, vector<BrickInfo> Closed)
	{
		for (int i = 0; i < Closed.size(); i++)
		{
			if (Closed[i].getCur() == index)
			{
				return true;
			}
		}
		return false;
	}
	int is_Opened(int index, vector<BrickInfo> Opened)
	{
		for (int i = 0; i < Opened.size(); i++)
		{
			if (Opened[i].getCur() == index)
			{
				return i;
			}
		}
		return -1;
	}
	int getNodeIndex(POINT point)
	{
		for (int i = 0; i < Size; i++)
		{
			if (point.x == getGnode(i)->getCord().x && point.y == getGnode(i)->getCord().y)
			{
				return i;
			}
		}
		return -1;
	}
	void Linknode(int a, int b, int type); // a에서 선이 뻗어 b를 연결
	vector<BrickInfo>* AstarAlgorithm(POINT chara, POINT enemy, vector<BrickInfo>*);
	void makeNode(Map* map);
	void autoLink(Map* map, int jumpPower);
	void Pushway(int index, int CurG, vector<BrickInfo>* Opened, vector<BrickInfo>* Closed, POINT END);
};


