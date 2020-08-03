#include <windows.h>
#pragma once
// 모니터 해상도
#define DEFUALT_WINDOWSIZE_WIDTH 1920
#define DEFUALT_WINDOWSIZE_HEIGHT 1080

static RECT Crect;

void SetDialog(RECT WinSizeRect);

RECT HD();

RECT FHD();
