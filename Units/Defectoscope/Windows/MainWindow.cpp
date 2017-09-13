#include "stdafx.h"
#include "MainWindow.h"
#include "window_tool/MenuApi.h"
#include "MainWindowMenu.hpp"
#include "window_tool\EmptyWindow.h"
#include <CommCtrl.h>
#include "Windows/Common.h"
#include "Compute\ComputeResult.h"
#include "window_tool\ItemIni.h"

#include "tools_debug\DebugMess.h"

namespace {
	struct __move_window_data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __move_window__
	{
		void operator()(O *o, P *p)
		{		
			int height =  2 * p->height;
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, height, true);
			p->y += height;
		}
	};

	template<class P>struct __move_window__<LongViewer, P>
	{
		typedef LongViewer O;
		void operator()(O *o, P *p)
		{			
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, p->height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, p->height, true);
			p->y += p->height;
		}
	};
	template<class P>struct __move_window__<ResultViewer, P>
	{
		typedef ResultViewer O;
		void operator()(O *o, P *p)
		{			
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, p->height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, p->height + 3, true);
			p->y += p->height;
		}
	};
}
//------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();
	
	RECT rt;
	GetWindowRect(toolBar.hWnd, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	
	
	static const int width = toolBar.Width();
	select.Size(width, 5, 400);
	//
	acsCheckBox	 .Size (width + 425, 55, 200, 20);
	longCheckBox	     .Size (width + 425,  5, 400, 20);
	ThickCheckBox        .Size (width + 425, 25, 400, 20);
	viewInterruptCheckBox.Size(width, 55, 400, 20);
	cuttingModeCheckBox	 .Size (width + 425 + 200, 55, 400, 20);
	//
	static const int topLabelHeight = 28;
	int y = rt.bottom - rt.top - 1;
	int t = r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight;
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	t = int((double)t / 6);
	__move_window_data__ data = {y, r.right, t};
	
	TL::foreach<viewers_list, __move_window__>()(&viewers, &data);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	EventDo(m);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}		
}
//------------------------------------------------------------------------
LRESULT MainWindow::operator()(TCreate &m)
{
	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);
	toolBar.Init(m.hwnd);
	select.Create(toolBar.hWnd);
//
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {200, 400, 600, 900};
	SendMessage(hStatusWindow, SB_SETPARTS, dimention_of(pParts), (LPARAM)pParts);
//
	acsCheckBox	 .Init(toolBar.hWnd, L"Режим с АСУ");
	longCheckBox	 .Init(toolBar.hWnd, L"Модуль продольных дефектов");
	ThickCheckBox.Init(toolBar.hWnd, L"Модуль толщины");
	viewInterruptCheckBox.Init(toolBar.hWnd, L"Прерывание на просмотр");
	cuttingModeCheckBox.Init(toolBar.hWnd, L"с отрезкой");
//
	topLabelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabelViewer);
	topLabelViewer.label.fontHeight = 16;
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);
	return 0;
}

//--------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	PostQuitMessage(0);
}
void MainWindow::operator()(TClose &l)
{
	if(IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	{
		DestroyWindow(l.hwnd);
	}
}
//---------------------------------------------------------------------------
typedef void(*TptrMess)(void *);
void MainWindow::operator()(TMessage &m)
{
	if(m.wParam)((TptrMess )(m.wParam))((void *)m.lParam);
}
//-----------------------------------------------------------------------------
namespace Common
{
	template<class P>struct __in_rect__<ResultViewer, P>: __in_rect_all__<ResultViewer, P, MainWindow::viewers_list>{};
}
//------------------------------------------------------------------------------
void MainWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, MainWindow>(*this, l)
		);
}
//--------------------------------------------------------------------------------
void MainWindow::operator()(TUser &l)
{
	(*l.ptr)(l.data);
}
//--------------------------------------------------------------------------------
void MainWindow::CheckBoxStateStoreInBase()
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		UpdateWhere<OnTheJobTable>(Singleton<OnTheJobTable>::Instance(), base).ID(1).Execute();
	}
}
//------------------------------------------------------------------
namespace
{
	template<class O, class P>struct __clear__
	{
		void operator()(O &o)
		{
			o.viewerData.currentOffsetZones = 0;
		}
	};
	template<class P>struct __clear__<ResultViewer, P>
	{
		typedef ResultViewer O;
		void operator()(O &o)
		{
			o.viewerData.currentOffsetZones = 0;
			o.viewerData.cutZone0 = 1;
			o.viewerData.cutZone1 = 1;
		}
	};
}
void MainWindow::ClearCharts()
{
	TL::foreach<viewers_list, __clear__>()(viewers);
	RepaintWindow(hWnd);
}

namespace
{
   struct __enable_data__
   {
	   bool en;
	   HWND h;
   };

   template<class O, class P>struct __enable__
	{
		void operator()(P &p)
		{
           EnableMenu<O>(p.h, p.en);
		}
	};
   template<class O, class P>struct __enable_parent__
	{
		void operator()(P &p)
		{
           EnableMenu<O>(p.h, p.en);
		}
	};
}

namespace
{
	typedef TL::MkTlst<
	//	TopMenu<MainFile>
	//	, TopMenu<MainOptionTypeSize>
		 TopMenu<MainWindowMenu::Options>
		, TopMenu<MainWindowMenu::Setting>
		//, TopMenu<TestUnit>
		//, TopMenu<MainAbout>
	>::Result MainMenu_Enable;	

	//файл
	
	//	typedef TL::MkTlst<
	//		MenuItem<MainWindowMenu::LoadDateFile>
	//		//,  MenuItem<MainWindowMenu::SaveDateFile>
	//		, Separator<0>
	//		//, MenuItem<MainWindowMenu::Compute_>
	//		, Separator<1>
	//		//, MenuItem<MainExit>
	//	>::Result file_list;
	
}

void MainWindow::EnableTool()
{
//	__enable_data__ data = {true, hWnd};
//	TL::foreach<MainMenu_Enable, __enable__>()(data);
//
//	//TL::foreach<file_list, __enable_parent__>()(data);
//
//	EnableWindow(acsCheckBox.hWnd, TRUE);
//	EnableWindow(longCheckBox.hWnd, TRUE);
//	EnableWindow(ThickCheckBox.hWnd, TRUE);
//	EnableWindow(select.hWnd, TRUE);
}

void MainWindow::DisableTool()
{
//	__enable_data__ data = {false, hWnd};
//	TL::foreach<MainMenu_Enable, __enable__>()(data);
//
//	///TL::foreach<file_list, __enable_parent__>()(data);
//
//	EnableWindow(acsCheckBox.hWnd, FALSE);
//	EnableWindow(longCheckBox.hWnd, FALSE);
//	EnableWindow(ThickCheckBox.hWnd, FALSE);
//	EnableWindow(select.hWnd, FALSE);
}

void CuttingMode::Command(TCommand &m, bool b)
{
	wchar_t path[1024];
	ItemIni::GetPath(path);
	withCutting = b;
	ItemIni::Set(L"Additional", L"withCutting", (int)b, path);
}
bool CuttingMode::Init(HWND h)
{
	wchar_t path[1024];
	ItemIni::GetPath(path);
	return withCutting =  0 != ItemIni::Get(L"Additional", L"withCutting", 1, path);
}



