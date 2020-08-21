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

Character Bruser(1, 0, 0, 150, 10, 1, 4, RGB(100, 100, 100)); // 브루저 몹
Character Ninja(1, 0, 0, 300, 10, 1, 4, RGB(0, 200, 250)); // 닌자 몹
Character Nerd(1, 0, 0, 120, 10, 1, 4, RGB(0, 100, 250)); // 찐따 몹
Character Ender(1, -1, -1, -1, -1, -1, -1, RGB(0, 0, 0)); // 마침표

int Stagenum = 0;

Character StageMobAry[Stages][MAX_MOBARY] = // 스테이지에 담기는 몹들
{
	{Nerd,Nerd,Nerd,Nerd,Bruser,Ninja,Ender},
	{Nerd,Bruser,Nerd,Nerd,Bruser,Nerd,Bruser,Ender},
	{Nerd,Bruser,Ninja,Ninja,Bruser,Ninja,Bruser,Ender},
	{Ninja,Ninja,Ninja,Ninja,Bruser,Bruser,Bruser,Ender},
	{Bruser,Ninja,Bruser,Ninja,Bruser,Ninja,Bruser,Ninja,Ender}
};

vector<EventStruct> eventList;    //이벤트 처리 리스트

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

// 처음 생성 좌표(2개), 좌우 속도, 점프파워, 점프 가능 횟수, 생명력 순
Character Player(0, PlayGround.getBlockCenterX(PlayGround.getWidth(0) / 2), PlayGround.getBlockCenterY(PlayGround.getHeight(0)/ 2), 250, 10, 2, 10, RGB(200, 0, 200));

// 마우스 좌표
float mX, mY;

// 적에 관환 데이터가 담길 리스트
Enemy FoeList(&PlayGround, 2);

// 맵에서의 플레이어 좌표
POINT CharainMap;
POINT OldCharainMap;

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
		// 적과 공격범위 충돌체크 추가
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
	// 플레이어의 좌표
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
	if ((CharainMap.x != OldCharainMap.x || CharainMap.y != OldCharainMap.y)) // 캐릭터 좌표 바뀔때마다 경로 변경
	{		
		FoeList.GetPath(CharainMap);
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
		FoeList.SpeedSet(FIXED);


		if (FoeList.WaitingEnem.size() == 0 && FoeList.EnemyList.size() == 0) // 적을 모두 죽이면 문 열기
		{
			PlayGround.can_NextStage = true;
		}
		else if (FoeList.EnemyList.size() == 0 && FoeList.WaitingEnem.size() != 0 && PlayGround.changedAnime == false) // 활동중인 적이 다 죽으면 다음 적 등장
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
				PlayGround.getBlockCenterY(PlayGround.getHeight(PlayGround.mapId) / 2)); // 플레이어 중심 생성
			Player.draw(bufferDC);
			FoeList.NodeChanger(); // 적이 지나갈 길 재생성
			FoeList.FillEnem(Stagenum, StageMobAry, MAX_MOBARY, 5); // 해당 스테이지의 적 채우기
			if (Stages - 1 > Stagenum)
			{
				Stagenum++;  // 스테이지 증가
			}
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
			Player.SetSpawn(PlayGround.getBlockCenterX(PlayGround.getWidth(PlayGround.mapId) / 2),
				PlayGround.getBlockCenterY(PlayGround.getHeight(PlayGround.mapId) / 2)); // 플레이어 중심 생성
			Player.vy = 0; // 확 떨어지는거 방지
		}

		// 충돌처리를 위한 전 좌표 따기
		Player.PastSaves();
		FoeList.PastSaves();

		Player.KeyMVment(bufferDC); // 플레이어 움직임
		PlayGround.Collision(&Player);

		FoeList.UpdatePath(bufferDC, CharainMap); // 적들 길 찾기

		Player.Grav(bufferDC, FIXED); // 중력
		FoeList.Grav(bufferDC, FIXED); // 중력

		// 투사체
		Player.UpdateProj(bufferDC, FIXED);

		// 장애물 충돌 처리
		PlayGround.ProjColl(bufferDC, &Player);
		PlayGround.Collision(&Player);

		// 적 충돌
		FoeList.Collision_E();

		FoeList.KillEnemy(&eventList); // 체력이 다한적 소멸

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

		// 플레이어 갱신
		Player.draw(bufferDC);

		// 적 갱신
		FoeList.Draw_E(bufferDC);


		/*
		// test
		char buffer[256];
		wsprintf(buffer, "x좌표: %d y좌표: %d", CharainMap.x, CharainMap.y);
		TextOut(bufferDC, 100, 100, buffer, lstrlen(buffer));
		*/

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
		return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
	}
	return 0;
}