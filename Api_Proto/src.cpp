#include <windows.h>
#include <cmath>
#include <list>
#include "Chara.h"
#include "Resol.h"
#include "Proj.h"
#include "Map.h"

Character Player(CharaW, CharaH); // ĳ�� ����
Map PlayGround; // �� ����

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

/*
void Run()
{
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	g_tTime = tTime;
	g_hDC = GetDC(g_hWnd);

	PlayGround.drawBorder(g_hDC); // �׵θ� �׸���
	PlayGround.drawObject(g_hDC); // ���� �׸���

	Player.MVSpeed = CHARACTERSPEED * g_fDeltatime;

	Player.MVLeft(g_hDC); // ����
	Player.MVRight(g_hDC); // ������	
	Player.MVJump(g_hDC); // ����
	Player.Grav(g_hDC, g_fDeltatime); // �߷�

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

<<<<<<< Updated upstream
	// ��ֹ� �浹 ó��
	PlayGround.Collision(&Player);
	
	Player.draw(g_hDC);
	ReleaseDC(g_hWnd, g_hDC);
}
*/
=======
typedef struct EventStruct
{
	float px, py;	//�÷��̾� ��ġ
	float mx, my;	//���콺 ��ġ
	float leftTime; //���۱��� �����ð�
	float mtTime;	//���� �ð�
	Weapon weapon;	//���� ��ü
}EventStruct;

list<EventStruct> eventList;	//�̺�Ʈ ó�� ����Ʈ
>>>>>>> Stashed changes

// ���� ���۸� �߰�
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
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)); // ���� �귯��
	HBRUSH OldB = (HBRUSH)SelectObject(bufferDC, NewB);

	FillRect(bufferDC, &Crect, NewB);
	PlayGround.drawBorder(bufferDC); // �׵θ� �׸���
	PlayGround.drawObject(bufferDC); // ���� �׸���

	Player.MVSpeed = CHARACTERSPEED * g_fDeltatime;

	Player.MVLeft(bufferDC); // ����
	Player.MVRight(bufferDC); // ������	
	Player.MVJump(bufferDC); // ����
	Player.Grav(bufferDC, g_fDeltatime); // �߷�

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

	//�̺�Ʈ ����Ʈ ����
	for (list<EventStruct*>::iterator it = eventList.begin(); it != intList.end(); it++)
	{
		if (eventList[it].leftTime > 0)
		{
			eventList[it].leftTime -= g_fDeltatime * 1;
		}
		else
		{
			if (eventList[it].mtTime > 0)
			{
				eventList[it].mtTime -= g_fDeltatime * 1;
				//eventStart
			}
			else
			{
				eventList[it].erase(it);
				//eventEnd
			}
		}
	}

	// ��ֹ� �浹 ó��
	PlayGround.Collision(&Player);

	Player.draw(bufferDC);

	BitBlt(g_hDC, 0, 0, Crect.right, Crect.bottom, bufferDC, 0, 0, SRCCOPY);
	SelectObject(bufferDC, OldB);
	DeleteObject(NewB);
	DeleteObject(SelectObject(bufferDC, oldbitmap)); // ���� ������� �� �� ����
	DeleteDC(bufferDC); // hMemDC ����

	ReleaseDC(g_hWnd, g_hDC);
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
		CW_USEDEFAULT, //������ ��   
		CW_USEDEFAULT, //������ ����   
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
			Run();
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

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &Crect);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		g_bLoop = false;
		PostQuitMessage(0);
		break;

	// �� �߰� //
	case WM_LBUTTONDOWN:
<<<<<<< Updated upstream
		Player.attackStart(hwnd, LOWORD(lParam), HIWORD(lParam));
=======
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		Player.attackStart(hwnd, mX, mY);
		break;

	case WM_RBUTTONDOWN:
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		if (Player.Projnum > 0)
		{
			Player.Thowable.push_back(Projectile(Player.centerX, Player.centerY, mX, mY, Arrowhead));
			Player.Projnum--;
		}
>>>>>>> Stashed changes
		break;

	case WM_RBUTTONDOWN:
		Player.throwStart(hwnd, LOWORD(lParam), HIWORD(lParam));
		break;

	// �� �߰� //
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
	}
	return 0;
}
