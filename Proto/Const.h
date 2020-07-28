#pragma once

// 모니터 해상도
#define DEFUALT_WINDOWSIZE_WIDTH 1920
#define DEFUALT_WINDOWSIZE_HEIGHT 1080

// 맵 사각형 개수
#define COUNT_OF_MAPWIDTH 33
#define COUNT_OF_MAPHEIGHT 16

// 맵 사각형 크기
#define SIZE_OF_MAPWIDTH 50
#define SIZE_OF_MAPHEIGHT 50

// 맵 그리기 시작 좌표 (left, top)
#define MAP_START_POINT_X 100
#define MAP_START_POINT_Y 100

// 루프 관련
bool g_bLoop = true;

// 시간 측정
LARGE_INTEGER tTime;
LARGE_INTEGER g_tsecond;
LARGE_INTEGER g_tTime;
float g_fDeltatime;


