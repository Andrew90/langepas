#include "stdafx.h"
#include "SignalViewer.h"
#include "lir\SubLir.h"
#include "templates\typelist.hpp"
#include "window_tool\EmptyWindow.h"

LRESULT SignalViewer::operator()(TCreate &l)
{
	ptr = NULL;
	startX = stopX = startY = stopY = 0;
	this->Parent::operator()(l);
	SetParam();
	return 0;
}

void SignalViewer::operator()(TLButtonDown &l)
{
		startX = l.x;
		startY = l.y;
}

void SignalViewer::operator()(TLButtonUp &l)
{
	int dx = l.x - startX;
	int dy = l.y - startY;
	bool b = false;
	double x0, y0, x1, y1;
	if(dx > 0 && dy > 0)
	{		
		chartLoc.AxesValues(startX, startY, x0, y0);
		chartLoc.AxesValues(l.x, l.y, x1, y1);
		if(x1 - x0 > 0)
		{
			xWidth = int(x1 - x0);
			b = true;
		}
		if(y0 - y1 > 0)
		{
			chartLoc.minAxesY = y1;
	        chartLoc.maxAxesY = y0;
			b = true;
		}
		
	}
	else if(dx <= 0 || dy <= 0)
	{
		TestGraphAxesTable::TItems &t = Singleton<TestGraphAxesTable>::Instance().items;
		xWidth =  t.get<PrimarySignalWidth>().value;
		chartLoc.minAxesY = t.get<PrimarySignalMin>().value;
		chartLoc.maxAxesY = t.get<PrimarySignalMax>().value;
		x0 = chartLoc.minAxesX;
		b = true;
	}
	if(b)
	{
		chartLoc.minAxesX = x0;
		chartLoc.maxAxesX = x0 + xWidth;
		RepaintWindow(l.hwnd);
	}
};

void SignalViewer::operator()(TRButtonDown &l)
{
	startX = l.x;
	startY = l.y;
	ptr = &SignalViewer::RightBtn;
}

void SignalViewer::operator()(TRButtonUp &l)
{
	ptr = NULL;
}

void SignalViewer::operator()(TMouseMove &l)
{
	if(NULL != ptr)(this->*ptr)(l.x, l.y);
}

void SignalViewer::RightBtn(int x, int y)
{
	double x0, y0, x1, y1;
	chartLoc.AxesValues(startX, startY, x0, y0);
	chartLoc.AxesValues(x, y, x1, y1);

	x1 -= x0;
	chartLoc.minAxesX -= x1;
	chartLoc.maxAxesX -= x1;	  

	y0 -= y1;
	chartLoc.minAxesY += y0;
	chartLoc.maxAxesY += y0;

	RepaintWindow(hWnd);
	startX = x;
	startY = y;
}

void SignalViewer::SetParam()
{
	TestGraphAxesTable::TItems &t = Singleton<TestGraphAxesTable>::Instance().items;
	xOffset = 0;
	chartLoc.minAxesX = 0;
	chartLoc.maxAxesX = t.get<PrimarySignalWidth>().value;
	xWidth =  (int)chartLoc.maxAxesX;
	chartLoc.minAxesY = t.get<PrimarySignalMin>().value;
	chartLoc.maxAxesY = t.get<PrimarySignalMax>().value;
}

void SignalViewer::OffsetLeft()
{
	xOffset -= xWidth;
	if(xOffset < 0)	xOffset = 0;
	chartLoc.minAxesX = xOffset;
	chartLoc.maxAxesX = xOffset + xWidth;
}

void SignalViewer::OffsetRight()
{
	SubLir &lir = Singleton<SubLir>::Instance();
	int max = lir.samples[lir.index - 1];
	xOffset += xWidth;
	if(xOffset > max)  xOffset = max - xWidth;
	if(chartLoc.minAxesX > xOffset)	return;
	chartLoc.minAxesX = xOffset;
	chartLoc.maxAxesX = xOffset + xWidth;
}
//----------------------------------------------------------------------
SignalViewer::Series::Series(Chart &c): chart(c){}
namespace
{
typedef TL::CreateNumList<SignalViewer::LineUnit, 0, SignalViewer::count_channels - 1>::Result sensors_list;
struct DrawUnitData
{
	int offset, count;
	LineSeries &line;
	TestLineOnTable::TItems &on;
	TestLineColorTable::TItems &color;
	DrawUnitData(LineSeries &line, int offset, int count)
		: line(line)
		, offset(offset)
		, count(count)
		, on(Singleton<TestLineOnTable>::Instance().items)
		, color(Singleton<TestLineColorTable>::Instance().items)
	{}
};
template<class T>struct layer_draw_unit
{
	typedef T Result;
};
template<>struct layer_draw_unit<Voltage>
{
	typedef Solid Result;
};
template<>struct layer_draw_unit<Current>
{
	typedef Solid Result;
};
template<class O, class P>struct draw_unit
{
	void operator()(P &p)
	{
		if(p.on.get<TestLineOn<O::NUM>>().value)
		{
			typedef typename layer_draw_unit<typename O::type>::Result T;
			ItemData<T> &data = Singleton<ItemData<T>>::Instance();
			p.line.color = p.color.get<TestLineColor<O::NUM>>().value;

			p.line.SetData(&data.ascan[O::sensor][p.offset], p.count);
			p.line.Draw();
		}
	}
};
}
void SignalViewer::Series::Draw()
{
	TL::foreach<sensors_list, draw_unit>()(DrawUnitData(
		LineSeries(chart)
		, owner->xOffset
		, owner->xWidth
		));
}