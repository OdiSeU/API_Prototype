#include <windows.h>

#pragma once

//�÷��̾� ü�� �ǵ�(�ǵ�� �Ѵ� ������ ������ 1��) - character.cpp 105 , ü��ǥ�� - UI.cpp // src.cpp 442���� �߰�
void drawEllipse(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb); // �� �׸���
void DrawPlayerHP(HDC hdc, int MaxHeart, int CurHeart, int Shield); // �÷��̾� ü�� �׸���
void drawSemicircle(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb); // �ݿ� �׸���
void drawRectangle(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb); // �簢�� �׸���
