#include <windows.h>
#include <vector>
#include <cmath>
#include <list>
#include "Chara.h"
#include "Resol.h"
#include "Proj.h"
#include "Map.h"
#include "Weapon.h"
#include "WindowScreen.h"
using namespace std;

#define FIXED_FRAME 0.016

typedef struct _EventStruct
{
	float px, py;    //�÷��̾� ��ġ
	float mx, my;    //���콺 ��ġ
	float leftTime; //���۱��� �����ð�
	float progTime;    //���� �ð�
	Weapon weapon;    //���� ��ü
}EventStruct;

vector<EventStruct> eventList;    //�̺�Ʈ ó�� ����Ʈ

Character Player(CharaW, CharaH); // ĳ�� ����

// ���� ����
bool g_bLoop = true;

// ���� ���۸� ��Ʈ�� ����
HBITMAP hbitmap, oldbitmap;

// �ð� ����
LARGE_INTEGER g_tsecond;
LARGE_INTEGER g_tTime;
float g_fDeltatime;

// ���� �ڵ�
HDC g_hDC;
HWND g_hWnd;

// ���� ���۸� ����
float fTime = 0;

// �� ���� ����
Screen WindowScreen; //������ ȭ��  �Źμ� �߰�
Map PlayGround(WindowScreen.rect); // �� ����
Map oldMap(WindowScreen.rect);
RECT border = PlayGround.MaxSize;

// ���콺 ��ǥ
float mX, mY;

void startAttack(EventStruct target, HDC hdc)
{
	PAINTSTRUCT ps;
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

void Run()
{
	
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	//g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	fTime += (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	if (fTime < FIXED_FRAME) return;
	fTime = 0;
	g_hDC = GetDC(g_hWnd);

	HDC bufferDC = CreateCompatibleDC(g_hDC);
	hbitmap = CreateCompatibleBitmap(g_hDC, Crect.right, Crect.bottom);
	oldbitmap = (HBITMAP)SelectObject(bufferDC, hbitmap);
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255,255,255)); // ���� �귯��
	HBRUSH OldB = (HBRUSH)SelectObject(bufferDC, NewB);

	FillRect(bufferDC, &Crect, NewB);
	Player.MVSpeed = CHARACTERSPEED * FIXED_FRAME;

	if (PlayGround.matrix[PlayGround.mapId][(int)((Player.centerY - PlayGround.MAP_START_POINT_Y) / PlayGround.SIZE_OF_MAPHEIGHT)]
		[(int)((Player.centerX - PlayGround.MAP_START_POINT_X) / PlayGround.SIZE_OF_MAPWIDTH)] == PlayGround.DoorOpen
		&& (GetAsyncKeyState('W') & 0x8000) && PlayGround.changedAnime == false)
	{
		oldMap = PlayGround;
		PlayGround.changer(WindowScreen.rect);
		PlayGround.changedAnime = true;
		oldMap.changedAnime = true;
		Player.NextStagePosition(PlayGround.MAP_START_POINT_X + CharaW, Player.centerY);
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
		PlayGround.changeAnimetion(bufferDC, WindowScreen.rect, FIXED_FRAME);
		oldMap.changeAnimetion(bufferDC, WindowScreen.rect, FIXED_FRAME);
		Player.NextStagePosition(Player.centerX - PlayGround.buff, Player.centerY);
	}

	float size = 4;
	float term = FIXED_FRAME / size;
	float starter = term;
	while (size > 0)
	{
		Player.MVLeft(bufferDC); // ����
		Player.MVRight(bufferDC); // ������	
		Player.MVJump(bufferDC); // ����
		Player.Grav(bufferDC, starter); // �߷�

		// ����ü
		Player.UpdateProj(bufferDC, starter);

		if (PlayGround.MAP_START_POINT_X > Player.getLeft()) // ���� �� ����
		{
			Player.centerX = PlayGround.MAP_START_POINT_X + CharaW / 2;
		}
		if (PlayGround.MAP_START_POINT_Y > Player.getTop()) // õ�� ����
		{
			Player.centerY = PlayGround.MAP_START_POINT_Y + CharaH / 2;
			Player.vy = 0;
		}
		if (PlayGround.borderX < Player.getRight()) // ������ �� ����
		{
			Player.centerX = PlayGround.borderX - CharaW / 2;
		}
		if (PlayGround.borderY < Player.getBottom()) // �ٴ� ����
		{
			Player.centerY = PlayGround.borderY - CharaH / 2;
			Player.vy = 0;
			Player.jumpNum = 2;
		}

		// ��ֹ� �浹 ó��
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);
		starter = starter + term; size--;
	}
	QueryPerformanceCounter(&g_tTime);
	/*
	Player.MVLeft(bufferDC); // ����
		Player.MVRight(bufferDC); // ������	
		Player.MVJump(bufferDC); // ����
		Player.Grav(bufferDC, g_fDeltatime); // �߷�

		// ����ü
		Player.UpdateProj(bufferDC, g_fDeltatime);

		if (MAP_START_POINT_X > Player.getLeft()) // ���� �� ����
		{
			Player.centerX = MAP_START_POINT_X + CharaW / 2;
		}
		if (MAP_START_POINT_Y > Player.getTop()) // õ�� ����
		{
			Player.centerY = MAP_START_POINT_Y + CharaH / 2;
			Player.vy = 0;
		}
		if (PlayGround.borderX < Player.getRight()) // ������ �� ����
		{
			Player.centerX = PlayGround.borderX - CharaW / 2;
		}
		if (PlayGround.borderY < Player.getBottom()) // �ٴ� ����
		{
			Player.centerY = PlayGround.borderY - CharaH / 2;
			Player.vy = 0;
			Player.jumpNum = 2;
		}

		// ��ֹ� �浹 ó��
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);
	*/
	if (Player.delay > 0)
	{
		Player.delay -= FIXED_FRAME * 1;
	}
	for (int it=0; it<eventList.size(); it++)
	{
		eventList[it].weapon.setWeaponPos(Player.centerX, Player.centerY);
		if (eventList[it].leftTime > 0)
		{
			eventList[it].leftTime -= FIXED_FRAME * 1;
		}
		else
		{
			if (eventList[it].progTime > 0)
			{
				eventList[it].progTime -= FIXED_FRAME * 1;
				startAttack(eventList[it], bufferDC);
			}
			else
			{
				eventList.erase(eventList.begin()+it);
				//eventEnd
			}
		}
	}

	// �÷��̾� ����
	Player.draw(bufferDC);

	drawBackground(bufferDC, border, WindowScreen.rect);

	BitBlt(g_hDC, 0, 0, Crect.right, Crect.bottom, bufferDC, 0, 0, SRCCOPY);
	SelectObject(bufferDC, OldB);
	DeleteObject(NewB);
	DeleteObject(SelectObject(bufferDC, oldbitmap)); // ���� ������� �� �� ����
	DeleteDC(bufferDC); // hMemDC ����

	ReleaseDC(g_hWnd, g_hDC);
}

