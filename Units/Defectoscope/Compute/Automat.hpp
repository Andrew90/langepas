#pragma once
#include "templates\typelist.hpp"
#include "App\AppBase.h"
#include "PerformanceCounter\PerformanceCounter.h"
#include "tools_debug\DebugMess.h"
#include "ControlMode.h"
#include "Automat.h"
#include "Log\LogMessages.h"

namespace AutomatN
{
	struct ExceptionTimeOut{};
	struct Exception—ontrol—ircuitsOff{};
	struct Exception—ycleOff{};

	struct ExceptionReturnTube{};
	struct ExceptionExitTube{};

	template<class>struct Off{};
	template<class>struct Inv{};
	template<class>struct Proc{};

	extern void(*ptrProc)(Mode::Data &);

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
			bool operator()(P *p)
			{
				if(TL::IndexOf<list, ExceptionRun>::value == *p)
				{
					ptrProc = &Mode::ControlTube;
					return false;
				}
				return true;
			}
		};

		template<class P>struct ev<ExceptionReturnTube, P>
		{
			bool operator()(P *p)
			{
				if(TL::IndexOf<list, ExceptionReturnTube>::value == *p)
				{
					ptrProc = &Mode::ReturnTube;
					return false;
				}
				return true;
			}
		};

		template<class P>struct ev<ExceptionExitTube, P>
		{
			bool operator()(P *p)
			{
				if(TL::IndexOf<list, ExceptionExitTube>::value == *p)
				{
					ptrProc = &Mode::ExitTube;
					return false;
				}
				return true;
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

	template<class InputBitTable, class List>struct SelectBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, typename InputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<InputBitTable>::Instance().items, &bits);
		}
	};
	template<class InputBitTable>struct SelectBits<InputBitTable, NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class OutputsTable, class List>struct SelectOutBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, OutputsTable::items_list>::Result, __bits_0__>()
				(&Singleton<OutputsTable>::Instance().items, &bits);
		}
	};

	template<class OutputsTable>struct SelectOutBits<OutputsTable, NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class List, class Sub>struct BitsOut
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<Sub, List::items_list>::Result, __bits_0__>()
				(&Singleton<List>::Instance().items, &bits);
		}
	};
	template<class List>struct BitsOut<List, NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	struct __default_do_data__
	{
		unsigned val1, val2, delay;
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
		void operator()(unsigned bits0, unsigned bits1, unsigned delay)
		{
			__default_do_data__ data = {bits0, bits1, delay};
			TL::foreach<List, __default_do__>()(data);
		}
	};
	template<>struct DefaultDo<NullType>
	{
		void operator()(unsigned bits0, unsigned bits1, unsigned delay){}
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
	template<class List>struct SelectItem<List, NullType>
	{
		typedef NullType Result;
	};

	template<class List>struct TEST_Bits
	{
		bool operator()()
		{
			typedef typename SelectItem<InputBit1Table::items_list, List>::Result List1;
			typedef typename SelectItem<InputBit2Table::items_list, List>::Result List2;

			typedef typename Filt<List1, On>::Result list_on1;
			typedef typename Filt<List1, Off>::Result list_off1;

			typedef typename Filt<List2, On>::Result list_on2;
			typedef typename Filt<List2, Off>::Result list_off2;

			static const bool bitsNotEmpty1 = __all_lists_not_empty__<list_on1, list_off1>::value;
			static const bool bitsNotEmpty2 = __all_lists_not_empty__<list_on2, list_off2>::value;

			unsigned res1 = 0, res2 = 0, bitOn1 = 0, bitOn2 = 0, bitInv1 = 0, bitInv2 = 0, bitOff1 = 0, bitOff2 = 0;
			if(bitsNotEmpty1)res1 = device1730_1.Read();
			if(bitsNotEmpty2)res2 = device1730_2.Read();

			SelectBits<InputBit1Table, list_on1>()(bitOn1);
			SelectBits<InputBit1Table, list_off1>()(bitOff1);
			SelectBits<InputBit1Table, typename Filt<List1, Inv>::Result>()(bitInv1);

			SelectBits<InputBit2Table, list_on2>()(bitOn2);
			SelectBits<InputBit2Table, list_off2>()(bitOff2);
			SelectBits<InputBit2Table, typename Filt<List2, Inv>::Result>()(bitInv2);

			bool bits1 = false, bits2 = false;

			if(bitsNotEmpty1 &&(bitOn1 || bitOff1))
			{						
				unsigned t = res1 ^ bitInv1;
				bits1 = bitOn1 == (t & (bitOn1 | bitOff1));						
			}					

			if(bitsNotEmpty2 &&(bitOn2 || bitOff2))
			{						
				unsigned t = res2 ^ bitInv2;
				bits2 = bitOn2 == (t & (bitOn2 | bitOff2));
			}

			return (bits1 || !bitsNotEmpty1)&&(bits2 || !bitsNotEmpty2)&&(bitsNotEmpty1 || bitsNotEmpty2);
		}
	};

	template<class List>struct AND_Bits
	{
		int operator()(unsigned delay = (unsigned)-1)
		{
			if((unsigned)-1 != delay) delay += Performance::Counter();

			typedef typename SelectItem<InputBit1Table::items_list, List>::Result List1;
			typedef typename SelectItem<InputBit2Table::items_list, List>::Result List2;

			typedef typename Filt<List1, On>::Result list_on1;
			typedef typename Filt<List1, Off>::Result list_off1;

			typedef typename Filt<List2, On>::Result list_on2;
			typedef typename Filt<List2, Off>::Result list_off2;

			static const bool bitsNotEmpty1 = __all_lists_not_empty__<list_on1, list_off1>::value;
			static const bool bitsNotEmpty2 = __all_lists_not_empty__<list_on2, list_off2>::value;

			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExit>::Result exeption_list;
			ArrEvents<exeption_list> arrEvents;

			unsigned bitOn1 = 0, bitOff1 = 0, bitInv1 = 0;
			SelectBits<InputBit1Table, list_on1>()(bitOn1);
			SelectBits<InputBit1Table, list_off1>()(bitOff1);
			SelectBits<InputBit1Table, typename Filt<List1, Inv>::Result>()(bitInv1);

			unsigned bitOn2 = 0, bitOff2 = 0, bitInv2 = 0;
			SelectBits<InputBit2Table, list_on2>()(bitOn2);
			SelectBits<InputBit2Table, list_off2>()(bitOff2);
			SelectBits<InputBit2Table, typename Filt<List2, Inv>::Result>()(bitInv2);

			while(true)
			{
				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 20);

				unsigned res1 = device1730_1.Read();
				unsigned res2 = device1730_2.Read();

				bool bits1 = false, bits2 = false;
				if(WAIT_TIMEOUT == ev)
				{
					if(bitsNotEmpty1 && (bitOn1 || bitOff1))
					{	
						unsigned t = res1 ^ bitInv1;
						bits1 = bitOn1 == (t & (bitOn1 | bitOff1));						
					}
					if(bitsNotEmpty2 && (bitOn2 || bitOff2)	)
					{	
						unsigned t = res2 ^ bitInv2;
						bits2 = bitOn2 == (t & (bitOn2 | bitOff2));
					}
					if((bits1 || !bitsNotEmpty1)&&(bits2 || !bitsNotEmpty2)&&(bitsNotEmpty1 || bitsNotEmpty2))
					{
						return -1;
					}
					DefaultDo<typename Filt<List, Proc>::Result>()(res1, res2, delay);
					if(Performance::Counter() >= delay)
					{
						throw ExceptionTimeOut();
					}
				}
				else
				{
					arrEvents.Throw(ev - WAIT_OBJECT_0);
					return ev - WAIT_OBJECT_0;
				}
			}
		}
	};
	template<class List, class Sub>struct NoUsed_OUT_Bits_1
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<List, typename Filt<Sub, On>::Result>()(bitOn);
			BitsOut<List, typename Filt<Sub, Off>::Result>()(bitOff);

			if(0 != bitOff || 0 != bitOn)
			{
				unsigned res = device1730_1.ReadOutput();

				res &= ~bitOff;
				res |= bitOn;

				device1730_1.Write(res);
			}
		}
	};
	template<class Table>struct NoUsed_OUT_Bits_1<Table, NullType>{void operator()(){}};
	template<class Table, class List>struct NoUsed_OUT_Bits_2
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<Table, typename Filt<List, On>::Result>()(bitOn);
			BitsOut<Table, typename Filt<List, Off>::Result>()(bitOff);

			if(0 != bitOff || 0 != bitOn)
			{
				unsigned res = device1730_2.ReadOutput();

				res &= ~bitOff;
				res |= bitOn;

				device1730_2.Write(res);
			}
		}
	};

	template<class Table>struct NoUsed_OUT_Bits_2<Table, NullType>{void operator()(){}};

	template<class List>struct OUT_Bits
	{
		void operator()()
		{
			NoUsed_OUT_Bits_1<OutputBit1Table, SelectItem<OutputBit1Table::items_list, List>::Result>()();
			NoUsed_OUT_Bits_2<OutputBit2Table, SelectItem<OutputBit2Table::items_list, List>::Result>()();
		}
	};

	template<class List, class Sub>struct NoUsed_SET_Bits_1
	{
		void operator()()
		{
			unsigned res = 0;
			BitsOut<List, typename Filt<Sub, On>::Result>()(res);

			device1730_1.Write(res);
		}
	};
	template<class List>struct NoUsed_SET_Bits_1<List, NullType>
	{
		void operator()(){}
	};
	template<class List, class Sub>struct NoUsed_SET_Bits_2
	{
		void operator()()
		{
			unsigned res = 0;
			BitsOut<List, typename Filt<Sub, On>::Result>()(res);

			device1730_2.Write(res);
		}
	};
	template<class List>struct NoUsed_SET_Bits_2<List, NullType>
	{
		void operator()(){}
	};

	template<class List>struct SET_Bits
	{
		void operator()()
		{
			NoUsed_SET_Bits_1<OutputBit1Table, SelectItem<OutputBit1Table::items_list, List>::Result>()();
			NoUsed_SET_Bits_2<OutputBit2Table, SelectItem<OutputBit2Table::items_list, List>::Result>()();
		}
	};

#define AND_BITS(...) AND_Bits<TL::MkTlst<__VA_ARGS__>::Result>()
#define OR_BITS(...) OR_Bits<TL::MkTlst<__VA_ARGS__>::Result>()

#define OUT_BITS(...) OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define SET_BITS(...) SET_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define TEST_OUT_BITS(...)Test_OutBits<TL::MkTlst<__VA_ARGS__>::Result>()()

#define TEST_IN_BITS(...) TEST_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()


	template<const LogMess::ID N>struct ExceptionAl
	{
		template<class T>static void Do(T &t)
		{
			if(Performance::Counter() >= t.delay)
			{
				Log::Mess<N>();
				throw AutomatN::ExceptionAlarm();
			}
		}
	};

}