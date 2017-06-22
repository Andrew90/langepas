#include "StdAfx.h"
#include "StatusMessages.h"
#include "MessageItems.h"
#include "TemplateMessages.hpp"
#include "templates\sort.hpp"

#define SKIP(item, ...)template<>struct TmplMess::ItemSkip<item>\
	{typedef TL::MkTlst<__VA_ARGS__>::Result Result;};

	SKIP(Undefined
		, DeathZone
		, Nominal			   
		, BorderKlass2<Thick> 
		, BorderKlass3<Thick> 
		, BorderDefect<Thick> 
		, BorderKlass2<Cross> 
		, BorderDefect<Cross> 
		, BorderKlass2<Long> 
		, BorderDefect<Long> 
		)

		SKIP(Nominal
		, DeathZone
		, BorderKlass3<Thick> 
		, BorderKlass2<Thick> 
		, BorderDefect<Thick> 
		, BorderKlass2<Cross> 
		, BorderDefect<Cross> 
		, BorderKlass2<Long> 
		, BorderDefect<Long> 
		)

		SKIP(BorderKlass2<Thick>
		, DeathZone
		, BorderKlass3<Thick> 
		, BorderDefect<Thick> 
		)
		SKIP(BorderKlass3<Thick>
		, DeathZone
		, BorderDefect<Thick> 
		)
		SKIP(BorderDefect<Thick>
		, DeathZone
		)

		SKIP(BorderKlass2<Cross>
		, DeathZone
		, BorderDefect<Cross> 
		)
		SKIP(BorderDefect<Cross>
		, DeathZone
		)

		SKIP(BorderKlass2<Long>
		, DeathZone
		, BorderDefect<Long> 
		)
		SKIP(BorderDefect<Long>
		, DeathZone
		)
#undef SKIP

namespace StatusMessages
{
	template<class T>struct Txt;
	template<>struct Txt<Undefined			  >{wchar_t *operator()(){return L"\"результат не определён\"";}};
	template<>struct Txt<DeathZone			  >{wchar_t *operator()(){return L"\"мёртвая зона\"";}};
	template<>struct Txt<Nominal			  >{wchar_t *operator()(){return L"\"норма\"";}};
	template<>struct Txt<BorderDefect<Thick>  >{wchar_t *operator()(){return L"\"дефект толщины\"";}};
	template<>struct Txt<BorderDefect<Cross>  >{wchar_t *operator()(){return L"\"поперечный дефект\"";}};
	template<>struct Txt<BorderDefect<Long>   >{wchar_t *operator()(){return L"\"продольный дефект\"";}};
	template<>struct Txt<BorderKlass3<Thick>  >{wchar_t *operator()(){return L"\"толщина класс 3\"";}};
	template<>struct Txt<BorderKlass2<Thick>  >{wchar_t *operator()(){return L"\"толщина класс 2\"";}};
	template<>struct Txt<BorderKlass2<Cross>  >{wchar_t *operator()(){return L"\"поперечный 2 класс\"";}};
	template<>struct Txt<BorderKlass2<Long>   >{wchar_t *operator()(){return L"\"продольный 2 класс\"";}};

	template<class T>struct Color;
/// \todo вставить цвета для текстового сообщения
	template<>struct Color<Undefined			>{const int value; Color():value(1){} };
	template<>struct Color<DeathZone			>{const int value; Color():value(2){} };
	template<>struct Color<Nominal				>{const int value; Color():value(3){} };
	template<>struct Color<BorderDefect<Thick>	>{const int value; Color():value(4){} };
	template<>struct Color<BorderDefect<Cross>	>{const int value; Color():value(5){} };
	template<>struct Color<BorderDefect<Long> 	>{const int value; Color():value(6){} };
	template<>struct Color<BorderKlass3<Thick>	>{const int value; Color():value(7){} };
	template<>struct Color<BorderKlass2<Thick>	>{const int value; Color():value(8){} };
	template<>struct Color<BorderKlass2<Cross>	>{const int value; Color():value(9){} };
	template<>struct Color<BorderKlass2<Long> 	>{const int value; Color():value(10){}};


	struct __arr_proc_data__
	{
		wchar_t *buf;
		int color;
	};

	template<class O, class P>struct __arr_proc__
	{
		void operator()(P &p)
		{
			wcscpy(p.buf, Txt<O>()());
			p.buf = &p.buf[wcslen(p.buf)];
		}
	};

	template<class T>struct ArrProc
	{
		static void Do(wchar_t *buf, int &color)
		{
			__arr_proc_data__ data = {buf};
			TL::foreach<T, __arr_proc__>()(data);
			color = Color<typename T::Head>().value;
		}
	};

	typedef TmplMess::GenList<status_list, 100>::Result lst;

	typedef void(*PtrMess)(wchar_t *, int &);
	struct IndBits
	{
		int id;
		int bits;
		bool operator >(IndBits &val){return bits > val.bits;}		
	};

	PtrMess ptrMess[TL::Length<lst>::value];
	IndBits bits[TL::Length<lst>::value];

	template<class List>struct __bits__;
	template<class Head, class Tail>struct __bits__<Tlst<Head, Tail>>
	{
		static const int value = (1 << TL::IndexOf<status_list, Head>::value) | __bits__<Tail>::value;
	};
	template<>struct __bits__<NullType>
	{
		static const int value = 0;
	};

	template<class List>struct InitArrayProc;
	template<class Head, class Tail>struct InitArrayProc<Tlst<Head, Tail>>: InitArrayProc<Tail>
	{
		InitArrayProc(): InitArrayProc<Tail>()
		{
			static const int i = TL::IndexOf<lst, Head>::value;
			ptrMess[i] = ArrProc<Head>::Do;
			IndBits t = {i, __bits__<status_list>::value};
			bits[i] = t;
		}
	};

	template<>struct InitArrayProc<NullType>{};

	InitArrayProc<lst> initArrayProc;

	
	struct IdBits
	{
		IdBits()
		{
			QuickSort(bits, dimention_of(bits));
		}
		unsigned operator()(int res)
		{
			unsigned mid = (dimention_of(bits) - 1) / 2;
			unsigned start = 0;
			unsigned stop = dimention_of(bits) - 1;
			IndBits *xxx = bits; 
			while(stop != start)
			{
				if(bits[mid].bits > res)
				{
					stop = mid;
					mid = (stop + start) / 2;
				}
				else if(bits[mid].bits < res)
				{
					start = mid;
					mid = (stop + start) / 2;
				}				
				if(bits[mid].bits == res)
				{
					break;
				}
				if(stop - start <= 1)
				{
					bool b = false;
					if(b =(bits[stop].bits == res)) mid = stop;
					else if(b = (bits[start].bits == res)) mid = start;
					break;
				}
			}
			return bits[mid].id;
		}
	};

	IdBits idBits;


	bool Message(int id, wchar_t *mess, int &color)
	{
		if(id < dimention_of(ptrMess))
		{
			ptrMess[id](mess, color);
			return true;
		}
		return false;
	}
}

