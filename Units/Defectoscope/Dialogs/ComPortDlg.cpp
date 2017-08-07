#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

namespace
{
	DO_NOT_CHECK(NumberComPort)
		DO_NOT_CHECK(Speed)

		PARAM_TITLE(NumberComPort, L"Номер COM-порта")
		PARAM_TITLE(Speed, L"Скорость")

		int speed[] = {4800, 7200, 9600, 14400, 19200, 38400};
	template<>struct FillComboboxList<Speed>			 
	{															 
		void operator()(HWND h, Speed &t)			 
		{	
			wchar_t buf[16];
			for(int i = 0; i < dimention_of(speed); ++i)	 
			{	
				_itow(speed[i], buf, 10);
				ComboBox_AddString(h, buf);			 
			}													 
		}														 
	};															 
	template<>struct CurrentValue<Speed>				 
	{															 
		void operator()(HWND h, Speed &t)			 
		{		
			for(int i = 0; i < dimention_of(speed); ++i)
			{
				if(t.value == speed[i])
				{
					ComboBox_SetCurSel(h, i);
					break;
				}
			}
		}
	};
	template<>struct DlgSubItems<Speed, int>: ComboBoxSubItem<Speed>{};
	template<class P>struct __ok_btn__<DlgItem<Speed>, P>
	{  
		typedef DlgItem<Speed> O; 
		void operator()(O *o, P *p)	 
		{						   
			wchar_t s[128];					
			GetWindowText(o->hWnd, s, dimention_of(s));	
			int t = _wtoi(s);
			for(int i = 0; i < dimention_of(speed); ++i)	
			{																		  
				if(t == speed[i])
				{
					o->value.value = t;
					p->update.set<typename TL::Inner<O>::Result>(o->value.value);
					break;	  
				}
			}												 
		}	 
	};
}

void ComPortDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, ComPortTable, DlgItem
	>(Singleton<ComPortTable>::Instance()).Do(h, L"Настройка COM-порта"))
	{
		ComPortTable::TItems & comPortParam = Singleton<ComPortTable>::Instance().items;
		comPort.Close();
		Sleep(100);
		if(!comPort.Open(comPortParam.get<NumberComPort>().value, comPortParam.get<Speed>().value))
		{
			MessageBox(h, L"Не могу инициировать COM-порт", L"Ошибка !!!", MB_ICONERROR);
		}
	}
}
									 
