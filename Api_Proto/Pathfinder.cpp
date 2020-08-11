#include "Pathfinder.h"

void Pathfinder::Linknode(int a, int b, int type)
{
	if (type == WALK)
	{
		getGnode(a)->getWalklist()->push_back(b);
	}
	else if (type == JUMP)
	{
		getGnode(a)->getJumplist()->push_back(b);
	}
	else if (type == DROP)
	{
		getGnode(a)->getDroplist()->push_back(b);
	}
}

void Pathfinder::GnodeReset(int i)
{
	Epath[i].setCord(-1, -1);
	Epath[i].getJumplist()->clear();
	Epath[i].getDroplist()->clear();
	Epath[i].getWalklist()->clear();
}

Gnode* Pathfinder::getGnode(int index) { return &Epath[index]; }

vector<BrickInfo>* Pathfinder::AstarAlgorithm(POINT chara, POINT enemy)
{
	vector<BrickInfo> Result; // ���� ��� ��� ����
	vector<BrickInfo> Closed;
	vector<BrickInfo> Opened;

	int Sindex = getNodeIndex(chara);
	int Eindex = getNodeIndex(enemy);
	Closed.push_back(BrickInfo(Sindex, Sindex, 0, 0, -1));
	Pushway(Sindex, 0, &Opened, &Closed, enemy);
	while (Opened.size() > 0)
	{
		auto mIter = min_element(Opened.begin(), Opened.end());
		BrickInfo Smallone = *mIter;
		if (getGnode(Smallone.getCur())->getCord().x == enemy.x && getGnode(Smallone.getCur())->getCord().y == enemy.y)
		{
			Closed.push_back(Smallone);
			break;
		}
		Closed.push_back(Smallone);
		Opened.erase(mIter);
		Pushway(Smallone.getCur(), Smallone.getG(), &Opened, &Closed, enemy);
	}
	if (Opened.size() == 0)
	{
		return NULL; // ���� ����
	}
	// �� ������
	int track = Eindex;
	while (track != Sindex)
	{
		for (int i = 0; i < Closed.size(); i++)
		{
			if (Closed[i].getCur() == track)
			{
				Result.push_back(Closed[i]);
				track = Closed[i].getParent();
				Closed.erase(Closed.begin() + i); break;
			}
		}
	}
	// ���� �� Ž���ϸ鼭 �ൿ ����
	/*
	for (int i = Result.size() - 1; i >= 0; i--)
	{
		int Curin = Result[i].getCur(), Parent = Result[i].getParent();
		if (Result[i].getState() == WALK)
		{
			// �ȱ� �ൿ
		}
		else if (Result[i].getState() == JUMP)
		{
			// ���� �ൿ
		}
		else if (Result[i].getState() == DROP)
		{
			// �������� �ൿ
		}
		// ������ ��x��ǥ ���̷� ����
	}
	*/
	return &Result;
}

void Pathfinder::Pushway(int index, int CurG, vector<BrickInfo>* Opened, vector<BrickInfo>* Closed, POINT END)
{
	vector<int> buffer; // �ε��� �����
	vector<short> status; // ���� �����
	int Gvalue;
	for (int i = 0; i < getGnode(index)->getJumplist()->size(); i++)
	{
		buffer.push_back(getGnode(index)->getJumplist()->at(i));
		status.push_back(JUMP);
	}
	for (int i = 0; i < getGnode(index)->getDroplist()->size(); i++)
	{
		buffer.push_back(getGnode(index)->getDroplist()->at(i));
		status.push_back(DROP);
	}
	for (int i = 0; i < getGnode(index)->getWalklist()->size(); i++)
	{
		buffer.push_back(getGnode(index)->getWalklist()->at(i));
		status.push_back(WALK);
	}
	for (int i = 0; i < buffer.size(); i++) // ���� ����Ʈ Ž������
	{
		if (is_Closed(buffer[i], *Closed) == true)
		{
			buffer.erase(buffer.begin() + i);
			status.erase(status.begin() + i--);
		}
	}
	for (int i = 0; i < buffer.size(); i++)
	{
		int Oindex = is_Opened(buffer[i], *Opened);
		Gvalue = getG(getGnode(index)->getCord(), getGnode(buffer[i])->getCord());
		if (Oindex != -1) // ���¸���Ʈ�� �����ϸ�
		{
			if (Opened->at(Oindex).getG() > Gvalue + CurG)
			{
				Opened->at(Oindex).modInfo(index, Gvalue + CurG, status[i]);
			}
		}
		else
		{
			Opened->push_back(BrickInfo(buffer[i], index, Gvalue + CurG, getH(getGnode(buffer[i])->getCord(), END), status[i]));
		}
	}
}