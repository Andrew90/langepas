#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"

/// \brief ��������� �������� � �������� �������  ����� ����� ������

namespace
{
#if 1
#define OFFS_CROSS(N)template<>struct ParamTitle<Offset<Cross, N> >{wchar_t *operator()(){\
	return L"�������� ����������� ������ "##L#N;}};
#define OFFS_LONG(N)template<>struct ParamTitle<Offset<Long, N> >{wchar_t *operator()(){\
	return L"�������� ����������� ������ "##L#N;}};

	typedef Offset<Voltage, 0> OffsVoltage;
	PARAM_TITLE(OffsVoltage, L"�������� ������ ���������� ��� ������ ���������")
	typedef	Offset<Current, 0> OffsCurrent;
	PARAM_TITLE(OffsCurrent, L"�������� ������ ���� ��� ������ ���������")

	typedef	Offset<MagneticField, 0> OffsetMagneticField;
	PARAM_TITLE(OffsetMagneticField, L"�������� ������ ������� ������� ���������� ����")

	typedef	Offset<Temperature, 0> OffsetTemperature1;
	PARAM_TITLE(OffsetTemperature1, L"�������� ������ ������� ����������� 1")

	typedef	Offset<Temperature, 1> OffsetTemperature2;
	PARAM_TITLE(OffsetTemperature2, L"�������� ������ ������� ����������� 2")
		
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
	>(Singleton<L502OffsetsTable>::Instance()).Do(h, L"������ �������"))
	{
	}
}

namespace
{
#define RANGE_CROSS(N)template<>struct ParamTitle<Range<Cross, N> >{wchar_t *operator()(){\
	return L"�������� ����������� ������ "##L#N;}};
#define RANGE_LONG(N)template<>struct ParamTitle<Range<Long, N> >{wchar_t *operator()(){\
	return L"�������� ����������� ������ "##L#N;}};

	typedef Range<Voltage, 0> RangeVoltage;
	PARAM_TITLE(RangeVoltage, L"���������� ��� ������ ���������")
	typedef	Range<Current, 0> RangeCurrent;
	PARAM_TITLE(RangeCurrent, L"��� ��� ������ ���������")

	typedef	Range<MagneticField, 0> RangeMagneticField;
	PARAM_TITLE(RangeMagneticField, L"������ ������� ���������� ����")

	typedef	Range<Temperature, 0> RangeTemperature1;
	PARAM_TITLE(RangeTemperature1, L"������ ����������� 1")

	typedef	Range<Temperature, 1> RangeTemperature2;
	PARAM_TITLE(RangeTemperature2, L"������ ����������� 2")

	
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
	>(Singleton<L502RangeTable>::Instance()).Do(h, L"�������� �������"))
	{
	}
}

namespace  
{

	#define MODE_CROSS(N)template<>struct ParamTitle<Mode502<Cross, N> >{wchar_t *operator()(){\
	return L"����� ����������� ����������� ������ "##L#N;}};
#define MODE_LONG(N)template<>struct ParamTitle<Mode502<Long, N> >{wchar_t *operator()(){\
	return L"����� ����������� ����������� ������ "##L#N;}};

	typedef Mode502<Voltage, 0> Mode502Voltage;
	PARAM_TITLE(Mode502Voltage, L"����� ����������� ��� ���������� ������ ���������")
	typedef	Mode502<Current, 0> Mode502Current;
	PARAM_TITLE(Mode502Current, L"����� ����������� ��� ���� ������ ���������")

	typedef	Mode502<MagneticField, 0> Mode502MagneticField;
	PARAM_TITLE(Mode502MagneticField, L"����� ����������� ��� ������� ������� ���������� ����")

	typedef	Mode502<Temperature, 0> Mode502Temperature1;
	PARAM_TITLE(Mode502Temperature1, L"����� ����������� ��� ������� ����������� 1")

	typedef	Mode502<Temperature, 1> Mode502Temperature2;
	PARAM_TITLE(Mode502Temperature2, L"����� ����������� ������� ����������� 2")

	
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

const wchar_t *modeGainData[] ={L"� ����� �����", L"����������������"};

#define COMBO_ITEMS(TYPE, N)\
	template<>struct FillComboboxList<Mode502<TYPE, N>>			 \
	{															 \
		void operator()(HWND h, Mode502<TYPE, N> &)			 \
		{														 \
			for(int i = 0; i < dimention_of(modeGainData); ++i)	 \
			{													 \
				ComboBox_AddString(h, modeGainData[i]);			 \
			}													 \
		}														 \
	};	\
	template<>struct CurrentValue<Mode502<TYPE, N>>				 \
	{															 \
		void operator()(HWND h, Mode502<TYPE, N> &)			 \
		{														 \
			const int val = Singleton<L502ModeTable>::Instance().items.get<Mode502<TYPE, N>>().value;\
			ComboBox_SetCurSel(h, val);\
		}\
	};\
	template<>struct DlgSubItems<Mode502<TYPE, N>, int>: ComboBoxSubItem<Mode502<TYPE, N>>{};\
	template<class P>struct __ok_btn__<DlgItem<Mode502<TYPE, N>>, P>\
    {  \
		typedef DlgItem<Mode502<TYPE, N>> O; \
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
	>(Singleton<L502ModeTable>::Instance()).Do(h, L"����� ����������� �������"))
	{
	}
}


//TODO �������� ��������� ���������� ������-������� ����� 502