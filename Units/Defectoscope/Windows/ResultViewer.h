#pragma once

#include "CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"
#include "Graphics/EmptyLeftAxes.h"
#include "Graphics\Borders.h"

class ResultViewer: public CommonViewer
{
	class Cut0 : public VBorder
	{
	public:
		Cut0(Chart &c);//:VBorder(c){color = 0xffffffff;widthPen = 3;}
		void Draw();
	};
	class Cut1 : public VBorder
	{
	public:
		Cut1(Chart &c);//:VBorder(c){color = 0xffffffff; widthPen = 3;}
		void Draw();
	};
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		EmptyLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, VerticalFixedGrid
		, Cut0
		, Cut1
	>::Result> TChart;
	TChart tchart;

	ResultData &viewerData;

	ResultViewer();

	bool GetColorBar(unsigned sensor, int zone, double &data, unsigned &color);
	bool Draw(TMouseMove &l, VGraphics &g);

	void operator()(TRButtonDown &l);
};

