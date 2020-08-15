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
using namespace std;

#pragma comment(lib, "winmm.lib")

typedef struct _EventStruct
{
	float px, py;    //�÷��̾� ��ġ
	float mx, my;    //���콺 ��ġ
	float leftTime; //���۱��� �����ð�
	float progTime;    //���� �ð�
	Weapon weapon;    //���� ��ü
}EventStruct;

vector<EventStruct> eventList;    //�̺�Ʈ ó�� ����Ʈ

Character Player(500, 500); // ĳ�� ����

Character Enemy(200, 200); // �� ����

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

// ���콺 ��ǥ
float mX, mY;

// AI �� �ձ�
Pathfinder Way(PlayGround, 3);

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

void startAttack(EventStruct target, HDC hdc)
{
	//PAINTSTRUCT ps;
	Motion motion = target.weapon.getMotion();
	switch (target.weapon.getWeaponType())
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
			// ���� ���ݹ��� �浹üũ �߰�
		}
		if (motion.shape == 's')
		{
			Pie(hdc,
				motion.centerX - motion.Radius,
				motion.centerY - motion.Radius,
				motion.centerX + motion.Radius,
				motion.centerY + motion.Radius,
				motion.centerX + motion.Radius * cos(motion.startAngle),
				motion.centerY - motion.Radius * sin(motion.startAngle),
				motion.centerX + motion.Radius * cos(motion.endAngle),
				motion.centerY - motion.Radius * sin(motion.endAngle)
			);
			// ���� ���ݹ��� �浹üũ �߰�
		}
		break;
	}
}

POINT EnemyinMap;
POINT CharainMap;
POINT OldCharainMap;
vector<BrickInfo> Result;

void Run()
{	
	dwCurrentGameTime = timeGetTime();
	rDeltaTime = (float)(dwCurrentGameTime - dwOldGameTime) / 1000;
	dwOldGameTime = dwCurrentGameTime;
	rAccumlationTime = rAccumlationTime + rDeltaTime;

	PlayGround.Collision(&Player);
	PlayGround.Collision(&Enemy);

	// ���� �÷��̾��� ��ǥ
	OldCharainMap.x = CharainMap.x; 
	OldCharainMap.y = CharainMap.y;
	CharainMap.x = PlayGround.xToCol(Player.centerX);
	CharainMap.y = PlayGround.yToRow(Player.centerY);
	EnemyinMap.x = PlayGround.xToCol(Enemy.centerX);
	EnemyinMap.y = PlayGround.yToRow(Enemy.centerY);

	if (CharainMap.x != OldCharainMap.x || CharainMap.y != OldCharainMap.y) // ĳ���� ��ǥ �ٲ𶧸��� ��� ����
	{		
		if (Way.getNodeIndex(CharainMap) != -1)
		{
			Result.clear();
			Way.AstarAlgorithm(CharainMap, EnemyinMap, &Result);
		}
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
		Enemy.MVSpeed = Enemy.CHARACTERSPEED * FIXED;

		if (PlayGround.matrix[PlayGround.mapId][(int)((Player.centerY - PlayGround.MAP_START_POINT_Y) / PlayGround.SIZE_OF_MAPHEIGHT)]
			[(int)((Player.centerX - PlayGround.MAP_START_POINT_X) / PlayGround.SIZE_OF_MAPWIDTH)] == PlayGround.DoorOpen
			&& (GetAsyncKeyState('W') & 0x8000) && PlayGround.changedAnime == false)
		{
			oldMap = PlayGround;
			PlayGround.changer(WindowScreen.rect);
			PlayGround.changedAnime = true;
			oldMap.changedAnime = true;
			Player.NextStagePosition(PlayGround.MAP_START_POINT_X + Player.CharaW, Player.centerY);
			Player.draw(bufferDC);
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
			Player.NextStagePosition(Player.centerX - PlayGround.buff, Player.centerY);
		}

		Player.bfLeft = Player.getLeft();
		Player.bfTop = Player.getTop();
		Player.bfBottom = Player.getBottom();
		Player.bfRight = Player.getRight();

		Enemy.bfLeft = Enemy.getLeft();
		Enemy.bfTop = Enemy.getTop();
		Enemy.bfBottom = Enemy.getBottom();
		Enemy.bfRight = Enemy.getRight();

		if (GetAsyncKeyState('A') & 0x8000)
		{
			Player.MVLeft(bufferDC); // ����
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			Player.MVRight(bufferDC); // ������	
		}
		if ((GetAsyncKeyState(VK_SPACE) & 0x0001) && Player.jumpNum >= 1)
		{
			Player.MVJump(bufferDC); // ����
		}

		if (Result.size() >= 1)
		{
			POINT CurBrick = Way.getGnode(Result.back().getCur())->getCord();
			if (EnemyinMap.x == CurBrick.x && EnemyinMap.y == CurBrick.y)
			{
				POINT NextBrick = Way.getGnode(Result[Result.size() - 1].getParent())->getCord();
				if (EnemyinMap.x < NextBrick.x)
				{
					Enemy.MVRight(bufferDC);
				}
				else if (EnemyinMap.x > NextBrick.x)
				{
					Enemy.MVLeft(bufferDC);
				}
			}
		}

		Player.Grav(bufferDC, FIXED); // �߷�
		Enemy.Grav(bufferDC, FIXED); // �߷�

		// ����ü
		Player.UpdateProj(bufferDC, FIXED);

		// ��ֹ� �浹 ó��
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);

		// �� �浹
		PlayGround.Collision(&Enemy);

		if (Player.delay > 0)
		{
			Player.delay -= FIXED * 1;
		}
		for (int it = 0; it < eventList.size(); it++)
		{
			eventList[it].weapon.setWeaponPos(Player.centerX, Player.centerY);
			if (eventList[it].leftTime > 0)
			{
				eventList[it].leftTime -= FIXED * 1;
			}
			else
			{
				if (eventList[it].progTime > 0)
				{
					eventList[it].progTime -= FIXED * 1;
					startAttack(eventList[it], bufferDC);
				}
				else
				{
					eventList.erase(eventList.begin() + it);
					//eventEnd
				}
			}
		}

		// �÷��̾� ����
		Player.draw(bufferDC);

		// �� ����
		Enemy.draw(bufferDC);

		drawBackground(bufferDC, border, WindowScreen.rect);

		// test
		char buffer[256];
		wsprintf(buffer, "x��ǥ: %d y��ǥ: %d", CharainMap.x, CharainMap.y);
		TextOut(bufferDC, 100, 100, buffer, lstrlen(buffer));

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
		Player.weapon.setWeaponPos(Player.centerX, Player.centerY);
		if (Player.delay <= 0) {
			Player.delay = Player.weapon.getDealy() + Player.weapon.getAtkSpeed();
			eventStruct.mx = LOWORD(lParam);
			eventStruct.my = HIWORD(lParam);
			eventStruct.px = Player.centerX;
			eventStruct.py = Player.centerY;
			eventStruct.weapon = Player.weapon;
			eventStruct.leftTime = Player.weapon.getDealy();
			eventStruct.progTime = Player.weapon.getAtkSpeed();
			eventList.push_back(eventStruct);

			Player.weapon.addCombo();
		}
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