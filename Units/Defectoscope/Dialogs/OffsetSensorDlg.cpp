#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "tools_debug\DebugMess.h"
#include "window_tool\Dlgfiles.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "DataItem/ConstData.h"

namespace
{
#define OffsSQ(a, b) OffsSQ<a, b>

	PARAM_TITLE(OffsSQ(Cross, 1), L"Вход поперечный модуль(SQ1po)")
	PARAM_TITLE(OffsSQ(Cross, 2), L"Выход поперечный модуль(SQ2po)")
	PARAM_TITLE(OffsSQ(Long , 1), L"Вход продольный модуль(SQ1pr)")
	PARAM_TITLE(OffsSQ(Long , 2), L"Выход продольный модуль(SQ2pr)")
	PARAM_TITLE(OffsSQ(Thick, 1), L"Вход модуль толщины(SQ1t)")
	PARAM_TITLE(OffsSQ(Thick, 2), L"Выход модуль толщины(SQ2t)")
	PARAM_TITLE(OffsSQ(Magn , 1), L"Вход модуль размагничивания(SQ1DM)")
	PARAM_TITLE(OffsSQ(Magn , 2), L"Выход модуль размагничивания(SQ2DM)")

#undef OffsSQ
	typedef TL::ListToWapperList<OffsetSensorsTable::items_list, DlgItem>::Result list;
	template<class O, class P>struct __set_buf__
	{
		void operator()(O &o, P &p)
		{
			wchar_t buf[64];
			GetWindowText(o.hWnd, buf, dimention_of(buf));
			int val = _wtoi(buf);
			p[TL::IndexOf<list, O>::value] = val;
		}
	};

	struct __data__
	{
		int i;
		wchar_t *txt;
	};

	template<class O, class P>struct __param__
	{
		bool operator()(P &p)
		{
			if(p.i == TL::IndexOf<list, O>::value)
			{
				p.txt = ParamTitle<TL::Inner<O>::Result>()();
				return false;
			}
			return true;
		}
	};

	
}

namespace TL
{
	template<>struct find<list, __test__>
	{		
		template<class O, class P>bool operator()(O *o, P *p)
		{
			int buf[TL::Length<list>::value];
			TL::foreach<list, __set_buf__>()((TL::Factory<list> &)o, buf);
			for(int i = 0; i < TL::Length<list>::value - 1; ++i)
				for(int j = 1; j < TL::Length<list>::value; ++j)
				{
					if(buf[i] >= buf[j])
					{
						__data__ data1 = {i, NULL};
						TL::find<list, __param__>()(data1);
						__data__ data2 = {j, NULL};
						TL::find<list, __param__>()(data2);
						wchar_t txt[128];
						wsprintf(txt, L"Некорректные параметры \"%s\" и \"%s\""
							, data1.txt
							, data2.txt
							);
						MessageBox(*p, txt, L"Ошибка!!!", MB_ICONERROR);
						return false;
					}
				}
				return true;
		} 
	};
}

void OffsetSensorDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, OffsetSensorsTable, DlgItem
	>(Singleton<OffsetSensorsTable>::Instance()).Do(h, L"Смещение датчиков наличия трубы в модуле(мм)")
	)
	{
	}
}