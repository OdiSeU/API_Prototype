// ������ �Ǿ���? �ȵǾ���?


#include <windows.h>
#include <iostream>
#include <cmath>
using namespace std;

#define mapwidth 33
#define mapheight 16
#define Rwidth 50
#define Rheight 50

RECT cwin;
int startx = 100, starty = 100;
bool g_bLoop = true;
LARGE_INTEGER tTime;
LARGE_INTEGER g_tsecond;
LARGE_INTEGER g_tTime;
float g_fDeltatime;

typedef struct _Cord
{
	int x; int y;
} Cordinate;

void Run()
{
	QueryPerformanceCounter(&tTime);
	g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	g_tTime = tTime;
}

class Map
{
	enum { Deadlock = 1, Floor };
	int height;
	int width;
	unsigned char matrix[mapheight][mapwidth] =
	{
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,2,2,2,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
	};
public:
	Map(int h, int w)
	{
		height = h; width = w;
	}
};

class Throw
{
	Cordinate Tcord;
	short type;
	float vx, vy;
public:
	Throw(short type, Cordinate player, Cordinate mouse)
	{
		if (type == 1)
		{
			Tcord.x = player.x;
			Tcord.y = player.y;
			double distance = sqrt(pow(player.x - mouse.x, 2) + pow(player.y - mouse.y, 2));
			double height = mouse.y - player.y;
			vx = 100;
			vy = height / distance;
		}
		else
		{

		}
	}
	void Update()
	{
		/*
		LARGE_INTEGER tTime;
		QueryPerformanceCounter(&tTime);
		g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
		g_tTime = tTime;
		*/
		Tcord.x = Tcord.x + vx * g_fDeltatime;
		Tcord.y = Tcord.y + vy * g_fDeltatime;
	}
};

Map testMap(Rwidth, Rheight);

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
	ShowWindow(hwnd, SW_MAXIMIZE); //�������� ȭ�� ���
	UpdateWindow(hwnd); //O/S �� WM_PAINT �޽��� ����

	QueryPerformanceFrequency(&g_tsecond);
	QueryPerformanceFrequency(&g_tTime);

	/*
	while (GetMessage(&msg, NULL, 0, 0)) //WinProc()���� PostQuitMessage() ȣ�� ������ ó��
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); //WinMain -> WinProc
	}
	*/

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

		GetClientRect(hwnd, &cwin);
		//AdjustWindowRect(&cwin, )
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < mapheight; i++)
		{
			for (int j = 0; j < mapwidth; j++)
			{
				Rectangle(hdc, startx + j * Rwidth, starty + i * Rheight, startx + j * Rwidth + Rwidth, starty + i * Rheight + Rheight);
			}
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		g_bLoop = false;
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}