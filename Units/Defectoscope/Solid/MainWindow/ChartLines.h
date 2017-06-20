#pragma once
class L502Signal   : public ChartData
{
	friend Singleton<L502Signal>;
	L502Signal(): ChartData(){}
	L502Signal &operator =(const L502Signal &);
};
class L502Reference: public ChartData
{
	friend Singleton<L502Reference>;
	L502Reference(): ChartData(){}
	L502Reference &operator =(const L502Reference &);
};