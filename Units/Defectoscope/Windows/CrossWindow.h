#pragma once
#include "CrossViewer.h"
#include "Windows/LineViewer.h"
#include "templates/typelist.hpp"
#include "DataItem/ConstData.h"
#include "Windows/LineChart.hpp"
#include "Common.h"
#include "Windows\WindowAdjustingMultipliers\WindowAdjustingMultipliers.hpp"

class CrossWindow
{
public:
	typedef Cross sub_type;
	int lastZone;
	const int &count_sensors;
public:
	HWND hWnd;
	bool drawZones;
	typedef NoSubMenu<CrossViewer, CrossWindow> MainChart;
	typedef TL::CreateWapperNumList<Line, CrossWindow, 0, App::count_cross_sensors - 1>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<MainChart>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	typedef TL::ListToWapperList<line_list, AdjustingMultipliers>::Result ajust_window_list;
	TL::Factory<ajust_window_list> adjustItem;
	CrossViewer &viewer;
	CrossWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);	
};