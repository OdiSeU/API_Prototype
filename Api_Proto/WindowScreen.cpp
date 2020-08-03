#include "WindowScreen.h"

Screen::Screen()
{
	rect = { 0,0,1400,800 };
	adjustRect = rect;
	AdjustWindowRect(&adjustRect, WS_OVERLAPPEDWINDOW, FALSE);
	width = adjustRect.right - adjustRect.left;
	height = adjustRect.bottom - adjustRect.top;
}
void Screen::SetScreen(int width, int height)
{
	rect = { 0,0,width,height };
	adjustRect = rect;
	AdjustWindowRect(&adjustRect, WS_OVERLAPPEDWINDOW, FALSE);
	width = adjustRect.right - adjustRect.left;
	height = adjustRect.bottom - adjustRect.top;
}

void Screen::ChangeScreenSize(HWND hwnd)
{
	SetWindowPos(hwnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0);
}