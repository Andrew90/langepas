#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"

/// \brief Настройки смещения и усиления каналов  платы сбора данных

namespace
{
#if 0
#define OFFS_CROSS(N)template<>struct ParamTitle<Offset<Cross, N> >{wchar_t *operator()(){\
	return L"смешение поперечного канала "##L#N;}};
#define OFFS_LONG(N)template<>struct ParamTitle<Offset<Long, N> >{wchar_t *operator()(){\
	return L"смешение продольного канала "##L#N;}};

	typedef Offset<ReferenceSignal, 0> OffsRef ;
	PARAM_TITLE(OffsRef, L"смещение опорного сигнала группы прочности")
		typedef	Offset<InputSignal, 0> OffsInp;
	PARAM_TITLE(OffsInp, L"смещение входного сигнала группы прочности")

		OFFS_CROSS(0)
		OFFS_CROSS(1)
		OFFS_CROSS(2)
		OFFS_CROSS(3)
		OFFS_CROSS(4)
		OFFS_CROSS(5)
		OFFS_CROSS(6)
		OFFS_CROSS(7)
		OFFS_CROSS(8)
		OFFS_CROSS(9)
		OFFS_CROSS(10)
		OFFS_CROSS(11)

		OFFS_LONG(0)
		OFFS_LONG(1)
		OFFS_LONG(2)
		OFFS_LONG(3)

#undef OFFS_CROSS
#undef OFFS_LONG

		template<template<class>class W, template<class, int>class X, class T, int N, class Q>struct Skip<W<X<T, N>>, Q>
	{
		template<class P>bool operator()(W<X<T, N>> *, P *){return true;}
	};

	template<class T, int N>struct DlgSubItems<Offset<T, N>, int >: UpDownSubItem<Offset<T, N>>{};
}

void OffsetsChannelDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase
		, L502OffsetsTable, DlgItem
	>(Singleton<L502OffsetsTable>::Instance()).Do(h, L"Смещения каналов"))
	{
	}
}

namespace
{
#define RANGE_CROSS(N)template<>struct ParamTitle<Range<Cross, N> >{wchar_t *operator()(){\
	return L"усиление поперечного канала "##L#N;}};
#define RANGE_LONG(N)template<>struct ParamTitle<Range<Long, N> >{wchar_t *operator()(){\
	return L"усиление продольного канала "##L#N;}};

	typedef Range<ReferenceSignal, 0> RangeRef ;
	PARAM_TITLE(RangeRef, L"усиление опорного сигнала группы прочности")
	typedef	Range<InputSignal, 0> RangeInp;
	PARAM_TITLE(RangeInp, L"усиление входного сигнала группы прочности")

		RANGE_CROSS(0)
		RANGE_CROSS(1)
		RANGE_CROSS(2)
		RANGE_CROSS(3)
		RANGE_CROSS(4)
		RANGE_CROSS(5)
		RANGE_CROSS(6)
		RANGE_CROSS(7)
		RANGE_CROSS(8)
		RANGE_CROSS(9)
		RANGE_CROSS(10)
		RANGE_CROSS(11)

		RANGE_LONG(0)
		RANGE_LONG(1)
		RANGE_LONG(2)
		RANGE_LONG(3)

#undef RANGE_CROSS
#undef RANGE_LONG

	const wchar_t *SyncGainData[] ={L"10", L"5", L"2", L"1", L"0.5", L"0.2"};

#define COMBO_ITEMS(TYPE)\
	template<>struct FillComboboxList<TYPE>			 \
	{															 \
		void operator()(HWND h, TYPE &t)			 \
		{														 \
			for(int i = 0; i < dimention_of(SyncGainData); ++i)	 \
			{													 \
				ComboBox_AddString(h, SyncGainData[i]);			 \
			}													 \
		}														 \
	};															 \
	template<>struct CurrentValue<TYPE>				 \
	{															 \
		void operator()(HWND h, TYPE &t)			 \
		{														 \
			ComboBox_SetCurSel(h, Singleton<L502RangeTable>::Instance().items.get<TYPE>().value);\
		}\
	};\
	template<>struct DlgSubItems<TYPE, int>: ComboBoxSubItem<TYPE>{};\
	template<class P>struct __ok_btn__<DlgItem<TYPE>, P>\
    {  \
		typedef DlgItem<TYPE> O; \
		void operator()(O *o, P *p)	 \
		{						   \
			wchar_t s[128];					\
			   GetWindowText(o->hWnd, s, dimention_of(s));	\
			int i = 0;											  \
			for(; i < dimention_of(SyncGainData); ++i)	\
			{																		  \
				if(0 == wcscmp(s, SyncGainData[i])) break;	  \
			}												 \
			   o->value.value =  i;		   \
			p->update.set<typename TL::Inner<O>::Result>(o->value.value);  \
		}	 \
	};

COMBO_ITEMS(RangeRef)
COMBO_ITEMS(RangeInp)
#undef COMBO_ITEMS

#define COMBO_ITEMS(TYPE, N)\
	template<>struct FillComboboxList<Range<TYPE, N>>			 \
	{															 \
		void operator()(HWND h, Range<TYPE, N> &)			 \
		{														 \
			for(int i = 0; i < dimention_of(SyncGainData); ++i)	 \
			{													 \
				ComboBox_AddString(h, SyncGainData[i]);			 \
			}													 \
		}														 \
	};	\
	template<>struct CurrentValue<Range<TYPE, N>>				 \
	{															 \
		void operator()(HWND h, Range<TYPE, N> &)			 \
		{														 \
			const int val = Singleton<L502RangeTable>::Instance().items.get<Range<TYPE, N>>().value;\
			ComboBox_SetCurSel(h, val);\
		}\
	};\
	template<>struct DlgSubItems<Range<TYPE, N>, int>: ComboBoxSubItem<Range<TYPE, N>>{};\
	template<class P>struct __ok_btn__<DlgItem<Range<TYPE, N>>, P>\
    {  \
		typedef DlgItem<Range<TYPE, N>> O; \
		void operator()(O *o, P *p)	 \
		{						   \
			wchar_t s[128];					\
			GetWindowText(o->hWnd, s, dimention_of(s));	\
			int i = 0;											  \
			for(; i < dimention_of(SyncGainData); ++i)	\
			{																		  \
				if(0 == wcscmp(s, SyncGainData[i])) break;	  \
			}												 \
			   o->value.value =  i;		   \
			p->update.set<typename TL::Inner<O>::Result>(o->value.value);  \
		}	 \
	};

COMBO_ITEMS(Cross, 0)
COMBO_ITEMS(Cross, 1)
COMBO_ITEMS(Cross, 2)
COMBO_ITEMS(Cross, 3)
COMBO_ITEMS(Cross, 4)
COMBO_ITEMS(Cross, 5)
COMBO_ITEMS(Cross, 6)
COMBO_ITEMS(Cross, 7)
COMBO_ITEMS(Cross, 8)
COMBO_ITEMS(Cross, 9)
COMBO_ITEMS(Cross, 10)
COMBO_ITEMS(Cross, 11)

COMBO_ITEMS(Long, 0)
COMBO_ITEMS(Long, 1)
COMBO_ITEMS(Long, 2)
COMBO_ITEMS(Long, 3)
#undef COMBO_ITEMS
#endif
}

void AmplificationChannelDlg::Do(HWND h)
{
	//if(TemplDialog<ParametersBase
	//	, L502RangeTable, DlgItem
	//>(Singleton<L502RangeTable>::Instance()).Do(h, L"Усиление каналов"))
	//{
	//}
}