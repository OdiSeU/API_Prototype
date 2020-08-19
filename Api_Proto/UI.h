#include <windows.h>

#pragma once

//플레이어 체력 실드(실드는 한대 맞을때 무적권 1개) - character.cpp 105 , 체력표기 - UI.cpp // src.cpp 442번줄 추가
void drawEllipse(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb); // 원 그리기
void DrawPlayerHP(HDC hdc, int MaxHeart, int CurHeart, int Shield); // 플레이어 체력 그리기
void drawSemicircle(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb); // 반원 그리기
void drawRectangle(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb); // 사각형 그리기
