#pragma once
#include "templates\typelist.hpp"

template<class T>struct BorderKlass3;
	template<class T>struct BorderKlass2;
	template<class T>struct BorderDefect;

	struct Thick{};
	struct Cross{};
	struct Long {};

	struct Undefined{};
	struct DeathZone{};
	struct Nominal	{};

	//template<>struct BorderKlass3<Thick> {};
	//template<>struct BorderKlass2<Thick> {};
	//template<>struct BorderDefect<Thick> {};
	//
	//template<>struct BorderKlass2<Cross> {};
	//template<>struct BorderDefect<Cross> {};
	//
	//template<>struct BorderKlass2<Long> {};
	//template<>struct BorderDefect<Long> {};

typedef TL::MkTlst<
	Undefined
	, DeathZone
	, Nominal	

	, BorderDefect<Thick>
	, BorderDefect<Cross> 
	, BorderDefect<Long> 
	
	, BorderKlass3<Thick> 
	, BorderKlass2<Thick> 
	, BorderKlass2<Cross> 
	, BorderKlass2<Long> 
	
>::Result status_list;
