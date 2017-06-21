#pragma once
#include "App/AppBase.h"
#include "tools_debug\DebugMess.h"
//-----------------------------------------------------------------
typedef TL::MkTlst<
/*0*/		Clr<Undefined   >
/*1*/		, Clr<DeathZone>
/*2*/		, Clr<Nominal	>
			, Clr<BorderKlass2<Thick> >
			, Clr<BorderKlass3<Thick> >
			, Clr<BorderDefect<Thick> >
/*5*/	    , Clr<BorderKlass2<Long> > 
/*6*/	    , Clr<BorderDefect<Long> >
/*7*/	    , Clr<BorderKlass2<Cross>>
/*8*/	    , Clr<BorderDefect<Cross>>

/*10*/      , Clr<Cancel<Projectionist>>

		, Clr<LessMinimumEnergy	  >
		, Clr<GreaterMaximumEnergy>
		, Clr<ValueLessThreshold  >

>::Result __first_color_list__;

typedef TL::MkTlst<	
	/*0*/		Clr<Undefined   >
	/*1*/		, Clr<DeathZone>
	/*2*/		, Clr<Nominal	>
	, Clr<BorderKlass2<Thick> >
			, Clr<BorderKlass3<Thick> >
			, Clr<BorderDefect<Thick> >
	/*5*/	    , Clr<BorderKlass2<Long> > 
	/*6*/	    , Clr<BorderDefect<Long> >
	/*7*/	    , Clr<BorderKlass2<Cross>>
	/*8*/	    , Clr<BorderDefect<Cross>>


	/*10*/  , Clr<Cancel<Projectionist>>

	, Clr<LessMinimumEnergy	  >
	, Clr<GreaterMaximumEnergy>
	, Clr<ValueLessThreshold  >


	///*11*/	, Clr< Nominal, Cancel<Projectionist>>

	

	/*21*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >

	/*31*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
	/*35*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
	/*39*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >

	/*45*/	, Clr<BorderKlass2<Long> , Cancel<Projectionist>> 
	/*46*/	, Clr<BorderDefect<Long> , Cancel<Projectionist>>
	/*47*/	, Clr<BorderKlass2<Cross>, Cancel<Projectionist>>
	/*48*/	, Clr<BorderDefect<Cross>, Cancel<Projectionist>>		

	/*55*/	, Clr<BorderDefect<Cross>, BorderDefect<Long>, Cancel<Projectionist>>

	/*65*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*69*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>

	/*73*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, Cancel<Projectionist>>




	, Clr<BorderDefect<Cross>, BorderDefect<Thick>>
, Clr<BorderDefect<Long>, BorderDefect<Thick>>
, Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderDefect<Thick>> 
, Clr<BorderDefect<Thick>, BorderKlass2<Cross>>
, Clr<BorderDefect<Thick>, BorderKlass2<Long>>
, Clr<BorderDefect<Thick>, BorderKlass2<Cross>, BorderKlass2<Long>> 
, Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderDefect<Thick>>    
, Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderDefect<Thick>>      
, Clr<BorderDefect<Thick>, Cancel<Projectionist> >
, Clr<BorderDefect<Cross>, BorderDefect<Thick>, Cancel<Projectionist>>
, Clr<BorderDefect<Long>, BorderDefect<Thick>, Cancel<Projectionist>>
, Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderDefect<Thick>, Cancel<Projectionist>> 
, Clr<BorderDefect<Thick>, BorderKlass2<Cross>, Cancel<Projectionist>>
, Clr<BorderDefect<Thick>, BorderKlass2<Long>, Cancel<Projectionist>>
, Clr<BorderDefect<Thick>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
, Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderDefect<Thick>, Cancel<Projectionist>>   
, Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderDefect<Thick>, Cancel<Projectionist>>  


, Clr<BorderDefect<Cross>, BorderKlass2<Thick>>
, Clr<BorderDefect<Long>, BorderKlass2<Thick>>
, Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderKlass2<Thick>> 
, Clr<BorderKlass2<Thick>, BorderKlass2<Cross>>
, Clr<BorderKlass2<Thick>, BorderKlass2<Long>>
, Clr<BorderKlass2<Thick>, BorderKlass2<Cross>, BorderKlass2<Long>> 
, Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderKlass2<Thick>>    
, Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderKlass2<Thick>>      
, Clr<BorderKlass2<Thick>, Cancel<Projectionist> >
, Clr<BorderDefect<Cross>, BorderKlass2<Thick>, Cancel<Projectionist>>
, Clr<BorderDefect<Long>, BorderKlass2<Thick>, Cancel<Projectionist>>
, Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderKlass2<Thick>, Cancel<Projectionist>> 
, Clr<BorderKlass2<Thick>, BorderKlass2<Cross>, Cancel<Projectionist>>
, Clr<BorderKlass2<Thick>, BorderKlass2<Long>, Cancel<Projectionist>>
, Clr<BorderKlass2<Thick>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
, Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderKlass2<Thick>, Cancel<Projectionist>> 
, Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderKlass2<Thick>, Cancel<Projectionist>> 


, Clr<BorderDefect<Cross>, BorderKlass3<Thick>>
, Clr<BorderDefect<Long>, BorderKlass3<Thick>>
, Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderKlass3<Thick>> 
, Clr<BorderKlass3<Thick>, BorderKlass2<Cross>>
, Clr<BorderKlass3<Thick>, BorderKlass2<Long>>
, Clr<BorderKlass3<Thick>, BorderKlass2<Cross>, BorderKlass2<Long>> 
, Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderKlass3<Thick>>    
, Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderKlass3<Thick>>      
, Clr<BorderKlass3<Thick>, Cancel<Projectionist> >
, Clr<BorderDefect<Cross>, BorderKlass3<Thick>, Cancel<Projectionist>>
, Clr<BorderDefect<Long>, BorderKlass3<Thick>, Cancel<Projectionist>>
, Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderKlass3<Thick>, Cancel<Projectionist>> 
, Clr<BorderKlass3<Thick>, BorderKlass2<Cross>, Cancel<Projectionist>>
, Clr<BorderKlass3<Thick>, BorderKlass2<Long>, Cancel<Projectionist>>
, Clr<BorderKlass3<Thick>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
, Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderKlass3<Thick>, Cancel<Projectionist>> 
, Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderKlass3<Thick>, Cancel<Projectionist>> 

	>::Result label_message_list;
//-------------------------------------------------------------------
template<class O>struct __status_label__
{
	typedef typename O::__template_must_be_overridded__ noused;
};

template<>struct __status_label__<NullType>
{
	static char *text(){return "NO_MESSAGE";}
};

#define STATUS_LABEL(O, txt)template<>struct __status_label__<O>\
{\
	static char *text(){return txt;};\
};

namespace
{
	struct __data_text__
	{
		int id;
		char *text;
		int color;
		bool visibleVal;
	};
	template<class T>struct ValueVisible
	{
		static const bool value = true;
	};
	template<>struct ValueVisible<Clr<Undefined>>
	{
		static const bool value = false;
	};
	template<>struct ValueVisible<Clr<DeathZone>>
	{
		static const bool value = false;
	};
	template<class T>struct __first_color__;

	template<class List>struct __xel__;
	template<class A, class B, class C, class D, class E, class F, class Tail>struct __xel__<Tlst<Clr<A, B, C, D, E, F>, Tail>>
	{
		typedef Tlst<Clr<A, B, C, D, E, F>, typename __xel__<Tail>::Result> Result;
	};
	template<class Head, class Tail>struct __xel__<Tlst<Head, Tail>>
	{
		typedef typename __xel__<Tail>::Result Result;
	};
	template<>struct __xel__<NullType>
	{
		typedef NullType Result;
	};

	template<class  _0, class _1, class _2, class _3, class _4,class _5>struct __first_color__<Clr<_0, _1, _2, _3, _4,_5>>
	{
		typedef Clr<_0, _1, _2, _3, _4, _5> O;
		typedef typename TL::_if<(TL::Length<typename __xel__<__first_color_list__>::Result>::value > TL::IndexOf<label_message_list, O>::value), O, Clr<_0>>::Result Result;
	};
	
	template<class O, class P>struct __select__
	{
		bool operator()(P *p)
		{
			if(TL::IndexOf<label_message_list, O>::value == p->id)
			{
				p->text = __status_label__<O>::text();
				p->color = Singleton<ColorTable>::Instance().items.get<typename __first_color__<O>::Result>().value;
				p->visibleVal = ValueVisible<O>::value;
				return false;
			}
			return true;
		}
	};
}

STATUS_LABEL(Clr<Undefined>, "\"результат не определён\"")
STATUS_LABEL(Clr<DeathZone>, "\"мёртвая зона\"")

STATUS_LABEL(Clr<Nominal>, "\"норма\"")

STATUS_LABEL(Clr<Cancel<Projectionist>>, "\"отмена\"")

STATUS_LABEL(Clr<LessMinimumEnergy	 >, "\"энергия меньше допустимой величины\"")
STATUS_LABEL(Clr<GreaterMaximumEnergy>, "\"энергия больше допустимой величины\"")
STATUS_LABEL(Clr<ValueLessThreshold  >, "\"энергия к первому пику\"")
//---------------------------------------------------------------------------
template<class O, class P>struct __set_color_bar__;

namespace
{
	struct __data_color__
	{
		int id;
		unsigned &color;
		double &data;
		double defData;
		__data_color__(int id, unsigned &color, double &data, double defData)
			: id(id)
		    , color(color)
		    , data(data)
			, defData(defData)
		{}
	};
}

template<class O, class P>struct __set_color_bar__
{
	int &color;
	__set_color_bar__() : color(Singleton<ColorTable>::Instance().items.get<O>().value){}
	bool operator()(P *p)
    {
		if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)
		{
		     p->color = color;
             return false;
		}
		return true;
    }
};

template<class T>struct GetFirst;
template<template<class, class, class, class, class,class>class X, class A, class B, class C, class D, class E, class F>struct GetFirst<X<A, B, C, D, E,F> >
{
	typedef X<A, NullType, NullType, NullType, NullType, NullType> Result;
};
template<class O, class P>struct __set_color_bar_next__
{
	bool operator()(P *p)
    {
		if(TL::IndexOf<label_message_list, O>::value == p->id)
		{
			p->color = Singleton<ColorTable>::Instance().items.get<typename GetFirst<O>::Result>().value;
             return false;
		}
		return true;
    }
};

#define COLOR_DATA(O)template<class P>struct __set_color_bar__<O, P>\
{\
	int &color;\
	__set_color_bar__() \
        : color(Singleton<ColorTable>::Instance().items.get<O>().value)\
        {}\
	bool operator()(P *p)\
    {\
        if(TL::IndexOf<label_message_list, O>::value == p->id)\
		{\
		     p->color = color;\
			 p->data = p->defData;\
             return false;\
		}\
		return true;\
    }\
};

COLOR_DATA(Clr<Undefined>)
COLOR_DATA(Clr<DeathZone>)

COLOR_DATA(Clr<LessMinimumEnergy   >)
COLOR_DATA(Clr<GreaterMaximumEnergy>)
COLOR_DATA(Clr<ValueLessThreshold  >)

#define TXT(a, b) a##_##b
#define STR(a, b) a<b>
#define PARAM_STR(a, b) STR(a, b)
#define PARAM_TXT(a, b) TXT(a, b)

#define STATUS_LABEL_1(a)template<>struct __status_label__<Clr<PARAM_STR##a>>\
{\
	static char *text(){return PARAM_TXT##a;}\
};

#define STATUS_LABEL_2(a, b)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b;}\
};

#define STATUS_LABEL_3(a, b, c)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c;};\
};

#define STATUS_LABEL_4(a, b, c, d)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d;}\
};

#define STATUS_LABEL_5(a, b, c, d, e)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d##PARAM_TXT##e;}\
};
#define STATUS_LABEL_6(a, b, c, d, e, f)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e, PARAM_STR##f>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d##PARAM_TXT##e##PARAM_TXT##f;}\
};

//-------------------------------
#define BorderDefect_Cross "\"поперечный дефект\""
#define BorderDefect_Long "\"продольный дефект\""

#define BorderKlass2_Cross "\"поперечный 2 класс\""
#define BorderKlass2_Long "\"продольный 2 класс\""


//#define BrakStrobe2_Thick "\"расслоение\""

#define Cancel_Projectionist "\"отмена\""

//template<>struct __status_label__<Clr<Nominal, Cancel<Projectionist>>>
//{
//	static char *text(){return "\"норма\"\"отмена\"";}
//};
//--------------------------------------------
STATUS_LABEL(Clr<BorderDefect<Thick>>, "\"дефект толщины\"")
#define BorderDefect_Thick "\"дефект толщины\""
STATUS_LABEL_2((BorderDefect, Cross), (BorderDefect, Thick))
STATUS_LABEL_2((BorderDefect, Long), (BorderDefect, Thick))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderDefect, Thick))
STATUS_LABEL_2((BorderDefect, Thick), (BorderKlass2, Cross))
STATUS_LABEL_2((BorderDefect, Thick), (BorderKlass2, Long))
STATUS_LABEL_3((BorderDefect, Thick), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderDefect, Thick))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderDefect, Thick))
STATUS_LABEL_2((BorderDefect , Thick), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Thick), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderDefect, Thick), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderDefect, Thick), (Cancel, Projectionist)) 
STATUS_LABEL_3((BorderDefect , Thick), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect , Thick), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect , Thick), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderDefect, Thick), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderDefect, Thick), (Cancel, Projectionist))  
//---------------------------------------------------------------------------------------------------
STATUS_LABEL(Clr<BorderKlass2<Thick>>, "\"толщина класс 2\"")
#define BorderKlass2_Thick "\"толщина класс 2\""
STATUS_LABEL_2((BorderDefect, Cross), (BorderKlass2, Thick))
STATUS_LABEL_2((BorderDefect, Long), (BorderKlass2, Thick))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderKlass2, Thick))
STATUS_LABEL_2((BorderKlass2, Thick), (BorderKlass2, Cross))
STATUS_LABEL_2((BorderKlass2, Thick), (BorderKlass2, Long))
STATUS_LABEL_3((BorderKlass2, Thick), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderKlass2, Thick))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderKlass2, Thick))
STATUS_LABEL_2((BorderKlass2 , Thick), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Thick), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Thick), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderKlass2, Thick), (Cancel, Projectionist)) 
STATUS_LABEL_3((BorderKlass2 , Thick), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BorderKlass2 , Thick), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderKlass2 , Thick), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderKlass2, Thick), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderKlass2, Thick), (Cancel, Projectionist)) 
//-----------------------------------------------------------------------
STATUS_LABEL(Clr<BorderKlass3<Thick>>, "\"толщина класс 3\"")
#define BorderKlass3_Thick "\"толщина класс 3\""
STATUS_LABEL_2((BorderDefect, Cross), (BorderKlass3, Thick))
STATUS_LABEL_2((BorderDefect, Long), (BorderKlass3, Thick))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderKlass3, Thick))
STATUS_LABEL_2((BorderKlass3, Thick), (BorderKlass2, Cross))
STATUS_LABEL_2((BorderKlass3, Thick), (BorderKlass2, Long))
STATUS_LABEL_3((BorderKlass3, Thick), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderKlass3, Thick))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderKlass3, Thick))
STATUS_LABEL_2((BorderKlass3 , Thick), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass3, Thick), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass3, Thick), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderKlass3, Thick), (Cancel, Projectionist)) 
STATUS_LABEL_3((BorderKlass3 , Thick), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BorderKlass3 , Thick), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderKlass3 , Thick), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderKlass3, Thick), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderKlass3, Thick), (Cancel, Projectionist))      
//-------------------------------------------------------------------------


STATUS_LABEL_1((BorderDefect, Cross))
STATUS_LABEL_1((BorderDefect, Long))

STATUS_LABEL_1((BorderKlass2, Cross))
STATUS_LABEL_1((BorderKlass2, Long))




STATUS_LABEL_2((BorderDefect, Cross), (BorderDefect, Long))

//---------------------------------------------------------------------------
				
			
STATUS_LABEL_2((BorderKlass2, Cross), (BorderKlass2, Long))
				
//--------------------
STATUS_LABEL_2((BorderDefect, Cross), (BorderKlass2, Long))
STATUS_LABEL_2((BorderDefect, Long), (BorderKlass2, Cross))												

STATUS_LABEL_2((BorderKlass2, Long), (Cancel, Projectionist)) 
STATUS_LABEL_2((BorderDefect, Long), (Cancel, Projectionist))
STATUS_LABEL_2((BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_2((BorderDefect, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (Cancel, Projectionist))
STATUS_LABEL_3((BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (Cancel, Projectionist))


#undef COLOR_DATA
#undef STATUS_LABEL
#undef STATUS_LABEL_2
#undef STATUS_LABEL_3
#undef STATUS_LABEL_4
#undef STATUS_LABEL_5
#undef STATUS_LABEL_6
#undef TXT
#undef STR
#undef PARAM_STR
#undef PARAM_TXT

#undef BorderDefect_Cross
#undef BorderDefect_Long 
#undef BorderKlass2_Cross 
#undef BorderKlass2_Long 
#undef BorderAbove_Thick
//------------------------------------------------------------------------------------------------



