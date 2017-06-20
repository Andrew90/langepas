#include "stdafx.h"
#include "PointsViewer.h"
#include <stdio.h>
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include "Dates/SolidData.h"
#include "tools_debug/DebugMess.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "Graphics/Chart.h"
#include "FrameWindow\FrameWindow.h"
//#include "MainWindow\MainWindow.h"

namespace
{
	template<class O, class P>struct __set_point__;
	template<int N, class P>struct __set_point__<PointsViewer::Signal<N>, P>
	{
		typedef PointsViewer::Signal<N> O;
		void operator()(O &o, P &p)
		{
			unsigned len = p.size() > dimention_of(o.data)
				? dimention_of(o.data)
				: p.size()
				;
			for(unsigned i = 0; i < len; ++i)
			{
				o.data[i] = p[i].points[N];
			}
			o.SetData(o.data, len);
		}
	};
}
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

#pragma warning(disable : 4996)

PointsViewer::PointsViewer()
	: View(chart)
	, chart(backScreen)
{
	chart.rect.left = 10;
	chart.rect.top = 10;
	chart.offsetAxesBottom = 10;
}
//------------------------------------------------------------------------------
void PointsViewer::BeforeDraw(Gdiplus::Graphics &)
{
	FillSignals();
}
//----------------------------------------------------------------------------------------------------
void PointsViewer::FillSignals()
{
	ComputeSolidGroup &sg = Singleton<ComputeSolidGroup>::Instance();
	chart.minAxesX = 0;
	chart.maxAxesX = sg.pointItems.size() - 1;

	chart.minAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMin>().value;
	chart.maxAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMax>().value;

	TL::foreach<TL::CreateNumList<Signal, 0, 7>::Result, __set_point__>()(chart.items, sg.pointItems);
	cursor.horizontalLine = true;
}
//----------------------------------------------------------------------
using namespace Gdiplus;
BottomGroupAxes::BottomGroupAxes(Chart &chart)
	: chart(chart) 
	, minA(100)
	, maxA(100)
{
}
void BottomGroupAxes::Draw()
{
	int y = chart.rect.bottom - chart.offsetAxesBottom - 20;
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft - 3)
		, REAL(y - 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) + 3)
		, REAL(chart.rect.bottom - 10)
		)),
		CombineModeReplace
		);	
	Gdiplus::Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	SolidBrush fontColor(chart.colorFontAxes);
	double height;	

	PointF origin, groupN;
	RectF rect;	
	double length = chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	int maxLen = 0;

	ComputeSolidGroup &sg = Singleton<ComputeSolidGroup>::Instance();
	int lenPoints = sg.pointItems.size();

	OffsetAxes(
		int(2.5 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, chart.minAxesX
		, chart.maxAxesX
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
	chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;
	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;
	y = chart.rect.bottom - chart.offsetAxesBottom;
	digit += deltaDigit;
	origin.Y = REAL(y - 12);
	groupN.Y = REAL(y - 25);
	if(lenPoints)
	{	
		while(offs < offsMax)
		{		
			groupN.X = (REAL)offs;
			int t = (int)digit;
			if(t < lenPoints)
			{
				wchar_t *s = sg.pointItems[t].GroupName();
				int len = wcslen(s);
				chart.g->DrawString(s, len, &font, groupN, &fontColor);
				if(0 == t % 2)
				{
					wchar_t buf[128];
					_itow( sg.pointItems[t].offs, buf, 10);
					origin.X = groupN.X;
					len = wcslen(buf);
					chart.g->DrawString(buf, len, &font, origin, &fontColor);
				}
			}
			offs += deltaTick;
			digit += deltaDigit;
		}
	}
	maxA = digit;
	chart.g->SetClip(&Region());
}

void PointsViewer::operator()(TMouseWell &l)
{
	mouseMove = false;

	offsetX += l.delta / 120;
	if(offsetX < 0) offsetX = 0;
	if(offsetX >= chart.maxAxesX) offsetX = int(chart.maxAxesX);

	double x = chart.rect.left + chart.offsetAxesLeft;
	double lenX = chart.rect.right - chart.offsetAxesRight - chart.rect.left - chart.offsetAxesLeft;
	double dX = lenX / (chart.maxAxesX - chart.minAxesX);

	storedMouseMove.x = WORD(x + dX * offsetX);

	cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));	

	ComputeSolidGroup &sg = Singleton<ComputeSolidGroup>::Instance();
	int lenPoints = sg.pointItems.size();

	if(offsetX > 0 && offsetX < lenPoints)
	{
		FrameWindow &f = Singleton<FrameWindow>::Instance();
		f.frameViewer.x =  sg.pointItems[offsetX - 1].offs + (sg.pointItems[offsetX].offs - sg.pointItems[offsetX - 1].offs) / 2;
		f.IncDecFrame();
		RepaintWindow(f.frameViewer.hWnd);
	}
}


