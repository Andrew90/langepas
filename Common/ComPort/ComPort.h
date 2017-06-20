#pragma once
#include <Windows.h>

class ComPort
{	
public:
	struct TObj{};
	typedef void(TObj::*TptrProc)(unsigned char(&)[1024], int);
protected:
   	TptrProc ptrProc;
	TObj *obj;
	HANDLE hCom, hThread, hEvent, hComEvent;
	OVERLAPPED inputOverlapped, outputOverlapped;
	static DWORD WINAPI Run(LPVOID);
	unsigned char input[1024], output[1024];
	void _SetReceiveHandler(TObj *, void(TObj::*)(unsigned char(&)[1024], int));
protected:
	void Do();
public:
	ComPort();
	~ComPort();
	bool Open(int numberComPort, int speed, int parity = NOPARITY, int stopBits = ONESTOPBIT);
	void Close();
	bool IsOpen();
	void Write(unsigned char *, int, void(*)(ComPort &, unsigned char(&)[1024], int));
	void Write(unsigned char *, int);
	template<class T>void SetReceiveHandler(T *o, void(T::*ptr)(unsigned char(&)[1024], int))
	{
		 obj = (TObj *)o;
		 ptrProc = (TptrProc )ptr;
	}
};
