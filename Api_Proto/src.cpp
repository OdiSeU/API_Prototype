#include <windows.h>
#include <cmath>
#include "Chara.h"
#include "Resol.h"
#include "Proj.h"
#include "Map.h"

Character Player(CharaW, CharaH); // 캐릭 선언
Map PlayGround;
//맵 id
int Mapnum = 0;

// 루프 관련
bool g_bLoop = true;

// 시간 측정
LARGE_INTEGER g_tsecond;
LARGE_INTEGER g_tTime;
float g_fDeltatime;

// 전역 핸들
HDC g_hDC;
HWND g_hWnd;

void Run()
{
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	g_fDeltatime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tsecond.QuadPart;
	g_tTime = tTime;
	g_hDC = GetDC(g_hWnd);
	//여기에 만든거 합치기
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
	ShowWindow(hwnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hwnd); //O/S 에 WM_PAINT 메시지 전송

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
		return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
	}
	return 0;
}