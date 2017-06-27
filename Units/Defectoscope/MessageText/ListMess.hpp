#include "stdafx.h"
#include "templates/typelist.hpp"

template<class list, template<class>class Skip>class GenList
{
public:
	typedef typename TL::ListToWapperList<list, Skip>::Result skip_list;
private:
	template<class Offs, class List>struct BitsItem;
	template<class Offs, class Head, class Tail>struct BitsItem<Offs, Tlst<Head, Tail>>
	{
		static const unsigned value = (0 != (Offs::value & 1) ? (Head::value): 0)| BitsItem<TL::IntToType<(Offs::value >> 1)>, Tail>::value;
	};
	template<class List>struct BitsItem<TL::IntToType<0>, List>
	{
		static const unsigned value = 0;
	};
	template<class Head, class Tail>struct BitsItem<TL::IntToType<0>, Tlst<Head, Tail>>
	{
		static const unsigned value = 0;
	};

	template<unsigned val>struct Val
	{
		static const unsigned tmp = BitsItem<TL::IntToType<val>, skip_list>::value;
		static const int value = val == (val & ~tmp)? val: 0;
	};

	template<unsigned current, unsigned max, class tmp = NullType, bool = true>struct Gen
	{
		static const unsigned id = Val<current>::value;
		typedef typename TL::_if<0 != id, TL::IntToType<id>, NullType>::Result Item;
		typedef typename Gen<1 + current, max, typename TL::Append<tmp, Item>::Result, (current < max)>::Result Result;
	};
	template<unsigned current, unsigned max, class tmp>struct Gen<current, max, tmp, false>
	{
		typedef tmp Result;
	};

	template<class id, class List>struct num_lst;
	template<class id, class Head, class Tail>struct num_lst<id, Tlst<Head, Tail>>
	{
		typedef typename num_lst<TL::IntToType<(id::value >> 1)>, Tail>::Result Tmp;
		typedef typename TL::_if<
			id::value & 1
			, Tlst<Head, Tmp>
			, Tmp
		>::Result Result;
	};
	template<class List>struct num_lst<TL::IntToType<0>, List>
	{
		typedef NullType Result;
	};
	template<class Head, class Tail>struct num_lst<TL::IntToType<0>, Tlst<Head, Tail>>
	{
		typedef NullType Result;
	};
	template<class List>struct to_lst;
	template<class Head, class Tail>struct to_lst<Tlst<Head, Tail>>
	{
		typedef Tlst<typename num_lst<Head, list>::Result, typename to_lst<Tail>::Result> Result;
	};
	template<>struct to_lst<NullType>
	{
		typedef NullType Result;
	};

public:
	typedef typename TL::MultyListToList<typename TL::MkTlst<
		typename Gen<1, 300>::Result 
		, typename Gen<301, 600>::Result 
		, typename Gen<601, 900>::Result 
		, typename Gen<901, 1023>::Result
	>::Result>::Result num_list;

	typedef typename to_lst<num_list>::Result lst_list;
};

