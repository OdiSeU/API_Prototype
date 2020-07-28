#include <windows.h>
#include "Resol.h"

//윈도우 크기 바꾸는 헤더 인데 아직 구현x 현재 화면은 사용자 모니터의 크기에 맞게 나오게 설정되어있음

void SetDialog(RECT WinSizeRect)
{
	AdjustWindowRect(&WinSizeRect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = WinSizeRect.right - WinSizeRect.left;

	int height = WinSizeRect.bottom - WinSizeRect.top;

	SetWindowPos(NULL, 0, 0, width, height, 0, NULL);
}

RECT HD()
{
	return { 0,0,1280, 720 };
}

RECT FHD()
{
	return { 0,0,1920,1080 };
}