#include<cmath>
#include "UI.h"


void drawEllipse(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, rgb);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);
	Ellipse(hdc, left, top, right, bottom);
	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // �� ����
}

void drawSemicircle(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, rgb);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);
	Pie(hdc, left, top, right, bottom, left + (right - left) / 2, top, left + (right - left) / 2, bottom);
	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // �� ����
}

void drawRectangle(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(rgb);
	HBRUSH OldB = (HBRUSH)SelectObject(hdc, NewB);
	HPEN NewPen = CreatePen(PS_SOLID, 1, BLACK_PEN);
	HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);
	Rectangle(hdc, left, top, right, bottom);
	SelectObject(hdc, OldB);
	DeleteObject(NewB);
	SelectObject(hdc, OldPen);
	DeleteObject(NewPen); // �� ����
}

void DrawPlayerHP(HDC hdc, int MaxHeart, int CurHeart, int Shield)
{
	const POINT startPoint = { 100,200 }; // �׸� ��ġ
	const int hpSize = 50; // �׸� ũ��
	int i;
	COLORREF rgb;

	for (i = 0; i < (MaxHeart + 1) / 2; i++)// �ִ� heart �׸���
	{
		rgb = RGB(10, 20, 30);
		drawEllipse(hdc, startPoint.x + (hpSize * i), startPoint.y, (startPoint.x + hpSize) + (hpSize * i), startPoint.y + hpSize, rgb);// �ִ� ü�� �׸���

		if (i < CurHeart / 2)
		{
			rgb = RGB(255, 0, 0);
			drawEllipse(hdc, startPoint.x + (hpSize * i), startPoint.y, (startPoint.x + hpSize) + (hpSize * i), startPoint.y + hpSize, rgb);// ���� ü�� �׸���
		}
	}
	i--;
	if (CurHeart % 2 == 1) //Ȧ���� ��ĭ¥�� ���� ü�� �׸���
	{
		rgb = RGB(255, 0, 0);
		drawSemicircle(hdc, startPoint.x + (hpSize * i), startPoint.y, (startPoint.x + hpSize) + (hpSize * i), startPoint.y + hpSize, rgb);
	}
	i++;
	for (; i < Shield + (MaxHeart + 1) / 2; i++)
	{
		rgb = RGB(0xE2, 0xE2, 0xE2);
		drawRectangle(hdc, startPoint.x + (hpSize * i), startPoint.y, (startPoint.x + hpSize) + (hpSize * i), startPoint.y + hpSize, rgb);
	}
}