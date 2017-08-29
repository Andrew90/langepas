#include "stdafx.h"
#include <CommCtrl.h>
#include "Test502Window.h"
#include "window_tool\MenuAPI.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "window_tool/InitToolBar.hpp"
#include "Windows\Common.h"
#include "Windows\Test502\Test502Window.h"
#include "lir\SubLir.h"
#include "Compute\Compute.hpp"
//------------------------------------------------------------------------------
namespace Test502WindowN
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
		
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};
	MENU_ITEM(L"Выход", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<MainExit>
		>::Result list;
	};

	struct Options{};
	MENU_TEXT(L"Настройки", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition    : WindowPositionDlg<Test502Window>{};
	MENU_ITEM(L"Сохранить координаты окна", WindowPosition)

	struct GraphicOptions:  GraphicOptionsDlg{};
	MENU_ITEM(L"График", GraphicOptions)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<GraphicOptions>
			, Separator<0>
			, MenuItem<WindowPosition>
		>::Result list;		
	};
//---------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<Options>
	>::Result MainMenu;	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 

	KEY(IDB_ModeTest   , L"Тест")
		KEY(IDB_LeftArrow   , NULL)
		KEY(IDB_RightArrow   , NULL)

		typedef TL::MkTlst<
		SeparatorToolbar<1>
		, BUTTON_KEY(IDB_ModeTest)
		, SeparatorToolbar<2>
		, BUTTON_KEY(IDB_LeftArrow)
		, BUTTON_KEY(IDB_RightArrow)
		, SeparatorToolbar<3>
		>::Result tool_button_list;

	void Key<IDB_ModeTest>::Click(HWND h)
	{	
		Test502Window *o = (Test502Window *)GetWindowLongPtr(h, GWLP_USERDATA);
		SendMessage(o->hToolBar, TB_ENABLEBUTTON, IDB_ModeTest, MAKELONG(App::measurementOfRunning, 0));
		if(!App::measurementOfRunning)
		{	
			o->topLabelViewer.SetMessage(L"<ff0000>Сбор тестовых данных");
			((Test502Window *)GetWindowLongPtr(h, GWLP_USERDATA))->Test();
		}
		else
		{
			o->topLabelViewer.SetMessage(L"<ff0000>Установка в режиме сбора данных");
		}
	}
	void Key<IDB_LeftArrow>::Click(HWND h)
	{		
		((Test502Window *)GetWindowLongPtr(h, GWLP_USERDATA))->Left();
	}
	void Key<IDB_RightArrow>::Click(HWND h)
	{		
		((Test502Window *)GetWindowLongPtr(h, GWLP_USERDATA))->Right();
	}

//-------------------------------------------------------------------------------
	struct __move_window_data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __move_window__
	{
		void operator()(O *o, P *p)
		{		
			int height =  p->height;
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, height, true);
			p->y += height;
		}
	};
}
//------------------------------------------------------------------------------
Test502Window::Test502Window(): seriesOn(*this){}
void Test502Window::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hToolBar, 0, 0, 0, 0, false);
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);

	RECT rt;
	GetWindowRect(hToolBar, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	

	static const int topLabelHeight = 28;
	int y = rt.bottom - rt.top - 1;
	int t = r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight;
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	Test502WindowN::__move_window_data__ data = {y, r.right, t};
	
	TL::foreach<viewers_list, Test502WindowN::__move_window__>()(&viewers, &data);
}

