#pragma once
#include "templates\typelist.hpp"
#include "App\AppBase.h"
#include "PerformanceCounter\PerformanceCounter.h"
#include "tools_debug\DebugMess.h"

 namespace AutomatN
 {
	struct ExceptionExit{};
	struct ExceptionStop{};
	struct ExceptionTimeOut{};
	struct ExceptionContinue{};
	struct ExceptionRun{};
	struct Exception—ontrol—ircuitsOff{};
	struct Exception—ycleOff{};
	struct ExceptionAlarm{};

	template<class>struct On{};
	template<class>struct Off{};
	template<class>struct Inv{};
	template<class>struct Proc{};
	template<class>struct Once{};
	template<class T>struct Ex;



	template<class list>struct ArrEvents
	{
		template<class O, class P>struct loc
		{
			void operator()(P *p)
			{
				p[TL::IndexOf<list, O>::value] = Ex<O>().handle;
			}
		};

		template<class O, class P>struct ev
		{
			bool operator()(P *p)
			{
				if(TL::IndexOf<list, O>::value == *p) throw O();
				return true;
			}
		};
		template<class P>struct ev<ExceptionRun, P>
		{
			typedef ExceptionRun O;
			bool operator()(P *p)
			{
				return TL::IndexOf<list, O>::value != *p;
			}
		};
		template<class P>struct ev<ExceptionContinue, P>
		{
			typedef ExceptionContinue O;
			bool operator()(P *p)
			{
				return TL::IndexOf<list, O>::value != *p;
			}
		};
		HANDLE h[TL::Length<list>::value];
		ArrEvents()
		{
			TL::foreach<list, loc>()(h);
		}

		int Throw(unsigned t)
		{
			TL::find<list, ev>()(&t);
			return t;
		}
	};

	template<class List, template<class>class T>struct Filt;
	template<class Head, class Tail, template<class>class T>struct Filt<Tlst<Head, Tail>, T>
	{
		typedef typename Filt<Tail, T>::Result Result;
	};
	template<class Head, class Tail, template<class>class T>struct Filt<Tlst<T<Head>, Tail>, T>
	{
		typedef Tlst<Head, typename Filt<Tail, T>::Result> Result;
	};
	template<template<class>class T>struct Filt<NullType, T>
	{
		typedef NullType Result;
	};

	template<class O, class P>struct __bits_0__
	{
		void operator()(O *o, P *p)
		{
			*p |= o->value;
		}
	};

	template<class List, class TList>struct __filtr__;
	template<class Head, class Tail, class TList>struct __filtr__<Tlst<Head, Tail>, TList>
	{
		typedef typename TL::_if<
			TL::TypeInList<TList, Head>::value
			, Tlst<Head, typename __filtr__<Tail, TList>::Result>
			, typename __filtr__<Tail, TList>::Result
		>::Result Result;
	};

	template<class TList>struct __filtr__<NullType, TList>
	{
		typedef NullType Result;
	};

	template<class List, template<class, class>class b>struct __sel_bits__
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
			TL::foreach<List, b>()(o, p);
		}
	};
	template<template<class, class>class b>struct __sel_bits__<NullType, b>
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
		}
	};
	
	template<class List>struct SelectBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, InputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<InputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct SelectBits<NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class List>struct SelectOutBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, OutputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<OutputBitTable>::Instance().items, &bits);
		}
	};

	template<>struct SelectOutBits<NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class List>struct BitsOut
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, OutputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<OutputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct BitsOut<NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class O, class P>struct __default_do__
	{
		void operator()(P &p)
		{
			O::Do(p);
		}
	};

	template<class List>struct DefaultDo
	{
		void operator()(unsigned bits)
		{
			TL::foreach<List, __default_do__>()(bits);
		}
	};
	template<>struct DefaultDo<NullType>
	{
		void operator()(unsigned){}
	};

	template<class List>struct Test_OutBits
	{
		bool operator()()
		{
			unsigned bitOn = 0, bitOff = 0;
			SelectOutBits<typename Filt<List, On>::Result>()(bitOn);
			SelectOutBits<typename Filt<List, Off>::Result>()(bitOff);
			unsigned res = device1730.ReadOutput();
			return bitOn == (res & (bitOn | bitOff));
		}
	};

	template<class A, class B>struct __all_lists_not_empty__
	{
		static const bool value = true;
	};
	template<>struct __all_lists_not_empty__<NullType, NullType>
	{
		static const bool value = false;
	};

	template<class A>struct __list_not_empty__
	{
		static const bool value = true;
	};
	template<>struct __list_not_empty__<NullType>
	{
		static const bool value = false;
	};

	template<class List>struct AND_Bits
	{
		unsigned operator()(unsigned delay = (unsigned)-1)
		{
			if((unsigned)-1 != delay) delay += Performance::Counter();
			unsigned bitOn = 0, bitOff = 0, bitInv = 0;
			typedef typename Filt<List, On>::Result list_on;
			typedef typename Filt<List, Off>::Result list_off;
			typedef typename Filt<List, Proc>::Result list_proc;
			static const bool bitsNotEmpty = __all_lists_not_empty__<list_on, list_off>::value;
			SelectBits<list_on>()(bitOn);
			SelectBits<list_off>()(bitOff);
			SelectBits<typename Filt<List, Inv>::Result>()(bitInv);

			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExit>::Result exeption_list;
			ArrEvents<exeption_list> arrEvents;

			GUARD{
				dprint("return guard\n");
			};

			while(true)
			{
				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 5);
				unsigned res = 0;
				if(bitsNotEmpty || __list_not_empty__<list_proc>::value)res = device1730.Read();
				if(WAIT_TIMEOUT == ev)
				{
					if(bitsNotEmpty &&(bitOn || bitOff))
					{						
						unsigned t = res ^ bitInv;
						if(bitOn == (t & (bitOn | bitOff))) 
						{
								//OnceDo<typename Filt<List, Once>::Result>()();
								return res;
						}
					}					
					DefaultDo<list_proc>()(res);
					if(Performance::Counter() >= delay)
					{
							throw ExceptionTimeOut();
					}
				}
				else
				{
					return arrEvents.Throw(ev - WAIT_OBJECT_0);
				}
			}
		}
	};
	template<class List>struct OUT_Bits
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<typename Filt<List, On>::Result>()(bitOn);
			BitsOut<typename Filt<List, Off>::Result>()(bitOff);

			unsigned res = device1730.ReadOutput();

            res &= ~bitOff;
			res |= bitOn;

			device1730.Write(res);
		}
	};
	template<class List>struct SET_Bits
	{
		void operator()()
		{
			unsigned res = 0;
			BitsOut<typename Filt<List, On>::Result>()(res);

			device1730.Write(res);
		}
	};

	template<>struct Off<i—ontrol—ircuits>
	{
		static unsigned &bit;
		static void Do(unsigned bits)
		{
		   if(0 == (bits & bit)) throw Exception—ontrol—ircuitsOff();
		}
	};

	template<>struct Off<iCycle>
	{
		static unsigned &bit;
		static void Do(unsigned bits)
		{
			if(0 == (bits & bit)) throw Exception—ycleOff();
		}
	};

#define AND_BITS(...) AND_Bits<TL::MkTlst<__VA_ARGS__>::Result>()
#define OR_BITS(...) OR_Bits<TL::MkTlst<__VA_ARGS__>::Result>()

#define OUT_BITS(...) OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define SET_BITS(...) SET_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define TEST_OUT_BITS(...)Test_OutBits<TL::MkTlst<__VA_ARGS__>::Result>()()

 }