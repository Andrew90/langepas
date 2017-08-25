#include "StdAfx.h"
#include "StatusMessages.h"
#include "App/MessageItems.h"
#include "templates\sort.hpp"
#include "App\AppBase.h"
#include "MessageText\ListMess.hpp"
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

/// \todo вставить цвета для текстового сообщения
	ColorTable::TItems &ct = Singleton<ColorTable>::Instance().items;

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
		static void Do(wchar_t *buf, unsigned &color)
		{
			if(NULL != buf)
			{
				__arr_proc_data__ data = {buf};
				TL::foreach<T, __arr_proc__>()(data);
			}
			color = ct.get<Clr<typename T::Head>>().value;
		}
	};

	typedef void(*PtrMess)(wchar_t *, unsigned &);
	struct IndBits
	{
		int id;
		int bits;
		bool operator >(IndBits &val){return bits > val.bits;}		
	};

	PtrMess ptrMess[TL::Length<lst_lst::num_list>::value];
	IndBits bits[TL::Length<lst_lst::num_list>::value];

	template<class List>struct InitArrayProc;
	template<class Head, class Tail>struct InitArrayProc<Tlst<Head, Tail>>: InitArrayProc<Tail>
	{
		InitArrayProc(): InitArrayProc<Tail>()
		{
			static const int i = TL::IndexOf<lst_lst::num_list, Head>::value;
			ptrMess[i] = ArrProc<typename TL::TypeAt<lst_lst::lst_list, i>::Result>::Do;
			IndBits t = {i, Head::value};
			bits[i] = t;
		}
	};

	template<>struct InitArrayProc<NullType>{};

	InitArrayProc<lst_lst::num_list> initArrayProc;

	
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


	bool Message(int id, wchar_t *mess, unsigned &color)
	{
		mess[0] = 0;
		if(id < dimention_of(ptrMess))
		{
			ptrMess[id](mess, color);
			return true;
		}
		return false;
	}

	template<class List = status_list>struct BrakBits;
	template<class Head, class Tail>struct BrakBits<Tlst<Head, Tail>>
	{
		static const unsigned value = BrakBits<Tail>::value;
	};
	template<class Head, class Tail>struct BrakBits<Tlst<BorderDefect<Head>, Tail>>
	{
		static const unsigned value = (1 << TL::IndexOf<status_list, BorderDefect<Head>>::value) | BrakBits<Tail>::value;
	};
	template<>struct BrakBits<NullType>
	{
		static const unsigned value = 0;
	};

	static const int brak_bits = BrakBits<>::value;
}

template<class O, class P>struct __skip__
{
	void operator()(P &p)
	{
		static unsigned skipBit = 1 << TL::IndexOf<StatusMessages::lst_lst::skip_list, O>::value;
		if(0 != (skipBit & p))
		{
			if(0 != (p & O::value))p &=~skipBit;
		 //  p &=~ O::value;
		}
	}
};

int ResultMessageId(unsigned *x)
{
	unsigned res = 0;
	while(-1 != *x)
	{
		if(*x < dimention_of(StatusMessages::bits)) res |= StatusMessages::bits[*x].bits;
		x = &x[1];
	}
	TL::foreach<StatusMessages::lst_lst::skip_list, __skip__>()(res);
	return StatusMessages::idBits(res);
}

void ColorBar::operator()(double &data, unsigned &color, int id, double defData)
{
   if(id < dimention_of(StatusMessages::bits))StatusMessages::ptrMess[id](NULL, color);
   if(TL::IndexOf<StatusMessages::lst_lst::lst_list, TL::MkTlst<Undefined>::Result>::value == id 
	   || TL::IndexOf<StatusMessages::lst_lst::lst_list, TL::MkTlst<DeathZone>::Result>::value == id)
			data = defData;
}

void StatusText::FromSensors(unsigned *sens, unsigned &color, bool &visible, wchar_t *buf)
	{
		
		buf[0] = 0;
		unsigned id = ResultMessageId(sens);

		if(id < dimention_of(StatusMessages::ptrMess))
		{
			StatusMessages::ptrMess[id](buf, color);
		}
		
		 visible = !(TL::IndexOf<StatusMessages::lst_lst::lst_list, TL::MkTlst<Undefined>::Result>::value == id 
			 || TL::IndexOf<StatusMessages::lst_lst::lst_list, TL::MkTlst<DeathZone>::Result>::value == id);
	}

unsigned StatusColor::operator()(unsigned id)
	{
		 unsigned color = 0;
		if(id < dimention_of(StatusMessages::bits))StatusMessages::ptrMess[id](NULL, color);
		return color;
	}

void StatusText::operator()(int id, unsigned &color, bool &visible, wchar_t *buf)
	{
		buf[0] = 0;

		if(id < dimention_of(StatusMessages::ptrMess))
		{
			StatusMessages::ptrMess[id](buf, color);
		}

		  visible = !(TL::IndexOf<StatusMessages::lst_lst::lst_list, TL::MkTlst<Undefined>::Result>::value == id 
			 || TL::IndexOf<StatusMessages::lst_lst::lst_list, TL::MkTlst<DeathZone>::Result>::value == id);
	}


bool IsDefect(unsigned id)
{
	if(id < dimention_of(StatusMessages::bits))
	{
	   return 0 != (StatusMessages::brak_bits & StatusMessages::bits[id].bits);
	}
	return false;
}

namespace StatusMessages
{
	static const unsigned IsKlass3 = 1 << TL::IndexOf<status_list, BorderKlass3<Thick> >::value;

	template<class List>struct __klass__;
	template<class Head, class Tail>struct __klass__<Tlst<Head, Tail>>
	{
		static const unsigned value = __klass__<Tail>::value;
	};
	template<class T, class Tail>struct __klass__<Tlst<BorderKlass2<T>, Tail>>
	{
		static const unsigned value = (1 << TL::IndexOf<status_list, BorderKlass2<T>>::value) | __klass__<Tail>::value;
	};
	template<>struct __klass__<NullType>
	{
		static const unsigned value = 0;
	};

	static const unsigned IsKlass2 = __klass__<status_list>::value;
}

int IsKlass(unsigned id)
{
	if(id < dimention_of(StatusMessages::bits))
	{
	   unsigned res = StatusMessages::bits[id].bits;
	   if(StatusMessages::IsKlass3 & res) return 3;
	   if(StatusMessages::IsKlass2 & res) return 2;
	}
	return -1;
}





