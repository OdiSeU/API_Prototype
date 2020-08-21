#include <windows.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#include "Proj.h"
#include "Chara.h"
#include "Resol.h"
#include "Map.h"
#include "Weapon.h"
#include "WindowScreen.h"
#include "Pathfinder.h"
#include "Enemy.h"
#include "UI.h"
using namespace std;

// Made by Minsu. Jinsung, Gyungjun in 2020-08-21

#pragma comment(lib, "winmm.lib")

Character Bruser(1, 0, 0, 150, 10, 1, 4, RGB(100, 100, 100)); // ����� ��
Character Ninja(1, 0, 0, 300, 10, 1, 4, RGB(0, 200, 250)); // ���� ��
Character Nerd(1, 0, 0, 120, 10, 1, 4, RGB(0, 100, 250)); // ��� ��
Character Ender(1, -1, -1, -1, -1, -1, -1, RGB(0, 0, 0)); // ��ħǥ

int Stagenum = 0;

Character StageMobAry[Stages][MAX_MOBARY] = // ���������� ���� ����
{
	{Nerd,Nerd,Nerd,Nerd,Bruser,Ninja,Ender},
	{Nerd,Bruser,Nerd,Nerd,Bruser,Nerd,Bruser,Ender},
	{Nerd,Bruser,Ninja,Ninja,Bruser,Ninja,Bruser,Ender},
	{Ninja,Ninja,Ninja,Ninja,Bruser,Bruser,Bruser,Ender},
	{Bruser,Ninja,Bruser,Ninja,Bruser,Ninja,Bruser,Ninja,Ender}
};

vector<EventStruct> eventList;    //�̺�Ʈ ó�� ����Ʈ

// ���� ����
bool g_bLoop = true;

// ���� ���۸� ��Ʈ�� ����
HBITMAP hbitmap, oldbitmap;

// �ð� ����
DWORD dwOldGameTime = 0, dwCurrentGameTime = 0;
FLOAT rDeltaTime = 0.0f, rAccumlationTime = 0.0f;

// ���� �ڵ�
HDC g_hDC;
HWND g_hWnd;

// �� ���� ����
Screen WindowScreen; //������ ȭ��  �Źμ� �߰�
Map PlayGround(WindowScreen.rect); // �� ����
Map oldMap(WindowScreen.rect);
RECT border = PlayGround.MaxSize;

// ó�� ���� ��ǥ(2��), �¿� �ӵ�, �����Ŀ�, ���� ���� Ƚ��, ����� ��
Character Player(0, PlayGround.getBlockCenterX(PlayGround.getWidth(0) / 2), PlayGround.getBlockCenterY(PlayGround.getHeight(0)/ 2), 250, 10, 2, 10, RGB(200, 0, 200));

// ���콺 ��ǥ
float mX, mY;

// ���� ��ȯ �����Ͱ� ��� ����Ʈ
Enemy FoeList(&PlayGround, 2);

// �ʿ����� �÷��̾� ��ǥ
POINT CharainMap;
POINT OldCharainMap;

#define FIXED 0.016f

/*
DWORD dwOldGameTime = 0, dwCurrentGameTime = 0;
DWORD dwOldGameTime = dwCurrentGameTime = ::timeGetTime();
FLOAT rDeltaTime = 0.0f, rAccumlationTime = 0.0f;

// DeltaTime�� ���� �����Ӱ� ���� �������� �ð� ������ �ǹ��ϰ�
// AccumlationTime�� DeltaTime�� ������ ���� �ǹ��մϴ�.

bool bGameLoop = true;
while (bGameLoop)
{
	dwCurrentGameTime = ::timeGetTime();
	rDeltaTime       = static_cast<float>(dwCurrentGameTime - dwOldGameTime) / 1000;
	dwOldGameTime    = dwCurrentGameTime;
	rAccumlationTime += rDeltaTime;

	if (rAccumlationTime > 0.016f) // 60FPS ����, 1 / 60.0f
	{
		rAccumlationTime = 0.0f;

		// ������ ���� ��ƾ�� ó���ϴ� �κ�
		OnUpdate();
		OnRender();
	}
}
*/

