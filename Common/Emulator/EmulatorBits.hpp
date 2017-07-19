#pragma once

template<class List, class T>struct InList;
template<class Head, class Tail, class T>struct InList<Tlst<Head, Tail>, T>
{
	static const bool value = InList<Tail, T>::value;
};
template<class Tail, class T>struct InList<Tlst<T, Tail>, T>
{
	static const bool value = true;
};
template<class T>struct InList<NullType, T>
{
	static const bool value = false;
};

template<class>struct On;
template<class>struct Off;

template<class Table, class T>struct __bit__;
template<class Table, class T>struct __bit__<Table, On<T>>
{
	void operator()(unsigned &b)
	{
		b |= Singleton<Table>::Instance().items.get<T>().value;
	}		
};
template<class Table, class T>struct __bit__<Table, Off<T>>
{
	void operator()(unsigned &b)
	{
		b &= ~Singleton<Table>::Instance().items.get<T>().value;
	}		
};

template<class T>struct __bits_data__
{
	typedef T Param;
	unsigned *value;
	__bits_data__(unsigned *value): value(value){}
};
typedef TL::MkTlst<InputBit1Table, InputBit2Table>::Result __tables_list__;

template<class O, class Param, bool>struct __bits1__
{
	bool operator()(unsigned *val)
	{
		__bit__<O, Param>()(val[TL::IndexOf<__tables_list__, O>::value]);
		return false;
	}
};
template<class O, class Param>struct __bits1__<O, Param, false>
{
	bool operator()(unsigned *val)
	{
		return true;
	}
};

template<class O, class P>struct __bits0__
{
	bool operator()(P &p)
	{
		typedef typename TL::Inner<P>::Result Param;
		return __bits1__<O, Param, InList<typename O::items_list, typename TL::Inner<Param>::Result>::value>()(p.value);
	}
};

template<class O, class P>struct __bits__
{
	void operator()(P *p)
	{
		TL::find<__tables_list__, __bits0__>()(__bits_data__<O>(p));
	}
};

