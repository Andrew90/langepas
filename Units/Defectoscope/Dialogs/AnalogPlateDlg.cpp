#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"

/// \brief Настройки смещения и усиления каналов  платы сбора данных

namespace
{
#if 1
#define OFFS_CROSS(N)template<>struct ParamTitle<Offset<Cross, N - 1> >{wchar_t *operator()(){\
	return L"смешение поперечного канала "##L#N;}};
#define OFFS_LONG(N)template<>struct ParamTitle<Offset<Long, N - 1> >{wchar_t *operator()(){\
	return L"смешение продольного канала "##L#N;}};

	typedef Offset<Voltage, 0> OffsVoltage;
	PARAM_TITLE(OffsVoltage, L"смещение канала напряжения для группы прочности")
		typedef	Offset<Current, 1> OffsCurrent;
	PARAM_TITLE(OffsCurrent, L"смещение канала тока для группы прочности")

		typedef	Offset<MagneticField, 0> OffsetMagneticField;
	PARAM_TITLE(OffsetMagneticField, L"смещение канала датчика наличия магнитного поля")

		typedef	Offset<Temperature, 0> OffsetTemperature1;
	PARAM_TITLE(OffsetTemperature1, L"смещение канала датчика температуры 1")

		typedef	Offset<Temperature, 1> OffsetTemperature2;
	PARAM_TITLE(OffsetTemperature2, L"смещение канала датчика температуры 2")

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
		OFFS_CROSS(12)

		OFFS_LONG(1)
		OFFS_LONG(2)
		OFFS_LONG(3)
		OFFS_LONG(4)

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
	>(Singleton<L502OffsetsTable>::Instance()).Do(h, L"Номера каналов"))
	{
	}
}

namespace
{
#define RANGE_CROSS(N)template<>struct ParamTitle<Range<Cross, N - 1> >{wchar_t *operator()(){\
	return L"усиление поперечного канала "##L#N;}};
#define RANGE_LONG(N)template<>struct ParamTitle<Range<Long, N - 1> >{wchar_t *operator()(){\
	return L"усиление продольного канала "##L#N;}};

	typedef Range<Voltage, 0> RangeVoltage;
	PARAM_TITLE(RangeVoltage, L"напряжение для группы прочности")
		typedef	Range<Current, 1> RangeCurrent;
	PARAM_TITLE(RangeCurrent, L"ток для группы прочности")

		typedef	Range<MagneticField, 0> RangeMagneticField;
	PARAM_TITLE(RangeMagneticField, L"датчик наличия магнитного поля")

		typedef	Range<Temperature, 0> RangeTemperature1;
	PARAM_TITLE(RangeTemperature1, L"датчик температуры 1")

		typedef	Range<Temperature, 1> RangeTemperature2;
	PARAM_TITLE(RangeTemperature2, L"датчик температуры 2")


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
		RANGE_CROSS(12)

		RANGE_LONG(1)
		RANGE_LONG(2)
		RANGE_LONG(3)
		RANGE_LONG(4)

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

	COMBO_ITEMS(RangeVoltage)
	COMBO_ITEMS(RangeCurrent)