void attackStart(EventStruct target, HDC hdc)
{
	//PAINTSTRUCT ps;
	target.subject->weapon.setWeaponPos(sin(target.angle) < 0 ? -1 : 1, target.subject->centerX, target.subject->centerY);
	Motion motion = target.subject->weapon.getMotion();
	switch (target.subject->weapon.getWeaponType())
	{
	case Arrow:
		break;
	default:
		if (motion.shape == 'r')
		{
			Rectangle(hdc,
				motion.centerX - motion.Hwidth,
				motion.centerY - motion.Hheight,
				motion.centerX + motion.Hwidth,
				motion.centerY + motion.Hheight
			);
		}
		if (motion.shape == 's')
		{ 
			float startX, startY, endX, endY;
			if (sin(target.angle) < 0)
			{
				startX = -cos(motion.endAngle);
				startY = sin(motion.endAngle);
				endX = -cos(motion.startAngle);
				endY = sin(motion.startAngle);
			}
			else
			{
				startX = cos(motion.startAngle);
				startY = sin(motion.startAngle);
				endX = cos(motion.endAngle);
				endY = sin(motion.endAngle);
			}
			Pie(hdc,
				motion.centerX - motion.Radius,
				motion.centerY - motion.Radius,
				motion.centerX + motion.Radius,
				motion.centerY + motion.Radius,
				motion.centerX + motion.Radius * startX,
				motion.centerY - motion.Radius * startY,
				motion.centerX + motion.Radius * endX,
				motion.centerY - motion.Radius * endY
			);
		}
		// ���� ���ݹ��� �浹üũ �߰�
		if (target.subject == &Player)
		{
			for (int i = 0; i < FoeList.EnemyList.size(); i++)
			{
				if (target.subject->isCollideWith(FoeList.EnemyList[i].foe))
				{
					FoeList.EnemyList[i].foe.newDamagedEvent(target.subject->weapon.damage, &eventList);
				}
			}
		}
		else
		{
			if (target.subject->isCollideWith(Player))
			{
				Player.newDamagedEvent(target.subject->weapon.damage, &eventList);
			}
		}
		break;
	}
}

void damagedStart(EventStruct* target)
{
	target->subject->isInvincible = true;
	target->subject->Color = RGB(200, 0, 0);
}

void damagedEnd(EventStruct* target)
{
	target->subject->isInvincible = false;
	target->subject->Color = target->subject->OrgColor;
}

