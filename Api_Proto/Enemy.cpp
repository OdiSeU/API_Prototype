#include "Enemy.h"

Enemy::Enemy(Map* PlayGround, int JumpP)
{
	MapWithE = PlayGround;
	Way.makeNode(MapWithE); Way.autoLink(MapWithE, JumpP);
}

void Enemy::PushEnemy(float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb)
{
	EnemWithPath buffer; buffer.foe.SetSpec(speed, jumppower, jumpnum, heart, rgb);
	buffer.foe.SetSpawn(x, y);
	EnemyList.push_back(buffer);
}

void Enemy::StackEnemy(float x, float y, int speed, int jumppower, int jumpnum, int heart, COLORREF rgb)
{
	WaitingEnem.push_back(Character(x, y, speed, jumppower, jumpnum, heart, rgb));
}

bool Enemy::StacktoPush(int x, int y)
{
	if (WaitingEnem.size() == 0)
	{
		return false;
	}
	Character* buffer = &WaitingEnem.back();
	PushEnemy(buffer->centerX, buffer->centerY, buffer->CHARACTERSPEED, buffer->JumpPower, buffer->jumpNum, buffer->MaxHeart, buffer->Color);
	EnemyList.back().foe.SetSpawn(MapWithE->getBlockCenterX(x), MapWithE->getBlockCenterY(y));
	WaitingEnem.pop_back();
	return true;
}

void Enemy::KillEnemy()
{
	for (int i = 0; i < EnemyList.size(); i++)
	{
		if (EnemyList[i].foe.MaxHeart <= 0)
		{
			EnemyList.erase(EnemyList.begin() + i--);
		}
	}
}

void Enemy::UpdatePath(HDC bufferDC, POINT CharainMap)
{
	POINT EnemyinMap;
	for (int i = 0; i < EnemyList.size(); i++)
	{
		EnemyinMap.x = MapWithE->xToCol(EnemyList[i].foe.centerX);
		EnemyinMap.y = MapWithE->yToRow(EnemyList[i].foe.centerY);
		int EnemyinMapRight = MapWithE->xToCol(EnemyList[i].foe.getRight());
		int EnemyinMapLeft = MapWithE->xToCol(EnemyList[i].foe.getLeft());
		if (EnemyList[i].Result.size() >= 1)
		{
			POINT CurBrick = Way.getGnode(EnemyList[i].Result.back().getParent())->getCord();
			POINT NextBrick = Way.getGnode(EnemyList[i].Result.back().getCur())->getCord();
			if (EnemyinMap.x == CurBrick.x)
			{
				if (EnemyList[i].Result.back().getState() == JUMP)
				{
					if (EnemyList[i].foe.centerX < MapWithE->getBlockCenterX(CurBrick.x) && EnemyList[i].foe.XStat == RIGHT)
					{
						EnemyList[i].foe.MVRight(bufferDC);
					}
					else if (EnemyList[i].foe.centerX > MapWithE->getBlockCenterX(CurBrick.x) && EnemyList[i].foe.XStat == LEFT)
					{
						EnemyList[i].foe.MVLeft(bufferDC);
					}
					else
					{
						if (EnemyinMapLeft == CurBrick.x && EnemyinMapRight == CurBrick.x)
						{
							EnemyList[i].foe.MVJump(bufferDC);
						}
						if (EnemyinMap.x < NextBrick.x)
						{
							EnemyList[i].foe.MVRight(bufferDC);
						}
						else if (EnemyinMap.x > NextBrick.x)
						{
							EnemyList[i].foe.MVLeft(bufferDC);
						}
					}
				}
				else if (EnemyList[i].Result.back().getState() == WALK)
				{
					if (EnemyinMap.x < NextBrick.x)
					{
						EnemyList[i].foe.MVRight(bufferDC);
					}
					else if (EnemyinMap.x > NextBrick.x)
					{
						EnemyList[i].foe.MVLeft(bufferDC);
					}
				}
				else if (EnemyList[i].Result.back().getState() == DROP)
				{
					if (EnemyinMap.x > NextBrick.x)
					{
						EnemyList[i].foe.MVLeft(bufferDC);
					}
					else
					{
						EnemyList[i].foe.MVRight(bufferDC);
					}
				}
				MapWithE->Collision(&EnemyList[i].foe);
			}
			else if (EnemyinMap.x == NextBrick.x && EnemyinMap.y == NextBrick.y)
			{
				EnemyList[i].Result.pop_back();
			}
			else if (EnemyList[i].foe.vy == 0)
			{
				int Rindex = -1;
				for (int j = 0; j < EnemyList[i].Result.size(); j++)
				{
					POINT RinMap = Way.getGnode(EnemyList[i].Result[j].getCur())->getCord();
					if (RinMap.x == EnemyinMap.x && RinMap.y == EnemyinMap.y)
					{
						Rindex = j; break;
					}
				}
				int Roop = EnemyList[i].Result.size() - Rindex - 1;
				while (Roop > 0 && Rindex != -1)
				{
					EnemyList[i].Result.pop_back();
					Roop--;
				}
				if (Rindex == -1)
				{
					if (Way.getNodeIndex(EnemyinMap) != -1) // Result에 없는데 적이 갈 수 있는 길이다.
					{
						EnemyList[i].Result.clear();
						Way.AstarAlgorithm(CharainMap, EnemyinMap, &EnemyList[i].Result);
					}
					else // Result에 없는데 적도 갈 수 없는 곳
					{
						if (EnemyList[i].foe.XStat == LEFT)
						{
							EnemyList[i].foe.MVLeft(bufferDC);
						}
						else
						{
							EnemyList[i].foe.MVRight(bufferDC);
						}
					}
				}
			}
			else // 징검다리 건너려면 있는게 좋음
			{
				if (CharainMap.x != EnemyinMap.x)
				{
					if (EnemyList[i].foe.XStat == LEFT)
					{
						EnemyList[i].foe.MVLeft(bufferDC);
					}
					else
					{
						EnemyList[i].foe.MVRight(bufferDC);
					}
					MapWithE->Collision(&EnemyList[i].foe);
				}
			}
		}
		else if (EnemyList[i].Result.size() == 0)
		{
			if (EnemyinMap.x != CharainMap.x || EnemyinMap.y != CharainMap.y)
			{
				if (Way.getNodeIndex(CharainMap) != -1)
				{
					EnemyList[i].Result.clear();
					Way.AstarAlgorithm(CharainMap, EnemyinMap, &EnemyList[i].Result);
				}
			}
		}
	}
}