	COMBO_ITEMS(RangeMagneticField)
	COMBO_ITEMS(RangeTemperature1)
	COMBO_ITEMS(RangeTemperature2)
#undef COMBO_ITEMS

#define COMBO_ITEMS(TYPE, N)\
	template<>struct FillComboboxList<Range<TYPE, N - 1>>			 \
	{															 \
	void operator()(HWND h, Range<TYPE, N - 1> &)			 \
	{														 \
	for(int i = 0; i < dimention_of(SyncGainData); ++i)	 \
	{													 \
	ComboBox_AddString(h, SyncGainData[i]);			 \
	}													 \
	}														 \
	};	\
	template<>struct CurrentValue<Range<TYPE, N - 1>>				 \
	{															 \
	void operator()(HWND h, Range<TYPE, N - 1> &)			 \
	{														 \
	const int val = Singleton<L502RangeTable>::Instance().items.get<Range<TYPE, N - 1>>().value;\
	ComboBox_SetCurSel(h, val);\
	}\
	};\
	template<>struct DlgSubItems<Range<TYPE, N - 1>, int>: ComboBoxSubItem<Range<TYPE, N - 1>>{};\
	template<class P>struct __ok_btn__<DlgItem<Range<TYPE, N - 1>>, P>\
	{  \
	typedef DlgItem<Range<TYPE, N - 1>> O; \
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
		COMBO_ITEMS(Cross, 12)

		COMBO_ITEMS(Long, 1)
		COMBO_ITEMS(Long, 2)
		COMBO_ITEMS(Long, 3)
		COMBO_ITEMS(Long, 4)
#undef COMBO_ITEMS
#endif
}

void AmplificationChannelDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase
		, L502RangeTable, DlgItem
	>(Singleton<L502RangeTable>::Instance()).Do(h, L"Усиление каналов"))
	{
	}
}

namespace  
{

#define MODE_CROSS(N)template<>struct ParamTitle<Mode502<Cross, N - 1> >{wchar_t *operator()(){\
	return L"режим подключения поперечного канала "##L#N;}};
#define MODE_LONG(N)template<>struct ParamTitle<Mode502<Long, N - 1> >{wchar_t *operator()(){\
	return L"режим подключения продольного канала "##L#N;}};

	typedef Mode502<Voltage, 0> Mode502Voltage;
	PARAM_TITLE(Mode502Voltage, L"режим подключения для напряжения группы прочности")
		typedef	Mode502<Current, 1> Mode502Current;
	PARAM_TITLE(Mode502Current, L"режим подключения для тока группы прочности")

		typedef	Mode502<MagneticField, 0> Mode502MagneticField;
	PARAM_TITLE(Mode502MagneticField, L"режим подключения для датчика наличия магнитного поля")

		typedef	Mode502<Temperature, 0> Mode502Temperature1;
	PARAM_TITLE(Mode502Temperature1, L"режим подключения для датчика температуры 1")

		typedef	Mode502<Temperature, 1> Mode502Temperature2;
	PARAM_TITLE(Mode502Temperature2, L"режим подключения датчика температуры 2")


		MODE_CROSS(1)
		MODE_CROSS(2)
		MODE_CROSS(3)
		MODE_CROSS(4)
		MODE_CROSS(5)
		MODE_CROSS(6)
		MODE_CROSS(7)
		MODE_CROSS(8)
		MODE_CROSS(9)
		MODE_CROSS(10)
		MODE_CROSS(11)
		MODE_CROSS(12)

		MODE_LONG(1)
		MODE_LONG(2)
		MODE_LONG(3)
		MODE_LONG(4)

#undef MODE_CROSS
#undef MODE_LONG

		const wchar_t *modeGainData[] ={L"с общей землёй", L"дифференциальный"};

#define COMBO_ITEMS(TYPE, N)\
	template<>struct FillComboboxList<Mode502<TYPE, N - 1>>			 \
	{															 \
	void operator()(HWND h, Mode502<TYPE, N - 1> &)			 \
	{														 \
	for(int i = 0; i < dimention_of(modeGainData); ++i)	 \
	{													 \
	ComboBox_AddString(h, modeGainData[i]);			 \
	}													 \
	}														 \
	};	\
	template<>struct CurrentValue<Mode502<TYPE, N - 1>>				 \
	{															 \
	void operator()(HWND h, Mode502<TYPE, N - 1> &)			 \
	{														 \
	const int val = Singleton<L502ModeTable>::Instance().items.get<Mode502<TYPE, N - 1>>().value;\
	ComboBox_SetCurSel(h, val);\
	}\
	};\
	template<>struct DlgSubItems<Mode502<TYPE, N - 1>, int>: ComboBoxSubItem<Mode502<TYPE, N - 1>>{};\
	template<class P>struct __ok_btn__<DlgItem<Mode502<TYPE, N - 1>>, P>\
	{  \
	typedef DlgItem<Mode502<TYPE, N - 1>> O; \
	void operator()(O *o, P *p)	 \
	{						   \
	wchar_t s[128];					\
	GetWindowText(o->hWnd, s, dimention_of(s));	\
	int i = 0;											  \
	for(; i < dimention_of(modeGainData); ++i)	\
	{																		  \
	if(0 == wcscmp(s, modeGainData[i])) break;	  \
	}												 \
	o->value.value =  i;		   \
	p->update.set<typename TL::Inner<O>::Result>(o->value.value);  \
	}	 \
	};

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
	COMBO_ITEMS(Cross, 12)

