#include "stdafx.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "Dialogs.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "1730/Device1730.h"
#include "InputOutputDlg.h"

namespace
{
template<class O, class P>struct __set__
{
	typedef typename TL::Inner<O>::Result T;
	void operator()(O *o, P *p)
	{
		wchar_t buf[128];
		GetWindowText(o->hWnd, buf, dimention_of(buf));
		o->value.value = p->Set<O>(Wchar_to<typename T::type_value>()(buf));
	}
};
template<class List>struct __data__
{
	typedef List list;
	unsigned buf[TL::Length<List>::value];
	template<class T>unsigned Set(unsigned val)
	{
		buf[TL::IndexOf<list, T>::value] = val;
		return 1 << val;
	}
};
template<class O, class P>struct __compress_bits__
{
	void operator()(O *o, P *p)
	{
		unsigned t = p->get<O>().value;
		unsigned i = 0;
		while(t >>= 1) ++i;
		o->value = i;
	}
};
struct putsDlg_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			if(!TL::find<Owner::list, __test__>()(&owner.items, &h))return;
			__data__<Owner::list> d;
			TL::foreach<Owner::list, __set__>()(&owner.items, &d);
			for(int i = 0; i < dimention_of(d.buf) - 1; ++i)
			{
				unsigned value = d.buf[i];
				for(int j = i + 1; j < dimention_of(d.buf); ++j)
				{
					if(value == d.buf[j])
					{
						MessageBox(h, L"Несколько данных ссылаются на один бит", L"Ошибка!!!", MB_ICONERROR);
						return;
					}
				}
			}
			CBase base(Owner::Base().name());
			if(base.IsOpen())
			{
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, owner.table.items);
				UpdateWhere<Owner::Table>(owner.table, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};


struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			CBase base(Owner::Base().name());
			if(base.IsOpen())
			{
				Owner::Table t;
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, t.items);
				UpdateWhere<Owner::Table>(t, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};
//----------------------------------------------------------------------------
/*
MIN_EQUAL_VALUE(iСontrolСircuits, 0)
MIN_EQUAL_VALUE(iCycle          , 0)
MIN_EQUAL_VALUE(iReady          , 0)
MIN_EQUAL_VALUE(iControl        , 0)
MIN_EQUAL_VALUE(iBase           , 0)
MIN_EQUAL_VALUE(iReserve        , 0)

MAX_EQUAL_VALUE(iСontrolСircuits, 31)
MAX_EQUAL_VALUE(iCycle          , 31)
MAX_EQUAL_VALUE(iReady          , 31)
MAX_EQUAL_VALUE(iControl        , 31)
MAX_EQUAL_VALUE(iBase           , 31)
MAX_EQUAL_VALUE(iReserve        , 31)


MIN_EQUAL_VALUE(iCross0	 , 0)
MIN_EQUAL_VALUE(iCross1	 , 0)
MIN_EQUAL_VALUE(iLong0 	 , 0)
MIN_EQUAL_VALUE(iLong1 	 , 0)
MIN_EQUAL_VALUE(iThick0	 , 0)
MIN_EQUAL_VALUE(iThick1	 , 0)

MAX_EQUAL_VALUE(iCross0	 , 31)
MAX_EQUAL_VALUE(iCross1	 , 31)
MAX_EQUAL_VALUE(iLong0 	 , 31)
MAX_EQUAL_VALUE(iLong1 	 , 31)
MAX_EQUAL_VALUE(iThick0	 , 31)
MAX_EQUAL_VALUE(iThick1	 , 31)

MIN_EQUAL_VALUE(iSolid	 , 0)
MAX_EQUAL_VALUE(iSolid	 , 31)


MIN_EQUAL_VALUE(oWork      , 0)
MIN_EQUAL_VALUE(oToShiftThe, 0)
MIN_EQUAL_VALUE(oResult1   , 0)
MIN_EQUAL_VALUE(oResult2   , 0)
MIN_EQUAL_VALUE(oPowerBM   , 0)
MIN_EQUAL_VALUE(oReserve   , 0)

MAX_EQUAL_VALUE(oWork      , 31)
MAX_EQUAL_VALUE(oToShiftThe, 31)
MAX_EQUAL_VALUE(oResult1   , 31)
MAX_EQUAL_VALUE(oResult2   , 31)
MAX_EQUAL_VALUE(oPowerBM   , 31)
MAX_EQUAL_VALUE(oReserve   , 31)

template<>struct DlgSubItems<iСontrolСircuits, unsigned >: UpDownSubItem<iСontrolСircuits>{};
template<>struct DlgSubItems<iCycle          , unsigned >: UpDownSubItem<iCycle          >{};
template<>struct DlgSubItems<iReady          , unsigned >: UpDownSubItem<iReady          >{};
template<>struct DlgSubItems<iControl        , unsigned >: UpDownSubItem<iControl        >{};
template<>struct DlgSubItems<iBase           , unsigned >: UpDownSubItem<iBase           >{};
template<>struct DlgSubItems<iReserve        , unsigned >: UpDownSubItem<iReserve        >{};

template<>struct DlgSubItems<iCross0, unsigned >: UpDownSubItem<iCross0>{};
template<>struct DlgSubItems<iCross1, unsigned >: UpDownSubItem<iCross1>{};
template<>struct DlgSubItems<iLong0 , unsigned >: UpDownSubItem<iLong0 >{};
template<>struct DlgSubItems<iLong1 , unsigned >: UpDownSubItem<iLong1 >{};
template<>struct DlgSubItems<iThick0, unsigned >: UpDownSubItem<iThick0>{};
template<>struct DlgSubItems<iThick1, unsigned >: UpDownSubItem<iThick1>{};

template<>struct DlgSubItems<oWork      , unsigned >: UpDownSubItem<oWork      >{};
template<>struct DlgSubItems<oToShiftThe, unsigned >: UpDownSubItem<oToShiftThe>{};
template<>struct DlgSubItems<oResult1   , unsigned >: UpDownSubItem<oResult1   >{};
template<>struct DlgSubItems<oResult2   , unsigned >: UpDownSubItem<oResult2   >{};
template<>struct DlgSubItems<oPowerBM   , unsigned >: UpDownSubItem<oPowerBM   >{};
template<>struct DlgSubItems<oReserve   , unsigned >: UpDownSubItem<oReserve   >{};

template<>struct DlgSubItems<iSolid   , unsigned >: UpDownSubItem<iSolid   >{};
*/

#define PAR(n)\
MIN_EQUAL_VALUE(n	 , 0)\
MAX_EQUAL_VALUE(n	 , 31)\
template<>struct DlgSubItems<n, unsigned >: UpDownSubItem<n>{};

PAR(iSQ1pr   )
PAR(iSQ2pr   )
PAR(iRPpr    )
PAR(iOPpr    )
PAR(iSQ1po   )
PAR(iSQ2po   )
PAR(iRPpo    )
PAR(iOPpo    )
PAR(iSQ1t    )
PAR(iSQ2t    )
PAR(iRPt     )
PAR(iOPt     )
PAR(iReadyT  )
PAR(iControlT)
PAR(iResultT )
PAR(iReserve )

PAR(oPowerPCH)
PAR(oRL      )
PAR(oRM      )
PAR(oRH      )
PAR(oSTF     )
PAR(oPowerSU )
PAR(oMagnet  )
PAR(oCooling )
PAR(oReloc1  )
PAR(oRP      )
PAR(oOP      )
PAR(oDefect  )
PAR(oReloc2  )
PAR(oWorkPR  )
PAR(oWorkPO  )
PAR(oWorkT 	 )

PAR(iZU           )
PAR(iPCH_B        )
PAR(iPCH_RUN      )
PAR(iPCH_A        )
PAR(iCycle        )
PAR(iReadyR1      )
PAR(iDone         )
PAR(iReserve0     )
PAR(iEtalon       )
PAR(iWork_pnevmo  )
PAR(iRevers_pnevmo)
PAR(iError_pnevmo )
PAR(iReserve1     )
PAR(iReserve2     )
PAR(iSQ1DM        )
PAR(iSQ2DM        )
				
PAR(oPR_OP   	  )
PAR(oPR_RP   	  )
PAR(oPO_OP   	  )
PAR(oPO_RP   	  )
PAR(oT_OP    	  )
PAR(oT_RP    	  )
PAR(oReserve0	  )
PAR(oReserve1	  )
PAR(oT_Work  	  )
PAR(oT_Cycle 	  )
PAR(oT_Base  	  )
PAR(oT_reserv	  )
PAR(oReserve2	  )
PAR(oReserve3	  )
PAR(oReserve4	  )
PAR(oReserve5	  )



#undef PAR

DO_NOT_CHECK(NamePlate1730_1)
PARAM_TITLE(NamePlate1730_1, L"Дескриптор платы 1730  1")
DO_NOT_CHECK(NamePlate1730_2)
PARAM_TITLE(NamePlate1730_2, L"Дескриптор платы 1730  2")
}
//---------------------------   ----------------------------   ---------------------------
void Inputs1Dlg::Do(HWND h)
{
	InputBit1Table t;
	TL::foreach<InputBit1Table::items_list, __compress_bits__>()(&t.items, &Singleton<InputBit1Table>::Instance().items);
	if(TemplDialog<ParametersBase, InputBit1Table, DlgItem, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит входного порта платы 1"))
	{
	}
}
//-----------------------------------------------------------------------------
void Outputs1Dlg::Do(HWND h)
{
	OutputBit1Table t;
	TL::foreach<OutputBit1Table::items_list, __compress_bits__>()(&t.items, &Singleton<OutputBit1Table>::Instance().items);
	if(TemplDialog<ParametersBase, OutputBit1Table, DlgItem, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит выходного порта платы 1"))
	{
	}
}
//---------------------------   ----------------------------   ---------------------------
void Inputs2Dlg::Do(HWND h)
{
	InputBit2Table t;
	TL::foreach<InputBit2Table::items_list, __compress_bits__>()(&t.items, &Singleton<InputBit2Table>::Instance().items);
	if(TemplDialog<ParametersBase, InputBit2Table, DlgItem, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит входного порта платы 2"))
	{
	}
}
//-----------------------------------------------------------------------------
void Outputs2Dlg::Do(HWND h)
{
	OutputBit2Table t;
	TL::foreach<OutputBit2Table::items_list, __compress_bits__>()(&t.items, &Singleton<OutputBit2Table>::Instance().items);
	if(TemplDialog<ParametersBase, OutputBit2Table, DlgItem, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит выходного порта платы 2"))
	{
	}
}
//----------------------------------------------------------------------------
void Descriptor1730Dlg::Do(HWND h)
{
	NamePlate1730ParametersTable &t = Singleton<NamePlate1730ParametersTable>::Instance();
	if(TemplDialog<ParametersBase, NamePlate1730ParametersTable, DlgItem>(t).Do(h, L"Дескрипторы плат 1730"))
	{
	}
}

