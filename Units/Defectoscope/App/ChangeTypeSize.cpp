#include "stdafx.h"
#include "window_tool\Emptywindow.h"
#include "Windows/MainWindow.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"

void ChangeTypeSize()
{
	MainWindow &o = Singleton<MainWindow>::Instance();
	CrossViewer &c = o.viewers.get<CrossViewer>();
	c.tchart.maxAxesY = 1 + c.count_sensors;

	///смена группы прочности
	NameParam::type_value &nameParam = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	Singleton<ComputeSolidGroup>::Instance().Load(nameParam);

	RepaintWindow(o.hWnd);
}