/*
void Run()
{

	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	g_tTime = tTime;
	g_hDC = GetDC(g_hWnd);

	HDC bufferDC = CreateCompatibleDC(g_hDC);
	hbitmap = CreateCompatibleBitmap(g_hDC, Crect.right, Crect.bottom);
	oldbitmap = (HBITMAP)SelectObject(bufferDC, hbitmap);
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255,255,255)); // ���� �귯��
	HBRUSH OldB = (HBRUSH)SelectObject(bufferDC, NewB);

	FillRect(bufferDC, &Crect, NewB);
	PlayGround.drawBorder(bufferDC); // �׵θ� �׸���
	PlayGround.drawObject(bufferDC); // ���� �׸���

	Player.MVSpeed = CHARACTERSPEED * g_fDeltatime;

	float size = 4;
	float term = g_fDeltatime / size;
	float starter = term;
	while (size > 0)
	{
		Player.MVLeft(bufferDC); // ����
		Player.MVRight(bufferDC); // ������
		Player.MVJump(bufferDC); // ����
		Player.Grav(bufferDC, starter); // �߷�

		// ����ü
		Player.UpdateProj(bufferDC, starter);

		if (MAP_START_POINT_X > Player.getLeft()) // ���� �� ����
		{
			Player.centerX = MAP_START_POINT_X + CharaW / 2;
		}
		if (MAP_START_POINT_Y > Player.getTop()) // õ�� ����
		{
			Player.centerY = MAP_START_POINT_Y + CharaH / 2;
			Player.vy = 0;
		}
		if (PlayGround.borderX < Player.getRight()) // ������ �� ����
		{
			Player.centerX = PlayGround.borderX - CharaW / 2;
		}
		if (PlayGround.borderY < Player.getBottom()) // �ٴ� ����
		{
			Player.centerY = PlayGround.borderY - CharaH / 2;
			Player.vy = 0;
			Player.jumpNum = 2;
		}

		// ��ֹ� �浹 ó��
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);
		starter = starter + term; size--;
	}

	// �÷��̾� ����
Player.draw(bufferDC);

BitBlt(g_hDC, 0, 0, Crect.right, Crect.bottom, bufferDC, 0, 0, SRCCOPY);
SelectObject(bufferDC, OldB);
DeleteObject(NewB);
DeleteObject(SelectObject(bufferDC, oldbitmap)); // ���� ������� �� �� ����
DeleteDC(bufferDC); // hMemDC ����

ReleaseDC(g_hWnd, g_hDC);
}
*/

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

	g_hWnd = hwnd;

	QueryPerformanceFrequency(&g_tsecond);
	QueryPerformanceCounter(&g_tTime);

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