#include "stdafx.h"
#include <CommCtrl.h>
#include "Test502Window.h"
#include "window_tool\MenuAPI.h"
#include "window_tool\Emptywindow.h"
//------------------------------------------------------------------------------
namespace
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
//---------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
	>::Result MainMenu;	
}
//------------------------------------------------------------------------------
void Test502Window::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
}

LRESULT Test502Window::operator()(TCreate &m)
{
	Menu<MainMenu>().Init(m.hwnd);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {200, 400, 600, 900};
	SendMessage(hStatusWindow, SB_SETPARTS, dimention_of(pParts), (LPARAM)pParts);
	return 0;
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

void Test502Window::Do(HWND)
{
	HWND h = FindWindow(WindowClass<Test502Window>()(), 0);
	if(NULL == h)
	{			
		Test502Window &self = Singleton<Test502Window>::Instance();
		HWND h = WindowTemplate(&self, L"Тест платы L502", 0,0,0,0, IDI_UPLOAD, (HBRUSH)COLOR_WINDOW);		
		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
		dwStyle &= ~(WS_SYSMENU);
		SetWindowLong(h, GWL_STYLE, dwStyle);
		ShowWindow(h, SW_SHOWNORMAL);
	}	
}