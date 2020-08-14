#include <Windows.h>
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