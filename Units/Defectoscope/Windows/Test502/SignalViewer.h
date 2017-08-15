#pragma once
#include "Windows\LineViewer.h"

class SignalViewer: public LineViewer
{
public:
	template<int N>struct LineUnit;	
#define CROSS(n)template<>struct LineUnit<n>{typedef Cross type;static const int NUM = n; static const int sensor = n;};
#define LONG(n)template<>struct LineUnit<n>{typedef Long type;static const int NUM = n; static const int sensor = n - 12;};
#define SOLID(n)template<>struct LineUnit<n>{typedef Solid type;static const int NUM = n; static const int sensor = n - 12 - 4;};
CROSS(0)
CROSS(1)
CROSS(2)
CROSS(3)
CROSS(4)
CROSS(5)
CROSS(6)
CROSS(7)
CROSS(8)
CROSS(9)
CROSS(10)
CROSS(11)

LONG(12)
LONG(13)
LONG(14)
LONG(15)

SOLID(16)
SOLID(17)

#undef CROSS
#undef LONG
#undef SOLID
public:
	static const int count_channels = App::count_cross_sensors + App::count_long_sensors + App::count_solid_sensors;
	template<int N>struct SeriesOn
	{
		static const int NUM = N;
		HWND hWnd;
	};
	struct Series
	{
		Chart &chart;
		SignalViewer *owner;
		Series(Chart &);
		void Draw();
	};
public:	
	typedef LineViewer Parent;
	typedef ChartDraw<Chart, typename TL::MkTlst<
		LeftAxes
		, BottomAxes
		, Series
		, Grid	
	>::Result> TChart;
private:
	TChart chartLoc;
	Cursor cursorLoc;
	int xOffset;
	int xWidth;
public:
	SignalViewer()
		: chartLoc(backScreen)
		, cursorLoc(chartLoc)
	{
		chart = &chartLoc;
		cursor = &cursorLoc;
		chartLoc.items.get<Series>().owner = this;
	}
	LRESULT operator()(TCreate &l);	

	void SetParam();

	void OffsetLeft();
	void OffsetRight();
};