#include <windows.h>
#include <cmath>
#include "Chara.h"
#include "Resol.h"
#include "Proj.h"
#include "Map.h"

Character Player(CharaW, CharaH); // ĳ�� ����
Map PlayGround;
//�� id
int Mapnum = 0;

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
	//���⿡ ����� ��ġ��
	int borderX = PlayGround.getWidth(Mapnum) * SIZE_OF_MAPWIDTH + MAP_START_POINT_X;
	int borderY = PlayGround.getHeight(Mapnum) * SIZE_OF_MAPHEIGHT + MAP_START_POINT_Y;

	Player.MVSpeed = CHARACTERSPEED * g_fDeltatime;

	Player.MVLeft(g_hDC);
	Player.MVRight(g_hDC);
	Player.update(g_hDC);

	if (MAP_START_POINT_X > Player.getLeft())
	{
		Player.centerX = MAP_START_POINT_X + CharaW / 2;
	}
	if (MAP_START_POINT_Y > Player.getTop())
	{
		Player.centerY = MAP_START_POINT_Y + CharaH / 2;	
	}
	if (borderX < Player.getRight())
	{
		Player.centerX = borderX - CharaW / 2;
	}
	if (borderY < Player.getBottom())
	{
		Player.centerY = borderY - CharaH / 2;
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

		for (int i = 0; i < PlayGround.getHeight(Mapnum); i++)
		{
			for (int j = 0; j < PlayGround.getWidth(Mapnum); j++)
			{
				Rectangle(hdc, MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT,
					MAP_START_POINT_X + j * SIZE_OF_MAPWIDTH + SIZE_OF_MAPWIDTH, MAP_START_POINT_Y + i * SIZE_OF_MAPHEIGHT + SIZE_OF_MAPHEIGHT);
			}
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		
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