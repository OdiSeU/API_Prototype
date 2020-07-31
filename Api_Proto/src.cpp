#include <windows.h>
#include <cmath>
#include "Chara.h"
#include "Resol.h"
#include "Proj.h"
#include "Map.h"

Character Player(CharaW, CharaH); // ĳ�� ����
Map PlayGround; // �� ����

// ���� ����
bool g_bLoop = true;

// �ð� ����
LARGE_INTEGER g_tsecond;
LARGE_INTEGER g_tTime;
float g_fDeltatime;

// ���� �ڵ�
HDC g_hDC;
HWND g_hWnd;

void Run()
{
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	g_tTime = tTime;
	g_hDC = GetDC(g_hWnd);

	PlayGround.drawBorder(g_hDC);
	PlayGround.drawObject(g_hDC);

	Player.MVSpeed = CHARACTERSPEED * g_fDeltatime;
	Player.JumpPower = 400 * g_fDeltatime;

	Player.MVLeft(g_hDC);
	Player.MVRight(g_hDC);
	

	//Player.Jump(g_hDC, g_fDeltatime);
	// ����, �Լ��� ������ ����� �������� �ϴ� ����
	if ((GetAsyncKeyState(VK_SPACE) & 0x0001))
	{
		JumpedY = Player.getTop() - JUMPHEIGHT;
		Player.MVStat = JUMPUP;
		if (JumpedY <= 100)
		{
			JumpedY = 100 + CharaH / 2;
		}
	}
	if (JumpedY < Player.centerY)
	{
		Player.clear(g_hDC);
		Player.MVStat = JUMPUP;
		Player.centerY = Player.centerY - Player.JumpPower;
		Player.vy = 0;
	}
	if (JumpedY >= Player.centerY)
	{
		Player.MVStat = JUMPDOWN;
		JumpedY = 10000;
		Player.update(g_hDC, g_fDeltatime);
	}

	if (MAP_START_POINT_X > Player.getLeft())
	{
		Player.centerX = MAP_START_POINT_X + CharaW / 2;
	}
	if (MAP_START_POINT_Y > Player.getTop()) // õ�� ����
	{
		Player.centerY = MAP_START_POINT_Y + CharaH / 2;	
	}
	if (PlayGround.borderX < Player.getRight())
	{
		Player.centerX = PlayGround.borderX - CharaW / 2;
	}
	if (PlayGround.borderY < Player.getBottom()) // �ٴ� ����
	{
		Player.centerY = PlayGround.borderY - CharaH / 2;
		Player.vy = 0;
	}
	int MindexX = (Player.centerX - MAP_START_POINT_X) / SIZE_OF_MAPWIDTH;
	int MindexY = (Player.getBottom() - MAP_START_POINT_Y - 1.f) / SIZE_OF_MAPHEIGHT;
	if (Player.MVStat == JUMPDOWN && (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) <= Player.getBottom() )
	{
		if (PlayGround.matrix[PlayGround.mapId][MindexY + 1][(int)((Player.getLeft() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 2 || PlayGround.matrix[PlayGround.mapId][MindexY + 1][(int)((Player.getRight() - (float)MAP_START_POINT_X) / (float)SIZE_OF_MAPWIDTH)] == 2)
		{
			Player.centerY = (MAP_START_POINT_Y + (MindexY + 1) * SIZE_OF_MAPHEIGHT) - CharaH / 2;
			Player.vy = 0;
		}
	}
	
	Player.draw(g_hDC);
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
		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
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