	COMBO_ITEMS(Long, 1)
	COMBO_ITEMS(Long, 2)
	COMBO_ITEMS(Long, 3)
	COMBO_ITEMS(Long, 4)
#undef COMBO_ITEMS


#define COMBO_ITEMS(TYPE)\
	template<>struct FillComboboxList<TYPE>			 \
	{															 \
	void operator()(HWND h, TYPE &t)			 \
	{														 \
	for(int i = 0; i < dimention_of(modeGainData); ++i)	 \
	{													 \
	ComboBox_AddString(h, modeGainData[i]);			 \
	}													 \
	}														 \
	};															 \
	template<>struct CurrentValue<TYPE>				 \
	{															 \
	void operator()(HWND h, TYPE &t)			 \
	{														 \
	ComboBox_SetCurSel(h, Singleton<L502ModeTable>::Instance().items.get<TYPE>().value);\
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
	for(; i < dimention_of(modeGainData); ++i)	\
	{																		  \
	if(0 == wcscmp(s, modeGainData[i])) break;	  \
	}												 \
	o->value.value =  i;		   \
	p->update.set<typename TL::Inner<O>::Result>(o->value.value);  \
	}	 \
	};
		COMBO_ITEMS(Mode502Voltage)
		COMBO_ITEMS(Mode502Current)

		COMBO_ITEMS(Mode502MagneticField)
		COMBO_ITEMS(Mode502Temperature1)
		COMBO_ITEMS(Mode502Temperature2)
#undef COMBO_ITEMS
}

void ModeChannelDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase
		, L502ModeTable, DlgItem
	>(Singleton<L502ModeTable>::Instance()).Do(h, L"Режим подключения каналов"))
	{
	}
}

namespace 
{
	PARAM_TITLE(Inp502<sinhro_s>, L"Вход синхросигнала 1(SINHRO_S)")
	PARAM_TITLE(Inp502<sinhro_d>, L"Вход синхросигнала 2(SINHRO_D)")
	PARAM_TITLE(Inp502<error_x> , L"Вход состояния размагничивания(РАБОТА)")
	PARAM_TITLE(Out502<start_x> , L"Включение размагничивания(START)")

	template<>struct DlgSubItems<Inp502<sinhro_s>, int>: UpDownSubItem<Inp502<sinhro_s>, 16>{};
	template<>struct DlgSubItems<Inp502<sinhro_d>, int>: UpDownSubItem<Inp502<sinhro_d>, 16>{};
	template<>struct DlgSubItems<Inp502<error_x> , int>: UpDownSubItem<Inp502<error_x> , 16>{};
	template<>struct DlgSubItems<Out502<start_x> , int>: UpDownSubItem<Out502<start_x> , 16>{};

	DO_NOT_CHECK(Inp502<sinhro_s>)
		DO_NOT_CHECK(Inp502<sinhro_d>)
		DO_NOT_CHECK(Inp502<error_x> )
		DO_NOT_CHECK(Out502<start_x> )
}

void Digital502Dlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase
		, L502OffsetsDigitTable, DlgItem
	>(Singleton<L502OffsetsDigitTable>::Instance()).Do(h, L"Дискретные каналы платы 502"))
	{
	}
}

//настройки дискретных портов L502


//TODO написать настройку дискретных входов-выходов платы 502