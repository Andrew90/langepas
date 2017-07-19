#pragma once

template<class T>struct Time
{
	unsigned value;
};

namespace AutomatN
{
	template<class T>struct Ex;
	template<class T>struct On{};

	struct ExceptionStop{};
	struct ExceptionExit{};
	struct ExceptionRun{};
	struct ExceptionAlarm
	{
		ExceptionAlarm();
	};
	struct ExceptionContinue{};

	template<>struct Ex<ExceptionStop>{static HANDLE handle;};
	template<>struct Ex<ExceptionExit>{static HANDLE handle;};
	template<>struct Ex<ExceptionContinue>{static HANDLE handle;};
	template<>struct Ex<ExceptionRun>{static HANDLE handle;};
}

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