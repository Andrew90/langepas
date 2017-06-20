#pragma once
#include "window_tool/message.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "templates/typelist.hpp"
#include "Graphics/Borders.h"
#include "Dates/ChartData.h"
#include "Common/Viewer.h"
#include "templates/HexColor.h"

class BottomGroupAxes
{
	double minA, maxA;
	int offsMax;
	int offsMin;
	double deltaTickDigit;
public:
  Chart &chart;
  BottomGroupAxes(Chart &chart);
  void Draw();
  void OffsetToPixel(WORD &, int);
};

class PointsViewer: public View
{	
public:
	template<int N>class Signal: public LineSeries
	{
	public:
		double data[1024];
		Signal(Chart &c): LineSeries(c){color = hex_color[N];}
	};
public:
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, Signal<0>
		, Signal<1>
		, Signal<2>
		, Signal<3>
		, Signal<4>
		, Signal<5>
		, Signal<6>
		, Signal<7>
		, Grid
		, BottomGroupAxes
	>::Result> chart;
public:
	typedef View Parent;
	HWND hWnd;
	PointsViewer();
	void BeforeDraw(Gdiplus::Graphics &);	
	void FillSignals();
	void operator()(TMouseWell &);
};
