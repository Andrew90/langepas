#include "stdafx.h"
#include "Viewer.h"
#include "App\App.h"
#include "tools_debug\DebugMess.h"
#include "Graphics\Color.h"

using namespace Gdiplus;

View::View(Chart &c)
	: backScreen(NULL)
	, chart(&c)
    , cursor(c)
	, offsetX(1)
	, offsetY(1)
{
	label.fontHeight = 12;
	label.top = 0;	
	mouseMove = true;
}

LRESULT View::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	currentX = currentY = 0;
	storedMouseMove.x = WORD((chart->rect.right - chart->rect.left) / 2);	
	storedMouseMove.y = WORD((chart->rect.bottom - chart->rect.top) / 2);
	return 0;
}

void View::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	

	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
			backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}

	SizeOffs(storedMouseMove);

	Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29); 

	chart->rect.right = l.Width;
	chart->rect.bottom = l.Height - 10;

	BeforeDraw(g);
	label.Draw(g);
	chart->Draw(g);	
}
void View::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.CrossCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}

void View::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
		storedMouseMove.y = l.y;
	}
}

void View::operator()(TLButtonDown &l)
{
	mouseMove = false;
}

void View::operator()(TMouseWell &l)
{
	mouseMove = false;
	if(0 == l.flags.lButton)
	{
		offsetX += l.delta / 120;
		if(offsetX < 0) offsetX = 0;
		if(offsetX >= chart->maxAxesX) offsetX = int(chart->maxAxesX - 1);

		double x = chart->rect.left + chart->offsetAxesLeft;
		double lenX = chart->rect.right - chart->offsetAxesRight - chart->rect.left - chart->offsetAxesLeft;
		double dX = lenX / (chart->maxAxesX - chart->minAxesX);

		storedMouseMove.x = WORD(x + dX * offsetX + dX / 2);
	}
	else
	{
		offsetY += l.delta / 120;
		if(offsetY < 0) offsetY = 0;
		if(offsetY >= chart->maxAxesY) offsetY = int(chart->maxAxesY - 1);

		double y = chart->rect.bottom - chart->offsetAxesBottom;
		double lenY = chart->rect.bottom - chart->offsetAxesBottom - chart->rect.top - chart->offsetAxesTop;
		double dY = lenY / (chart->maxAxesY - chart->minAxesY);

		storedMouseMove.y = WORD(y - dY * offsetY - dY / 2);
	}
	cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));	
}

void View::SizeOffs(TMouseMove &l)
{
	double x = chart->rect.left + chart->offsetAxesLeft;
	double lenX = chart->rect.right - chart->offsetAxesRight - chart->rect.left - chart->offsetAxesLeft;
	double dX = lenX / (chart->maxAxesX - chart->minAxesX);

	double offs = (double)l.x - x;
	if(offs > 0)
	{
		offsetX = int(offs / dX);
		if(offsetX >= chart->maxAxesX - chart->minAxesX) offsetX = int(chart->minAxesX - 1);
	}

	double y = chart->rect.bottom - chart->offsetAxesBottom;
	double lenY = chart->rect.bottom - chart->offsetAxesBottom - chart->rect.top - chart->offsetAxesTop;
	double dY = lenY / (chart->maxAxesY - chart->minAxesY);

	offs = (double)y - l.y;
	if(offs > 0)
	{
		offsetY = int(offs / dY);
		if(offsetY >= chart->maxAxesY) offsetY = int(chart->maxAxesY - 1);
	}
}

void View::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		SizeOffs(l);
		cursor.CrossCursor(l, HDCGraphics(l.hwnd, backScreen));	
		storedMouseMove = l;
	}
}
