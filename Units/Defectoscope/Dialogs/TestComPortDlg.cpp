#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "ASU\Asu.h"
#include "App\App.h"

namespace
{
	using namespace Communication;
	HFONT hFont;
	template<class T>struct DlgItemsLabel
	{		
		HWND Init(HWND h, int &width, int &dy, T &)
		{	
			HWND hWnd = CreateWindow(L"static", L"Нажмите кнопку тест"
				, WS_VISIBLE | WS_CHILD
				, 30, dy, 600, 30, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			dy += 30;
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
			return hWnd;
		}
	};

	DEFINE_PARAM(Mess, bool, false)

	struct Label
	{
		typedef TL::MkTlst<
			Mess
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};

	template<>struct DlgSubItems<Mess , bool>: DlgItemsLabel<Mess>{};

	template<int(*Test)(ComPort &)>struct StartBtnPos
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Тест";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			SetWindowText(owner.items.get<DlgItem<Mess>>().hWnd, L"Запрос к модулю");
			int ret = Test(comPort);			
			wchar_t *txt = L"Ошибка!";
			switch(ret)
			{
			case ok			  : txt = L"Тест прошёл успешно"					;break;
			case time_overflow: //txt = L"Ошибка! Превышено время ожидания"		;break;
			case error_crc    : //txt = L"Ошибка! Не совпадает контрольная сумма"	;break;
			case error_count  : txt = L"Нет связи с модулем";/*L"Ошибка! Длина сообщения"*/				;break;				
			}

			SetWindowText(owner.items.get<DlgItem<Mess>>().hWnd, txt);
		}
	};
	template<int(*ptr)(ComPort &)>void __Do__(HWND h, wchar_t *title)
	{
		hFont = CreateFont (26, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"));
		TemplDialog<NullType, Label, DlgItem, TL::MkTlst<StartBtnPos<ptr>, CancelBtn>::Result>(Label()).Do(h, title);
		DeleteObject(hFont); 
	}
}

void ThicknessComTest::Do(HWND h)
{
	__Do__<Communication::Thick::Test>(h, L"Тестирование связи с толщиномером");
}

void ASUComTest::Do(HWND h)
{
	__Do__<Communication::Asu::Test>(h, L"Тестирование связи с АСУ");
}