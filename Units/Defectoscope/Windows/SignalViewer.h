#pragma once

#include "Windows/CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"
#include "Graphics\Borders.h"

class SignalWindow;

class XViewer: public CommonViewer
{
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesGrid
		, LineSeries
		, Grid		
	>::Result> TChart;
	TChart tchart;

	double scan[1024];

	XViewer();
};

class SignalViewer: public XViewer
{
public: typedef XViewer Parent;
		SignalWindow &owner;
		SignalViewer(SignalWindow &);
};

class ACFViewer: public CommonViewer
{
	struct LeftBorder: VBorder{LeftBorder(Chart &chart): VBorder(chart){}};
	struct RightBorder: VBorder{RightBorder(Chart &chart): VBorder(chart){color = 0xffff0000;}};
public:
	void(ACFViewer::*ptrProc)(TMouseWell &l);
public: typedef CommonViewer Parent;
		typedef ChartDraw<Chart, TL::MkTlst<
			LeftAxes
			, BottomAxesGrid
			, LineSeries
			, Grid	
			, LeftBorder
			, RightBorder
		>::Result> TChart;
		TChart tchart;
		SignalWindow &owner;
		double scan[1024];

		ACFViewer(SignalWindow &);

		bool Draw(TMouseMove &l, VGraphics &g);
		void operator()(TRButtonDown &);
		void operator()(TSize &);
		void operator()(TMouseWell &);

		void LeftCutOff(TMouseWell &);
		void RightCutOff(TMouseWell &);
};

class FFTViewer: public CommonViewer
{
	struct LeftBorder: VBorder{LeftBorder(Chart &chart): VBorder(chart){}};
	struct RightBorder: VBorder{RightBorder(Chart &chart): VBorder(chart){color = 0xffff0000;}};

	struct Peak: HBorder{Peak(Chart &chart): HBorder(chart){}};
	struct MinEnergy: HBorder{MinEnergy(Chart &chart): HBorder(chart){color = 0xffff0000;}};
	struct MaxEnergy: HBorder{MaxEnergy(Chart &chart): HBorder(chart){color = 0xff;}};

public:
public: 
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesGrid
		, LineSeries
		, Grid	
		, LeftBorder
		, RightBorder
		, Peak
		, MinEnergy
		, MaxEnergy
	>::Result> TChart;
	TChart tchart;
	SignalWindow &owner;
	double scan[1024];
	FFTViewer(SignalWindow &);
	bool Draw(TMouseMove &l, VGraphics &g);

	void operator()(TSize &);
	//void operator()(TMouseWell &);
};

