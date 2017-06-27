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
	//template<class>struct Once{};
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
		void operator()(unsigned bits0, unsigned bits1)
		{
			TL::foreach<List, __default_do__>()(bits0, bits1);
		}
	};
	template<>struct DefaultDo<NullType>
	{
		void operator()(unsigned bits0, unsigned bits1){}
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

	template<class List, class SubList>struct SelectItem;
	template<class List, class Head, class Tail>struct SelectItem<List, Tlst<Head, Tail>>
	{
		typedef typename SelectItem<List, Tail>::Result __next__;
		typedef typename TL::_if<
			TL::ItemInList<List, typename TL::Inner<Head>::Result>::value
			, Tlst<Head, __next__> 
			, __next__
		>::Result Result;
	};

	template<class List>struct AND_Bits
	{
		void operator()(unsigned delay = (unsigned)-1)
		{
			if((unsigned)-1 != delay) delay += Performance::Counter();
			unsigned bitOn1 = 0, bitOff1 = 0, bitInv1 = 0;
			unsigned bitOn2 = 0, bitOff2 = 0, bitInv2 = 0;

			typedef SelectItem<OutputBit1Table::items_list, List>::Result List1
			typedef SelectItem<OutputBit2Table::items_list, List>::Result List2

			typedef typename Filt<List1, On>::Result list_on1;
			typedef typename Filt<List1, Off>::Result list_off1;

			typedef typename Filt<List2, On>::Result list_on2;
			typedef typename Filt<List2, Off>::Result list_off2;

			typedef typename Filt<List, Proc>::Result list_proc;

			static const bool bitsNotEmpty1 = __all_lists_not_empty__<list_on1, list_off1>::value;
			static const bool bitsNotEmpty2 = __all_lists_not_empty__<list_on2, list_off2>::value;

			SelectBits<list_on1>()(bitOn1);
			SelectBits<list_off1>()(bitOff1);
			SelectBits<typename Filt<List1, Inv>::Result>()(bitInv1);

			SelectBits<list_on2>()(bitOn2);
			SelectBits<list_off2>()(bitOff2);
			SelectBits<typename Filt<List2, Inv>::Result>()(bitInv2);

			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExit>::Result exeption_list;
			ArrEvents<exeption_list> arrEvents;

			while(true)
			{
				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 5);
				unsigned res1 = 0, res2 = 0;
				if(__list_not_empty__<list_proc>::value)
				{
					if(bitsNotEmpty1)res1 = device1730_1.Read();
					if(bitsNotEmpty2)res2 = device1730_2.Read();
				}
				if(WAIT_TIMEOUT == ev)
				{
					if(bitsNotEmpty1 &&(bitOn1 || bitOff1))
					{						
						unsigned t = res1 ^ bitInv1;
						if(bitOn1 == (t & (bitOn1 | bitOff1))) 
						{
								return;
						}
					}					

					if(bitsNotEmpty2 &&(bitOn2 || bitOff2))
					{						
						unsigned t = res2 ^ bitInv2;
						if(bitOn2 == (t & (bitOn2 | bitOff2))) 
						{
								return;
						}
					}	

					DefaultDo<list_proc>()(res1, res2);
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
	template<class List>struct NoUsed_OUT_Bits_1
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<typename Filt<List, On>::Result>()(bitOn);
			BitsOut<typename Filt<List, Off>::Result>()(bitOff);

			unsigned res = device1730_1.ReadOutput();

            res &= ~bitOff;
			res |= bitOn;

			device1730_1.Write(res);
		}
	};
	template<>struct NoUsed_OUT_Bits_1<NullType>{void operator()(){}};
	template<class List>struct NoUsed_OUT_Bits_2
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<typename Filt<List, On>::Result>()(bitOn);
			BitsOut<typename Filt<List, Off>::Result>()(bitOff);

			unsigned res = device1730_2.ReadOutput();

            res &= ~bitOff;
			res |= bitOn;

			device1730_2.Write(res);
		}
	};

	template<>struct NoUsed_OUT_Bits_2<NullType>{void operator()(){}};

	template<class List>struct OUT_Bits
	{
		void operator()()
		{
			NoUsed_OUT_Bits_1<SelectItem<OutputBit1Table::items_list, List>::Result>()();
			NoUsed_OUT_Bits_2<SelectItem<OutputBit2Table::items_list, List>::Result>()();
		}
	};



	template<class List>struct NoUsed_SET_Bits_1
	{
		void operator()()
		{
			unsigned res = 0;
			BitsOut<typename Filt<List, On>::Result>()(res);

			device1730_1.Write(res);
		}
	};
	template<>struct NoUsed_SET_Bits_1<NullType>
	{
		void operator()(){}
	};
	template<class List>struct NoUsed_SET_Bits_2
	{
		void operator()()
		{
			unsigned res = 0;
			BitsOut<typename Filt<List, On>::Result>()(res);

			device1730_2.Write(res);
		}
	};
	template<>struct NoUsed_SET_Bits_2<NullType>
	{
		void operator()(){}
	};

	template<class List>struct SET_Bits
	{
		void operator()()
		{
			NoUsed_SET_Bits_1<SelectItem<OutputBit1Table::items_list, List>::Result>()();
			NoUsed_SET_Bits_2<SelectItem<OutputBit2Table::items_list, List>::Result>()();
		}
	};
	//template<>struct Off<i—ontrol—ircuits>
	//{
	//	static unsigned &bit;
	//	static void Do(unsigned bits)
	//	{
	//	   if(0 == (bits & bit)) throw Exception—ontrol—ircuitsOff();
	//	}
	//};
	//
	//template<>struct Off<iCycle>
	//{
	//	static unsigned &bit;
	//	static void Do(unsigned bits)
	//	{
	//		if(0 == (bits & bit)) throw Exception—ycleOff();
	//	}
	//};

#define AND_BITS(...) AND_Bits<TL::MkTlst<__VA_ARGS__>::Result>()
#define OR_BITS(...) OR_Bits<TL::MkTlst<__VA_ARGS__>::Result>()

#define OUT_BITS(...) OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define SET_BITS(...) SET_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define TEST_OUT_BITS(...)Test_OutBits<TL::MkTlst<__VA_ARGS__>::Result>()()

 }