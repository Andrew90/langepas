#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "tools_debug\DebugMess.h"
#include "Resource.h"
#include "templates/typelist.hpp"
#include "window_tool/InitToolBar.hpp"
#include "Compute/Automat.h"
#include "StoredData\StoredBase.h"
#include "App/App.h"
#include "Windows/MainWindow.h"
#include "Compute\ControlMode.h"
using namespace Gdiplus;
namespace 
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_CycleBtn   , L"F4 Контроль")
	KEY(IDB_Reset      , L"Esc Стоп")
	KEY(IDB_QueryBtn   , L"Тест")
	KEY(IDB_arrow_down , L"F5 Вниз test")
	KEY(IDB_arrow_up   , L"F6 БАЗА")
	KEY(IDB_arrow_left , L"F7 Влево")
	KEY(IDB_arrow_right, L"F8 Clear")
	KEY(IDB_Continue, L"F11 Продолжить")
	KEY(IDB_ExitTubeBtn, L"F9 Выгон")
	KEY(IDB_ReturnTubeBtn, L"F10 Возврат")
	//KEY(IDB_ClampBtn, L"Установка")
	//KEY(IDB_SensorsUnit, L"Вращение")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Continue)
		, BUTTON_KEY(IDB_Reset)
		//, BUTTON_KEY(IDB_QueryBtn) 		
		, SeparatorToolbar<1>
		, BUTTON_KEY(IDB_ExitTubeBtn)
		, BUTTON_KEY(IDB_ReturnTubeBtn)
		, SeparatorToolbar<2>
		//, BUTTON_KEY(IDB_ClampBtn)
		//, BUTTON_KEY(IDB_SensorsUnit)
		//, SeparatorToolbar<3>
#ifdef DEBUG_ITEMS
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, SeparatorToolbar<2>
#endif
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------------------------------------------
	static bool closed_packet_dialog = true;
	static bool run_once_per_sycle = false;
	void Key<IDB_CycleBtn>::Click(HWND h)
	{		
		app.mainWindow.DisableTool();
		Automat::Start();		
	}
	void Key<IDB_Continue>::Click(HWND h)
	{
	   Automat::Continue();
	}
	//void Key<IDB_ClampBtn>::Click(HWND h)
	//{
	//}
	//void Key<IDB_SensorsUnit>::Click(HWND h)
	//{}
//-------------------------------------------------------------------------------
	void Key<IDB_QueryBtn>::Click(HWND h)
	{
	//	if(USPC_Test())AppKeyHandler::Run();

	  Stored::Do(123, L"test");
	}
//------------------------------------------------------------------------------

	void Key<IDB_arrow_down>::Click(HWND h)
	{
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
	void Key<IDB_ReturnTubeBtn>::Click(HWND h)
	{
		Automat::ReturnTube();
	}

	void Key<IDB_Reset>::Click(HWND h)
	{
		app.mainWindow.EnableTool();
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


