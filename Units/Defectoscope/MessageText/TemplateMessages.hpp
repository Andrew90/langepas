#pragma once
#include "templates\typelist.hpp"

namespace TmplMess
{
	template<class List>struct pow;
	template<class Head, class Tail>struct pow<Tlst<Head, Tail>>
	{
		static const int value = 2 * pow<Tail>::value;
	};
	template<>struct pow<NullType>
	{
		static const int value = 1;
	};

	template<class List, class Sub = List>struct Bit;
	template<class List, class Head, class Tail>struct Bit<List, Tlst<Head, Tail>>
	{
		static const unsigned value = (1 << TL::IndexOf<List, Head>::value) | Bit<List, Tail>::value;
	};
	template<class List>struct Bit<List, NullType>
	{
		static const unsigned value = 0;
	};

	template<class List, class SubList, template<class>class SkipList>struct Skip;
	template<class List, class Head, class Tail, template<class>class SkipList>struct Skip<List, Tlst<Head, Tail>, SkipList>
	{
		static const unsigned value = Bit<List, SkipList<Head>::Result>::value | Skip<List, Tail, SkipList>::value;
	};
	template<class List, template<class>class SkipList>struct Skip<List, NullType, SkipList>
	{
		static const unsigned value = 0;
	};

	template<unsigned current, class List, class SubList, template<class>class ItemSkip>struct Ok
	{
		static const bool value = current == (current & ~Skip<List, SubList, ItemSkip>::value);
	};
	 template<class List, unsigned current, unsigned offs = 0, class tmp = NullType, bool = true>struct IntList
	{
		typedef typename TL::_if<0 != (current & (1 << offs)), typename TL::TypeAt<List, offs>::Result, NullType>::Result Item;
		typedef typename IntList<List, current, 1 + offs, typename TL::Append<tmp, Item>::Result, offs < TL::Length<List>::value>::Result Result;
	};
	template<class List, unsigned current, unsigned offs, class tmp>struct IntList<List, current, offs, tmp, false>
	{
		typedef tmp Result;
	};

	template<class List, template<class>class SkipList, unsigned max = pow<List>::value, unsigned current = 1, class tmp = NullType>struct XGenList;

	template<bool b, class List, template<class>class SkipList, unsigned max, unsigned current, class tmp>struct __XGenList__
	{
		typedef typename XGenList<List, SkipList, max, 1 + current, typename TL::Append<tmp, typename IntList<List, current>::Result>::Result>::Result Result;
	};
	template<class List, template<class>class SkipList, unsigned max, unsigned current, class tmp>struct __XGenList__<false, List, SkipList, max, current, tmp>
	{
		typedef typename XGenList<List, SkipList, max, 1 + current, NullType>::Result Result;
	};

	template<class List, template<class>class SkipList, unsigned max, unsigned current, class tmp>struct XGenList<List, SkipList, max, current, tmp>
	{
		typedef typename IntList<List, current>::Result intList;
		static const bool ok = Ok<current, List, intList, SkipList>::value;		
		typedef typename TL::_if<ok, intList, NullType>::Result Item;
		typedef typename XGenList<List, SkipList, max, 1 + current, typename TL::Append<tmp, Item>::Result>::Result Result;
		//typedef typename __XGenList__<ok, List, SkipList, max, current, tmp>::Result Result;
	};

	template<class List, template<class>class SkipList, unsigned max, class tmp>struct XGenList<List, SkipList, max, max, tmp>
	{
		typedef tmp Result;
	};

	template<class List, template<class>class SkipList>struct GenList
	{
		typedef typename TL::MultyListToList<typename TL::MkTlst<
		    typename XGenList<List, SkipList, 200, 1>::Result
			, typename XGenList<List, SkipList, 400, 201>::Result
			, typename XGenList<List, SkipList, 600, 401>::Result
			, typename XGenList<List, SkipList, 800, 601>::Result
			, typename XGenList<List, SkipList, pow<List>::value, 801>::Result
		>::Result>::Result Result;
	};
}