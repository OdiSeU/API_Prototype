// 수정이 되었나? 안되었나?


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

LPCTSTR lpszClass = TEXT("Api Sample"); // 타이틀 이름

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : 윈도우 프로그램이라는 의미
	LPSTR lpszCmdLine, int nCmdShow) //hInstance : 운영체제의 커널이 응용 프로그램에 부여한 ID
{ //szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
	HWND hwnd; //iCmdShow : 윈도우가 화면에 출력될 형태
	MSG msg;
	WNDCLASS WndClass; //WndClass 라는 구조체 정의 
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //출력스타일 : 수직/수평의 변화시 다시 그림
	WndClass.lpfnWndProc = WndProc; //프로시저 함수명
	WndClass.cbClsExtra = 0; //O/S 사용 여분 메모리 (Class)
	WndClass.cbWndExtra = 0; //O/s 사용 여분 메모리 (Window)
	WndClass.hInstance = hInstance; //응용 프로그램 ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 유형
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 유형
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//배경색   
	WndClass.lpszMenuName = NULL; //메뉴 이름
	WndClass.lpszClassName = lpszClass; //클래스 이름
	RegisterClass(&WndClass); //앞서 정의한 윈도우 클래스의 주소

	hwnd = CreateWindow(lpszClass, //윈도우가 생성되면 핸들(hwnd)이 반환
		lpszClass, //윈도우 클래스, 타이틀 이름
		WS_OVERLAPPEDWINDOW, //윈도우 스타일
		CW_USEDEFAULT, //윈도우 위치, x좌표
		CW_USEDEFAULT, //윈도우 위치, y좌표
		CW_USEDEFAULT, //윈도우 폭   
		CW_USEDEFAULT, //윈도우 높이   
		NULL, //부모 윈도우 핸들 
		NULL, //메뉴 핸들
		hInstance,     //응용 프로그램 ID
		NULL      //생성된 윈도우 정보
	);
	ShowWindow(hwnd, SW_MAXIMIZE); //윈도우의 화면 출력
	UpdateWindow(hwnd); //O/S 에 WM_PAINT 메시지 전송

	QueryPerformanceFrequency(&g_tsecond);
	QueryPerformanceFrequency(&g_tTime);

	/*
	while (GetMessage(&msg, NULL, 0, 0)) //WinProc()에서 PostQuitMessage() 호출 때까지 처리
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
	return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}