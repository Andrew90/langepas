#pragma once

namespace Lan
{
	extern const size_t &bufSize;
	unsigned Init();
	void Destroy();
	unsigned SetParams();
	void Start();
	void Stop();
}