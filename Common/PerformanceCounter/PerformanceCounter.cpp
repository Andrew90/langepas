#include "stdafx.h"
#include "PerformanceCounter.h"
#include <WTypes.h>
namespace Performance
{
	LARGE_INTEGER frequency;

	//unsigned t = 1000;

	void Init()
	{
		QueryPerformanceFrequency(&frequency);
	}
	unsigned Counter()
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return unsigned(1000.0 * t.QuadPart/frequency.QuadPart);
		//t += 200;
		//return t;
	}
}