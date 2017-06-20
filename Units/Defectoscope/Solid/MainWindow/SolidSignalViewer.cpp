#include "stdafx.h"
#include "SolidSignalViewer.h"
#include <stdio.h>
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include "Dates/SolidData.h"
#include "tools_debug/DebugMess.h"
#include "FrameWindow\FrameWindow.h"
//#include "MainWindow\MainWindow.h"
#include "MainWindow/ChartLines.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
}
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

#pragma warning(disable : 4996)

SolidSignalViewer::SolidSignalViewer()
	: View(chart)
	, chart(backScreen)
	, signal   (Singleton<L502Signal>::Instance())
	, reference(Singleton<L502Reference>::Instance())
{
	chart.rect.left = 10;
	chart.rect.top = 10;
	chart.offsetAxesBottom = 10;	
}
//------------------------------------------------------------------------------
void SolidSignalViewer::BeforeDraw(Gdiplus::Graphics &)
{
	 chart.maxAxesX = solidData.currentOffset;
	 chart.items.get<SignalMin>().SetData(signal.dataMin, signal.count);
	 chart.items.get<SignalMax>().SetData(signal.dataMax, signal.count);
	 chart.items.get<ReferenceMin>().SetData(reference.dataMin, reference.count);
	 chart.items.get<ReferenceMax>().SetData(reference.dataMax, reference.count);
	 chart.items.get<VCursor1>().value = solidData.start;
	 chart.items.get<VCursor2>().value = solidData.stop;
	 chart.minAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMin>().value;
	 chart.maxAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMax>().value;
}
//----------------------------------------------------------------------------------------------------
void SolidSignalViewer::operator()(TLButtonDown &){}
void SolidSignalViewer::operator()(TRButtonDown &l)
{
	//todo разобраться 
	//zprint("sign %d %d  %d\n", l.x, l.y, offsetX);
	//Singleton<FrameWindow>::Instance().frameViewer.x = offsetX;
	//MainWindow &w = Singleton<MainWindow>::Instance();
	//TCommand c = {w.hWnd, WM_COMMAND, IDB_OptionsBtn, 0, w.toolBar.hWnd};
	//SendMessage(MESSAGE(c));
	//FrameWindow &f = Singleton<FrameWindow>::Instance();
	//f.IncDecFrame();
	//RepaintWindow(f.frameViewer.hWnd);
}

void SolidSignalViewer::operator()(TMouseWell &l)
{
	mouseMove = false;
	if(0 == l.flags.lButton)
	{
		offsetX += l.delta / 120;
		if(offsetX < 0) offsetX = 0;
		if(offsetX >= chart.maxAxesX) offsetX = int(chart.maxAxesX);

		double x = chart.rect.left + chart.offsetAxesLeft;
		double lenX = chart.rect.right - chart.offsetAxesRight - chart.rect.left - chart.offsetAxesLeft;
		double dX = lenX / (chart.maxAxesX - chart.minAxesX);

		storedMouseMove.x = WORD(x + dX * offsetX + dX / 2);
	}
	cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));	

	FrameWindow &f = Singleton<FrameWindow>::Instance();
	if(NULL != f.hWnd)
	{		
		f.frameViewer.x =  offsetX;
		f.IncDecFrame();
		RepaintWindow(f.frameViewer.hWnd);
	}
}



