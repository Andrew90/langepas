#pragma once
#include "templates\typelist.hpp"

template<class T>struct BorderKlass3;
	template<class T>struct BorderKlass2;
	template<class T>struct BorderDefect;

	struct Thick{};
	struct Cross{};
	struct Long {};
	struct Magn{};

	struct Undefined{};
	struct DeathZone{};
	struct Nominal	{};

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
