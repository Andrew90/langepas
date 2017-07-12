#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "tools_debug\DebugMess.h"
#include "Resource.h"
#include "templates/typelist.hpp"
#include "window_tool/InitToolBar.hpp"
#include "Compute/Automat.h"
#include "StoredData\StoredBase.h"
//#include "Compute\AppKeyHandler.h"
using namespace Gdiplus;
namespace 
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_CycleBtn   , L"F4 ����")
	KEY(IDB_Reset      , L"Esc ����")
	KEY(IDB_QueryBtn   , L"����")
	KEY(IDB_arrow_down , L"F5 ����")
	KEY(IDB_arrow_up   , L"F6 ����")
	KEY(IDB_arrow_left , L"F7 �����")
	KEY(IDB_arrow_right, L"F8 Clear")
	KEY(IDB_Continue, L"F11 ����������")
	KEY(IDB_ExitTubeBtn, L"F9 �����")
	KEY(IDB_SensorsUnit, L"F10 �������")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Continue)
		, BUTTON_KEY(IDB_Reset)
		, BUTTON_KEY(IDB_QueryBtn) 		
		, SeparatorToolbar<1>
		, BUTTON_KEY(IDB_ExitTubeBtn)
		, BUTTON_KEY(IDB_SensorsUnit)
		, SeparatorToolbar<2>
#ifdef DEBUG_ITEMS
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, SeparatorToolbar<2>
#endif
		>::Result tool_button_list;
#undef BUTTON_KEY
	//namespace closed_window
	//{		
	//	//typedef TL::MkTlst<
	//	//	WindowClass<CrossWindow		>
	//	//	, WindowClass<LongWindow		>
	//	//	, WindowClass<ThickWindow	>
	//	//	, WindowClass<ScanWindow		>
	//	//>::Result window_list;
	//	template<class O, class P>struct __close_window__
	//	{
	//		void operator()()
	//		{
	//			HWND hh = FindWindow(O()(), NULL);
	//	    	if(NULL != hh) DestroyWindow(hh);
	//		}
	//	};
	//
	//	void close_window()
	//	{
	//		//TL::foreach<window_list, __close_window__>()();
	//	}
	//}
//----------------------------------------------------------------------------------
	static bool closed_packet_dialog = true;
	static bool run_once_per_sycle = false;
	void Key<IDB_CycleBtn>::Click(HWND h)
	{
		//HWND hh = NULL;
		//for(int i = 0; i < dimention_of(closed_window::name); ++i)
		//{
		//	hh = FindWindow(closed_window::name[i], NULL);
		//	if(NULL != hh) DestroyWindow(hh);
		//}
		//closed_window::close_window();
		//
		//if(closed_packet_dialog)
		//{
		//	closed_packet_dialog = false;
		//	if(run_once_per_sycle || PacketWindowDlg(h)) 
		//	{
		//		CloseAllWindows();
		//		run_once_per_sycle = true;
		//		SetEvent(App::ProgrammRunEvent);
		//		AppKeyHandler::Run();
		//		AutomatAdditional::SetToBottomLabel();
		//		MainWindow *w = (MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		//		SetToolButtonText(w->toolBar.hWnd, IDB_CycleBtn, L"F4 ������");
		//	}
		//	closed_packet_dialog = true;
		//}
		Automat::Start();
	}
	void Key<IDB_Continue>::Click(HWND h)
	{
	//	SetEvent(App::ProgrammContinueEvent);
	//	AppKeyHandler::Run();
	   Automat::Continue();
	}
//-------------------------------------------------------------------------------
	void Key<IDB_QueryBtn>::Click(HWND h)
	{
	//	if(USPC_Test())AppKeyHandler::Run();

	  Stored::Do(123, L"test");
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_down>::Click(HWND h)
	{
	//	LogUSPCWindow::Open();		
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_up>::Click(HWND h)
	{
		zprint("\n");
//test
	//	NumberTubeDlg::Do();
	//	Stored::Do();
//test
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_left>::Click(HWND h)
	{
		zprint("\n");	
		//LogUSPCWindow::Open();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_right>::Click(HWND h)
	{
		zprint("\n");	
		//app.mainWindow.ClearCharts();
	}
//----------------------------------------------------------------------------
	void Key<IDB_ExitTubeBtn>::Click(HWND h)
	{
		Automat::ExitTube();
	}
	void Key<IDB_SensorsUnit>::Click(HWND h)
	{
		Automat::ReturnTube();
	}

	void Key<IDB_Reset>::Click(HWND h)
	{
		//run_once_per_sycle = false;
		//SetEvent(App::ProgrammStopEvent);
		//MainWindow *w = (MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		//SetToolButtonText(w->toolBar.hWnd, IDB_CycleBtn, L"F4 ����");
		//AppKeyHandler::Stop();
		Automat::Stop();
	}
}
//--------------------------------------------------------------------------------------------
HWND MainWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list>()(h);
}
//-------------------------------------------------------------------
void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------


