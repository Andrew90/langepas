#pragma once

namespace Sync
{
	template<class List, class Sub, class tmp = NullType>struct GetSubList;
	template<template<class, int>class W, class T, int N, class Tail, class Sub, class tmp>struct GetSubList<Tlst<W<T, N>, Tail>, Sub, tmp>
	{
		typedef typename GetSubList<
			Tail
			, Sub
			, typename TL::Append<tmp, typename TL::_if<TL::ItemInList<Sub, T>::value, W<T, N>, NullType>::Result>::Result
		>::Result Result;
	};
	template<class Sub, class tmp>struct GetSubList<NullType, Sub, tmp>
	{
		typedef tmp Result;
	};
}

#define SYNC(list) Sync::GetSubList<list, TL::MkTlst<Cross, Long, Voltage, Current>::Result>::Result