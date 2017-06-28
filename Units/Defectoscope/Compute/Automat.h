#pragma once

template<class T>struct Time
{
	unsigned value;
};

class Automat
{
public:
	static void Init();
	static void Stop();
	static void Start();
	static void Continue();
    static void Exit();

	static void ReturnTube();
	static void ExitTube();
};