#include "stdafx.h"
#include "SolidBase.h"
//#include "ComputeSolidGroup.h"
#include <typeinfo>
#include "tools_debug/DebugMess.h"
#include "Base\tables.hpp"
//-----------------------------------------------------------------------
namespace
{
template<class O, class P>struct row_table
{
	void operator()(P *p)
	{
		O &o = Singleton<O>::Instance();
		Select<O>(*p).ID(1).Execute(o);
	}
};

template<int N>void DPrint(Holder<N> &d){dprint("%S", (wchar_t *)d);}
	template<class O, class P>struct __default_param__
	{
		void operator()(O *o, P *){}
	};	
	template<class X, class P>struct __default_param__<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			X &x = Singleton<X>::Instance();
		    Select<X>(*base).ID(o->value).Execute(x);
			TL::foreach<typename X::items_list, __default_param__XXX>()(&x.items, base);
		}
	};
	template<class O, class P>struct __default_param__XXX
	{
		void operator()(O *o, P *){}
	};	
	template<class X, class P>struct __default_param__XXX<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			X &x = Singleton<X>::Instance();
		    Select<X>(*base).ID(o->value).Execute(x);
			TL::foreach<typename X::items_list, __default_param__>()(&x.items, base);
		}
	};
}
//-----------------------------------------------
//void SolidBase::InitTypeSizeTables()
//{
//	//TODO отладить
//	///вставить солид базу
//	//CurrentParametersTable x;
//	//Select<CurrentParametersTable>(*this).ID(1).Execute(x);
//	//ParametersTable	&p = Singleton<ParametersTable>::Instance();
//	//Select<ParametersTable>(*this).ID(x.items.get<CurrentID>().value).Execute(p);
//	//TL::foreach<typename ParametersTable::items_list, __default_param__>()(&p.items, this);
//}
//
//void SolidBase::Init()
//{
//	ParametersBase parameters;
//	CBase base(
//		parameters.name()
//		, CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> >()
//		, parameters.tables
//		);
//	if(base.IsOpen())
//	{
//		zprint("\n");
//		TL::foreach<ParametersBase::one_row_table_list, row_table>()(&base);
//
//		InitTypeSizeTables();
//	}
//}
//------------------------------------------------------------------------
const wchar_t *SolidBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = wcslen(path);
	path[len - 4] = 0;
	wcscat(path, L"SolidBase");
	return path;
}
//---------------------------------------------------------------------



