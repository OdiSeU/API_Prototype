#include <windows.h>
#pragma once

class Screen
{
public:
	RECT rect;
	RECT adjustRect;
	int width;
	int height;
public:
	Screen();
	void SetScreen(int width, int height);
	void ChangeScreenSize(HWND hwnd);
};