namespace
{
	template<class T>struct Name;
	template<>struct Name<Cross>{wchar_t *operator()(){return L"Поп";}};
	template<>struct Name<Long>{wchar_t *operator()(){return L"Прод";}};
	template<>struct Name<Voltage>{wchar_t *operator()(){return L"Напр";}};
	template<>struct Name<Current>{wchar_t *operator()(){return L"Ток";}};
	struct __make_checkbox_data__
	{
		HWND hwnd;
		int x, y;
	};
	template<class O, class P>struct __make_checkbox__
	{
		void operator()(O &o, P &p)
		{
			if(p.y > 30)
			{
				p.y = 3;
				p.x += 70;
			}
			wchar_t buf[64];
			typedef SignalViewer::LineUnit<O::NUM> T;
			wsprintf(buf, L"%s %d", Name<typename T::type>()(), 1 +T::sensor);
			o.hWnd = CreateWindow(L"button", buf
			, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
			, p.x, p.y, 70, 14, p.hwnd, NULL, GetModuleHandle(NULL), NULL
			);
			SetWindowLongPtr(o.hWnd, GWLP_USERDATA, (LONG)&o);
			Button_SetCheck(o.hWnd, Singleton<TestLineOnTable>::Instance().items.get<TestLineOn<O::NUM>>().value ? BST_CHECKED : BST_UNCHECKED);
			p.y += 17;
		}
	};
}

LRESULT Test502Window::operator()(TCreate &m)
{
	guard = false;
	Menu<Test502WindowN::MainMenu>().Init(m.hwnd);
	hToolBar = InitToolbar<Test502WindowN::tool_button_list, 16>()(m.hwnd);
	__make_checkbox_data__ data = {hToolBar, 120, 3};
	TL::foreach<series_on_list, __make_checkbox__>()(seriesOn, data);

	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {200, 400, 600, 900};
	SendMessage(hStatusWindow, SB_SETPARTS, dimention_of(pParts), (LPARAM)pParts);

	topLabelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabelViewer);
	topLabelViewer.label.fontHeight = 16;

	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);
	return 0;
}
void Test502Window::operator()(TDestroy &m)
{
	ParametersBase parameters;
	CBase base(ParametersBase().name());

	if(base.IsOpen())
	{
		UpdateWhere<TestLineOnTable>(Singleton<TestLineOnTable>::Instance(), base).ID(1).Execute();
	}
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
}

void Test502Window::operator()(TCommand &m)
{
	EventDo(m);
}

void Test502Window::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}

void Test502Window::operator()(TMouseWell &l)
{
	if(l.delta > 0)
	{
		Left();
	}
	else
	{
		Right();
	}
}

void Test502Window::Do(HWND)
{
	HWND h = FindWindow(WindowClass<Test502Window>()(), 0);
	
	if(NULL != h)
	{			
		RepaintWindow(h);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		Test502Window &o = Singleton<Test502Window>::Instance();
		RECT r;
		WindowPosition::Get<Test502Window>(r);
		HWND h = WindowTemplate(&o, L"Тестирование измерительных каналов платы 502", r.left, r.top, r.right, r.bottom, IDI_Add);
		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		
		ShowWindow(h, SW_SHOWNORMAL);
	}		
}

void Test502Window::SetParam()
{
	SignalViewer &t = viewers.get<SignalViewer>();
	t.SetParam();
	RepaintWindow(t.hWnd);
}

void Test502Window::Thread()
{
	guard = true;

	unit502.Start();
	for(int i = 0; i < 10; ++i)
	{
	  unit502.Read();
	  Sleep(1000);
	}
	unit502.Stop();
	guard = false;
	topLabelViewer.SetMessage(L"<ff>Сбор тестовых данных завершён");
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_ModeTest, MAKELONG(true, 0));
	RepaintWindow(viewers.get<SignalViewer>().hWnd);
}

DWORD WINAPI Test502Window::ThreadProc(_In_ LPVOID lpParameter)
{
	((Test502Window *)lpParameter)->Thread();
	return 0;
}

void Test502Window::Test()
{
	if(!guard)QueueUserWorkItem(ThreadProc, this, WT_EXECUTEDEFAULT);
}

void Test502Window::Left()
{
	SignalViewer &t = viewers.get<SignalViewer>();
	t.OffsetLeft();
	RepaintWindow(t.hWnd);
}

void Test502Window::Right()
{
	SignalViewer &t = viewers.get<SignalViewer>();
	t.OffsetRight();
	RepaintWindow(t.hWnd);
}