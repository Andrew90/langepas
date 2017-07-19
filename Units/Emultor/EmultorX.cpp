// Emultor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
//#include <Windows.h>
//#include <locale.h>
#include "Base/Base.hpp"
#include "PerformanceCounter\PerformanceCounter.h"
#include "App/AppBase.h"
#include "Emulator\Emulator.h"



int _tmain(int argc, _TCHAR* argv[])
{
	Performance::Init();
	Initialize initialize;
	
	AppBase().Init();
	//
	Emulator &emulator = Singleton<Emulator>::Instance();
	
	while(char c = getch())
	{
		switch(c)
		{
		case VK_ESCAPE:
		//	printf("��� ����������� ������� ����� ������� ...");
		//	getchar();
			return 0;
		case 's':
			printf("�����\n");
			emulator.Start();
			break;
			case 't':
			printf("����\n");
			emulator.Stop();
			break;
		}
	}	
	getchar();
	return 0;
}

