#include "stdafx.h"
#include "window_tool\Emptywindow.h"
#include "Windows/MainWindow.h"

void ChangeTypeSize()
{
	MainWindow &o = Singleton<MainWindow>::Instance();
	CrossViewer &c = o.viewers.get<CrossViewer>();
	c.tchart.maxAxesY = 1 + c.count_sensors;
	RepaintWindow(o.hWnd);
}