void Enemy::GetPath(POINT CharainMap)
{
	POINT EnemyinMap;
	for (int i = 0; i < EnemyList.size(); i++)
	{
		EnemyinMap.x = MapWithE->xToCol(EnemyList[i].foe.centerX);
		EnemyinMap.y = MapWithE->yToRow(EnemyList[i].foe.centerY);
		int Cindex = Way.getNodeIndex(CharainMap);
		int Eindex = Way.getNodeIndex(EnemyinMap);
		if (Cindex != -1 && Eindex != -1)
		{
			EnemyList[i].Result.clear();
			Way.AstarAlgorithm(CharainMap, EnemyinMap, &EnemyList[i].Result);
		}
		else if (Eindex != -1 && Cindex == -1)
		{
			POINT CharaLeft; CharaLeft.x = CharainMap.x - 1; CharaLeft.y = CharainMap.y;
			POINT CharaRight; CharaRight.x = CharainMap.x + 1; CharaRight.y = CharainMap.y;
			if (Way.getNodeIndex(CharaLeft) != -1)
			{
				EnemyList[i].Result.clear();
				Way.AstarAlgorithm(CharaLeft, EnemyinMap, &EnemyList[i].Result);
			}
			else if (Way.getNodeIndex(CharaRight) != -1)
			{
				EnemyList[i].Result.clear();
				Way.AstarAlgorithm(CharaRight, EnemyinMap, &EnemyList[i].Result);
			}
		}
	}
}

void Enemy::FillEnem(int Stagenum, Character MobAry[][MAX_MOBARY], int col, int row)
{
	int i = 0; Character* A;
	while (MobAry[Stagenum][i].jumpNum != -1)
	{
		A = &MobAry[Stagenum][i++];
		StackEnemy(A->centerX, A->centerY, A->CHARACTERSPEED, A->JumpPower, A->jumpNum, A->MaxHeart, A->Color);
	}
}