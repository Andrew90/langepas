#pragma once
#include "App/App.h"
#include "window_tool/SelectTypeSizeList.h"
#include <windows.h>
#include "window_tool/message.h"
#include "Windows/TopLabelViewer.h"
#include "window_tool/CheckBoxWidget.h"
#include "MainWindowToolBar.h"
#include <CommCtrl.h>
#include "App/AppBase.h"
#include "Windows/ThicknessViewer.h"
#include "Windows/CrossViewer.h"
#include "Windows/LongViewer.h"
#include "Windows/ResultViewer.h"

template<class T, class Parent>struct OnTheJobCheckBox
{
protected:
	void Command(TCommand &m, bool b)
	{
		Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<T> >().value = b;
		Parent::CheckBoxStateStoreInBase();
	}
	bool Init(HWND h)
	{		
		return Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<T> >().value;
	}
};

class MainWindow
{
public:
	HWND hWnd;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
	CheckBoxWidget<OnTheJobCheckBox<ACS    , MainWindow> > acsCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<Long     , MainWindow> > longCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<Thick, MainWindow> > ThickCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<ViewInterrupt, MainWindow> > viewInterruptCheckBox;
	TopLabelViewer topLabelViewer;
	SelectTypeSizeList select;
	typedef TL::MkTlst<
		CrossViewer
		, LongViewer
		, ThickViewer
		, ResultViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TMouseWell &);
	void operator()(TUser &);
	void operator()(TClose &);
	static void CheckBoxStateStoreInBase();

	void ClearCharts();

	void EnableTool();
	void DisableTool();
};
