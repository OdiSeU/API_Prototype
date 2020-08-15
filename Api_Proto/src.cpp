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
	float px, py;    //플레이어 위치
	float mx, my;    //마우스 위치
	float leftTime; //시작까지 남은시간
	float progTime;    //진행 시간
	Weapon weapon;    //무기 객체
}EventStruct;

vector<EventStruct> eventList;    //이벤트 처리 리스트

Character Player(500, 500); // 캐릭 선언

Character Enemy(200, 200); // 적 선언

// 루프 관련
bool g_bLoop = true;

// 더블 버퍼링 비트맵 선언
HBITMAP hbitmap, oldbitmap;

// 시간 측정
DWORD dwOldGameTime = 0, dwCurrentGameTime = 0;
FLOAT rDeltaTime = 0.0f, rAccumlationTime = 0.0f;

// 전역 핸들
HDC g_hDC;
HWND g_hWnd;

// 맵 전연 변수
Screen WindowScreen; //윈도우 화면  신민수 추가
Map PlayGround(WindowScreen.rect); // 맵 선언
Map oldMap(WindowScreen.rect);
RECT border = PlayGround.MaxSize;

// 마우스 좌표
float mX, mY;

// AI 길 뚫기
Pathfinder Way(PlayGround, 3);

#define FIXED 0.016f

/*
DWORD dwOldGameTime = 0, dwCurrentGameTime = 0;
DWORD dwOldGameTime = dwCurrentGameTime = ::timeGetTime();
FLOAT rDeltaTime = 0.0f, rAccumlationTime = 0.0f;

// DeltaTime은 이전 프레임과 현재 프레임의 시간 간격을 의미하고
// AccumlationTime은 DeltaTime이 누적된 것을 의미합니다.

bool bGameLoop = true;
while (bGameLoop)
{
	dwCurrentGameTime = ::timeGetTime();
	rDeltaTime       = static_cast<float>(dwCurrentGameTime - dwOldGameTime) / 1000;
	dwOldGameTime    = dwCurrentGameTime;
	rAccumlationTime += rDeltaTime;

	if (rAccumlationTime > 0.016f) // 60FPS 기준, 1 / 60.0f
	{
		rAccumlationTime = 0.0f;

		// 실제로 게임 루틴을 처리하는 부분
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
			// 적과 공격범위 충돌체크 추가
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
			// 적과 공격범위 충돌체크 추가
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

	// 적과 플레이어의 좌표
	OldCharainMap.x = CharainMap.x; 
	OldCharainMap.y = CharainMap.y;
	CharainMap.x = PlayGround.xToCol(Player.centerX);
	CharainMap.y = PlayGround.yToRow(Player.centerY);
	EnemyinMap.x = PlayGround.xToCol(Enemy.centerX);
	EnemyinMap.y = PlayGround.yToRow(Enemy.centerY);

	if (CharainMap.x != OldCharainMap.x || CharainMap.y != OldCharainMap.y) // 캐릭터 좌표 바뀔때마다 경로 변경
	{		
		if (Way.getNodeIndex(CharainMap) != -1)
		{
			Result.clear();
			Way.AstarAlgorithm(CharainMap, EnemyinMap, &Result);
		}
	}

	if (rAccumlationTime > FIXED) // 60FPS 기준, 1 / 60.0f
	{
		rAccumlationTime = 0;
		
		g_hDC = GetDC(g_hWnd);

		HDC bufferDC = CreateCompatibleDC(g_hDC);
		hbitmap = CreateCompatibleBitmap(g_hDC, Crect.right, Crect.bottom);
		oldbitmap = (HBITMAP)SelectObject(bufferDC, hbitmap);
		HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)); // 배경색 브러쉬
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
			PlayGround.openNextStage(); //다음스테이지 조건 충족 확인
			PlayGround.drawBorder(bufferDC); // 테두리 그리기
			PlayGround.drawObject(bufferDC); // 물건 그리기
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
			Player.MVLeft(bufferDC); // 왼쪽
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			Player.MVRight(bufferDC); // 오른쪽	
		}
		if ((GetAsyncKeyState(VK_SPACE) & 0x0001) && Player.jumpNum >= 1)
		{
			Player.MVJump(bufferDC); // 점프
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

		Player.Grav(bufferDC, FIXED); // 중력
		Enemy.Grav(bufferDC, FIXED); // 중력

		// 투사체
		Player.UpdateProj(bufferDC, FIXED);

		// 장애물 충돌 처리
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);

		// 적 충돌
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

		// 플레이어 갱신
		Player.draw(bufferDC);

		// 적 갱신
		Enemy.draw(bufferDC);

		drawBackground(bufferDC, border, WindowScreen.rect);

		// test
		char buffer[256];
		wsprintf(buffer, "x좌표: %d y좌표: %d", CharainMap.x, CharainMap.y);
		TextOut(bufferDC, 100, 100, buffer, lstrlen(buffer));

		BitBlt(g_hDC, 0, 0, Crect.right, Crect.bottom, bufferDC, 0, 0, SRCCOPY);
		SelectObject(bufferDC, OldB);
		DeleteObject(NewB);
		DeleteObject(SelectObject(bufferDC, oldbitmap)); // 종이 원래대로 한 후 제거
		DeleteDC(bufferDC); // hMemDC 제거

		ReleaseDC(g_hWnd, g_hDC);
	}
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
		WindowScreen.width, //윈도우 폭   
		WindowScreen.height, //윈도우 높이   
		NULL, //부모 윈도우 핸들 
		NULL, //메뉴 핸들
		hInstance,     //응용 프로그램 ID
		NULL      //생성된 윈도우 정보
	);
	ShowWindow(hwnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hwnd); //O/S 에 WM_PAINT 메시지 전송

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
		return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
	}
	return 0;
}