void Run()
{	
	dwCurrentGameTime = timeGetTime();
	rDeltaTime = (float)(dwCurrentGameTime - dwOldGameTime) / 1000;
	dwOldGameTime = dwCurrentGameTime;
	rAccumlationTime = rAccumlationTime + rDeltaTime;

	/*
	// �÷��̾��� ��ǥ
	OldCharainMap.x = CharainMap.x;
	OldCharainMap.y = CharainMap.y;
	CharainMap.x = PlayGround.xToCol(Player.centerX);
	CharainMap.y = PlayGround.yToRow(Player.centerY);
	*/

	if (CharainMap.x != PlayGround.xToCol(Player.centerX) || CharainMap.y != PlayGround.yToRow(Player.centerY))
	{
		OldCharainMap.x = CharainMap.x;
		OldCharainMap.y = CharainMap.y;
		CharainMap.x = PlayGround.xToCol(Player.centerX);
		CharainMap.y = PlayGround.yToRow(Player.centerY);
	}

	PlayGround.Collision(&Player);
	FoeList.Collision_E();

	if (Stagenum == 0)
	{
		FoeList.FillEnem(Stagenum, StageMobAry, MAX_MOBARY, 5);
		Stagenum++;
	}
	if ((CharainMap.x != OldCharainMap.x || CharainMap.y != OldCharainMap.y)) // ĳ���� ��ǥ �ٲ𶧸��� ��� ����
	{		
		FoeList.GetPath(CharainMap);
	}

	if (rAccumlationTime > FIXED) // 60FPS ����, 1 / 60.0f
	{			
		rAccumlationTime = 0;

		g_hDC = GetDC(g_hWnd);

		HDC bufferDC = CreateCompatibleDC(g_hDC);
		hbitmap = CreateCompatibleBitmap(g_hDC, Crect.right, Crect.bottom);
		oldbitmap = (HBITMAP)SelectObject(bufferDC, hbitmap);
		HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)); // ���� �귯��
		HBRUSH OldB = (HBRUSH)SelectObject(bufferDC, NewB);

		FillRect(bufferDC, &Crect, NewB);
		Player.MVSpeed = Player.CHARACTERSPEED * FIXED;
		FoeList.SpeedSet(FIXED);


		if (FoeList.WaitingEnem.size() == 0 && FoeList.EnemyList.size() == 0) // ���� ��� ���̸� �� ����
		{
			PlayGround.can_NextStage = true;
		}
		else if (FoeList.EnemyList.size() == 0 && FoeList.WaitingEnem.size() != 0 && PlayGround.changedAnime == false) // Ȱ������ ���� �� ������ ���� �� ����
		{
			FoeList.StacktoPush(0, PlayGround.getHeight(PlayGround.mapId) - 2);
			FoeList.StacktoPush(PlayGround.getWidth(PlayGround.mapId) - 1, PlayGround.getHeight(PlayGround.mapId) - 2);
		}

		if (PlayGround.matrix[PlayGround.mapId][(int)((Player.centerY - PlayGround.MAP_START_POINT_Y) / PlayGround.SIZE_OF_MAPHEIGHT)]
			[(int)((Player.centerX - PlayGround.MAP_START_POINT_X) / PlayGround.SIZE_OF_MAPWIDTH)] == PlayGround.DoorOpen
			&& (GetAsyncKeyState('W') & 0x8000) && PlayGround.changedAnime == false)
		{
			oldMap = PlayGround;
			PlayGround.changer(WindowScreen.rect);
			PlayGround.changedAnime = true;
			oldMap.changedAnime = true;
			Player.SetSpawn(PlayGround.getBlockCenterX(PlayGround.getWidth(PlayGround.mapId) / 2),
				PlayGround.getBlockCenterY(PlayGround.getHeight(PlayGround.mapId) / 2)); // �÷��̾� �߽� ����
			Player.draw(bufferDC);
			FoeList.NodeChanger(); // ���� ������ �� �����
			FoeList.FillEnem(Stagenum, StageMobAry, MAX_MOBARY, 5); // �ش� ���������� �� ä���
			if (Stages - 1 > Stagenum)
			{
				Stagenum++;  // �������� ����
			}
		}
		else
		{
			PlayGround.openNextStage(); //������������ ���� ���� Ȯ��
			PlayGround.drawBorder(bufferDC); // �׵θ� �׸���
			PlayGround.drawObject(bufferDC); // ���� �׸���
		}

		if (PlayGround.changedAnime)
		{
			PlayGround.changeAnimetion(bufferDC, WindowScreen.rect, FIXED);
			oldMap.changeAnimetion(bufferDC, WindowScreen.rect, FIXED);
			Player.SetSpawn(PlayGround.getBlockCenterX(PlayGround.getWidth(PlayGround.mapId) / 2),
				PlayGround.getBlockCenterY(PlayGround.getHeight(PlayGround.mapId) / 2)); // �÷��̾� �߽� ����
			Player.vy = 0; // Ȯ �������°� ����
		}

		// �浹ó���� ���� �� ��ǥ ����
		Player.PastSaves();
		FoeList.PastSaves();

		Player.KeyMVment(bufferDC); // �÷��̾� ������
		PlayGround.Collision(&Player);

		FoeList.UpdatePath(bufferDC, CharainMap); // ���� �� ã��

		Player.Grav(bufferDC, FIXED); // �߷�
		FoeList.Grav(bufferDC, FIXED); // �߷�

		// ����ü
		Player.UpdateProj(bufferDC, FIXED);

		// ��ֹ� �浹 ó��
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);

		// �� �浹
		FoeList.Collision_E();

		FoeList.KillEnemy(&eventList); // ü���� ������ �Ҹ�

		for (int i = 0; i < FoeList.EnemyList.size(); i++)
		{
			FoeList.EnemyList[i].foe.delay -= FIXED * 1;
			Character enemy = FoeList.EnemyList[i].foe;
			float distToPlayer = enemy.getDistance(Player);
			float weaponDist = enemy.weapon.getRange();
			if (weaponDist > distToPlayer) FoeList.EnemyList[i].foe.newAttackEvent(Player.centerX, Player.centerY, &eventList);
		}

		//for (int i = 0; i < FoeList.EnemyList.size(); i++)
		//{
		//
		//}


		if (Player.delay > 0) Player.delay -= FIXED * 1;

		for (int i = 0; i < eventList.size(); i++)
		{
			if (eventList[i].leftTime > 0)
			{
				eventList[i].leftTime -= FIXED * 1;
			}
			else
			{
				if (eventList[i].progTime > 0)
				{
					eventList[i].progTime -= FIXED * 1;
					if (eventList[i].eType == ATTACK) attackStart(eventList[i], bufferDC);
					if (eventList[i].eType == DAMAGED) damagedStart(&eventList[i]);
				}
				else
				{
					//if (eventList[it].eType == ATTACK)
					if (eventList[i].eType == DAMAGED) damagedEnd(&eventList[i]);
					eventList.erase(eventList.begin() + i--);
				}
			}
		}

		drawBackground(bufferDC, border, WindowScreen.rect);

		DrawPlayerHP(bufferDC, Player.MaxHeart, Player.CurHeart, Player.Shield);
		//FoeList.ShowNode(bufferDC);

		// �÷��̾� ����
		Player.draw(bufferDC);

		// �� ����
		FoeList.Draw_E(bufferDC);


		/*
		// test
		char buffer[256];
		wsprintf(buffer, "x��ǥ: %d y��ǥ: %d", CharainMap.x, CharainMap.y);
		TextOut(bufferDC, 100, 100, buffer, lstrlen(buffer));
		*/

		BitBlt(g_hDC, 0, 0, Crect.right, Crect.bottom, bufferDC, 0, 0, SRCCOPY);
		SelectObject(bufferDC, OldB);
		DeleteObject(NewB);
		DeleteObject(SelectObject(bufferDC, oldbitmap)); // ���� ������� �� �� ����
		DeleteDC(bufferDC); // hMemDC ����

		ReleaseDC(g_hWnd, g_hDC);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = TEXT("Api Sample"); // Ÿ��Ʋ �̸�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : ������ ���α׷��̶�� �ǹ�
	LPSTR lpszCmdLine, int nCmdShow) //hInstance : �ü���� Ŀ���� ���� ���α׷��� �ο��� ID
{ //szCmdLine : Ŀ��Ʈ���� �󿡼� ���α׷� ���� �� ���޵� ���ڿ�
	HWND hwnd; //iCmdShow : �����찡 ȭ�鿡 ��µ� ����
	MSG msg;

	WNDCLASS WndClass; //WndClass ��� ����ü ���� 
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //��½�Ÿ�� : ����/������ ��ȭ�� �ٽ� �׸�
	WndClass.lpfnWndProc = WndProc; //���ν��� �Լ���
	WndClass.cbClsExtra = 0; //O/S ��� ���� �޸� (Class)
	WndClass.cbWndExtra = 0; //O/s ��� ���� �޸� (Window)
	WndClass.hInstance = hInstance; //���� ���α׷� ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//����   
	WndClass.lpszMenuName = NULL; //�޴� �̸�
	WndClass.lpszClassName = lpszClass; //Ŭ���� �̸�
	RegisterClass(&WndClass); //�ռ� ������ ������ Ŭ������ �ּ�

	hwnd = CreateWindow(lpszClass, //�����찡 �����Ǹ� �ڵ�(hwnd)�� ��ȯ
		lpszClass, //������ Ŭ����, Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW, //������ ��Ÿ��
		CW_USEDEFAULT, //������ ��ġ, x��ǥ
		CW_USEDEFAULT, //������ ��ġ, y��ǥ
		WindowScreen.width, //������ ��   
		WindowScreen.height, //������ ����   
		NULL, //�θ� ������ �ڵ� 
		NULL, //�޴� �ڵ�
		hInstance,     //���� ���α׷� ID
		NULL      //������ ������ ����
	);
	ShowWindow(hwnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hwnd); //O/S �� WM_PAINT �޽��� ����

	dwOldGameTime = dwCurrentGameTime = timeGetTime();

	g_hWnd = hwnd;

 	while (g_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Run();
		}
	}
	return (int)msg.wParam;
}

HDC hdc;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	EventStruct eventStruct;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &Crect);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		Player.newAttackEvent(LOWORD(lParam), HIWORD(lParam), &eventList);
		break;
		
	case WM_RBUTTONDOWN:
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		if (Player.Projnum > 0)
		{
			Player.Thowable.push_back(Projectile(Player.centerX, Player.centerY, mX, mY, Arrowhead));
			Player.Projnum--;
		}
		break;

	case WM_DESTROY:
		g_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
	}
	return 0;
}