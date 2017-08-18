#include "stdafx.h"
#include "Emulator/Emulator.h"
#include <Windows.h>
#include "PerformanceCounter\PerformanceCounter.h"
#include "templates\typelist.hpp"
#include "App/AppBase.h"
#include <math.h>
#include "App/Config.h"
#include "EmulatorBits.hpp"
namespace {
	wchar_t name[] = L"Parallel hatch in input data";
	wchar_t eventName[] = L"Event input data";

	HANDLE hMapFile;


	HANDLE h;
	struct EMapData
	{		
		unsigned start;
		unsigned count;
		unsigned bits0;
		unsigned bits1;
		unsigned outs0;
		unsigned outs1;
		bool isStart;
		double data[4 * 20 * 1024];
		char scan[100 * App::count_Thick_sensors * 474];
	};
	EMapData *map;
	Emulator &emulator = Singleton<Emulator>::Instance();
}

int start_time;


static const int dl = 25000;
static const int xl = 2000;

unsigned last0;
unsigned last1;
bool tst()
{
	if(map->bits0 != last0 || map->bits1 != last1)
	{
		last0 = map->bits0;
		last1 = map->bits1;
		return true;
	}
	return false;
}



Emulator::Emulator()
	: f(NULL)
	, start(false)
{
	h = CreateEvent(NULL, TRUE, FALSE, eventName);
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(EMapData),                // buffer size  
		name);                 // name of mapping object

	int res = GetLastError();
	map = (EMapData *) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		sizeof(EMapData));           


	//if(res == 0)
	{
		map->start = 0;
		map->isStart = false;
	}
}

Emulator::~Emulator()
{
}

void Emulator::Read(unsigned &start, double *data, unsigned &count, int delay)
{
	//int t = Performance::Counter();
	//while(map->count < count) Sleep(5);
	//memmove(data, map->data, sizeof(double) * count);
	//map->count = 0;
	//map->start = 0;
	//start = map->start % 18;
	for(unsigned i = 0; i < count; ++i)
	{
		short t = rand() % 10;
		data[i] = t & 1 ? t: -t;
	}
	start = 0;
	Sleep(2 * delay / 3);
}
namespace 
{
	const static int period = 3000;
	unsigned x = 0;
	double dY = 0;
}

#ifdef EMULATOR	
Emulator::Group::Group()
	: offs(0)
{
	f = fopen(".\\000014_73_D_170419084929.dat", "rb");
	fread(&size, sizeof(size), 1, f);
	fread(bufRef, sizeof(double), bufSize, f);
	fseek(f, size * sizeof(double) + sizeof(size), SEEK_SET);
	fread(bufSig, sizeof(double), bufSize, f);
}

Emulator::Group::~Group(){if(NULL != f)fclose(f);}

void Emulator::Group::Do(double &ref, double &sig)
{
	if(offs >= bufSize)
	{
		if(offsFile > size - bufSize) offsFile = 0;

		fseek(f, offsFile * sizeof(double) + sizeof(size), SEEK_SET);
		fread(bufRef, sizeof(double), bufSize, f);

		fseek(f, (size + offsFile) * sizeof(double) + sizeof(size), SEEK_SET);
		fread(bufSig, sizeof(double), bufSize, f);
		offsFile += bufSize;
		offs = 0;
	}
	ref = bufRef[offs];
	sig = bufSig[offs];
	++offs;
}
#else
Emulator::Group::Group(){}

Emulator::Group::~Group(){}

void Emulator::Group::Do(double &ref, double &sig){}
#endif

#define BITS(...) TL::foreach<TL::MkTlst<__VA_ARGS__>::Result, __bits__>()(&map->bits0);\
	if(!rp)break;\
	printf(#__VA_ARGS__##"\n");

#define XBITS(...) TL::foreach<TL::MkTlst<__VA_ARGS__>::Result, __bits__>()(&map->bits0);\
	printf(#__VA_ARGS__##"\n");


#define ZBITS(...) TL::foreach<TL::MkTlst<__VA_ARGS__>::Result, __bits__>()(&map->bits0);
	
int loop = 0;
int lastLoop = 0;
#define OUTS(...) !TL::find<TL::MkTlst<__VA_ARGS__>::Result, __out_bits__>()(&map->outs0)
void Emulator::Do()
{
	rp = false;
	while(map->isStart)
	{
		Sleep(2000);
		XBITS(On<iCycle>);
		XBITS(On<iPCH_B>, On<iPCH_RUN>);
		XBITS(On<iReadyR1>);
		XBITS(On<iReadyT>, On<iControlT>, Off<iResultT>);
		while(!rp) Sleep(50);
	//	{
			printf("Sleep\n");
			 while(OUTS(Off<oCooling>)) Sleep(50);
			 printf("Start\n");
			++loop;
		Sleep(3000);
		BITS(On<iSQ1po>);
		Sleep(600);
		BITS(On<iSQ2po>);
		Sleep(200);
		BITS(On<iSQ1t>);
		Sleep(600);
		BITS(On<iSQ2t>);
		Sleep(200);
		BITS(On<iSQ1pr>);
		Sleep(600);
		BITS(On<iSQ2pr>);
		Sleep(200);
		BITS(On<iSQ1DM>);
		Sleep(600);
		BITS(On<iSQ2DM>);

		Sleep(5000);

		BITS(Off<iSQ1po>);
		Sleep(600);
		BITS(Off<iSQ2po>);
		Sleep(200);
		BITS(Off<iSQ1t>);
		Sleep(600);
		BITS(Off<iSQ2t>);
		Sleep(200);
		BITS(Off<iSQ1pr>);
		Sleep(600);
		BITS(Off<iSQ2pr>);
		Sleep(200);
		BITS(Off<iSQ1DM>);
		Sleep(600);
		BITS(Off<iSQ2DM>);
		BITS(On<iResultT>)
//		}

		if(loop != lastLoop)
		{
				printf("Stop\n");
				lastLoop = loop;
		}
		Sleep(1000);
	}
}


int x0 = 0;
int x1 = 0;

bool power = true;

void Emulator::ReadDo()
{
	//map->outs0 = 1 << 5;
	while(true)//map->isStart)
	{
	   if(OUTS(On<oPowerSU>))
	   {
		  //.. if(power)
		   {
		   ZBITS(On<iPCH_B>, Off<iPCH_RUN>, Off<iPCH_A>);
		  
		 //  Sleep(2000);
		//   power = false;
		   }
	   }
	   else
	   {
		   power = true;
	   }

	   rp = OUTS(On<oRP>);

	   if(map->outs0 != x0 || map->outs1 != x1)
	   {
		  x0 = map->outs0;
		  x1 = map->outs1;
		  printf("out %x %x\n", x0, x1);
	   }

	   Sleep(5);
	}
	printf("OUT **************************************************************************************************************************************");
}

DWORD WINAPI Proc(PVOID x)
{
	((Emulator *)x)->Do();
	return 0;
}

DWORD WINAPI ProcOut(PVOID x)
{
	((Emulator *)x)->ReadDo();
	return 0;
}

void Emulator::Start()
{
	if(!map->isStart)
	{
		last0 = 0x0badc0de;
		last1 = 0x0badc0de;
		start_time = Performance::Counter();
		map->isStart = true;
		CloseHandle(CreateThread(NULL, 0, Proc, this, 0, NULL));
		CloseHandle(CreateThread(NULL, 0, ProcOut, this, 0, NULL));
	}  
}

void Emulator::Stop()
{
	map->isStart = false;
}

unsigned Emulator::Inputs(int x)
{
	return (0 == x) ? map->bits0: map->bits1;
}

void Emulator::Outputs(int x, unsigned o)
{
	if(0 == x)  map->outs0 = o;
	else map->outs1 = o;
}

unsigned Emulator::ReadOutputs(int x)
{
	return (0 == x) ? map->outs0: map->outs1;
}

//unsigned Emulator::Outputs(int x)
//{
//	return (0 == x) ? map->outs0: map->outs1;
//}

DWORD WINAPI LAN_DO(PVOID d)
{
	((Emulator *)d)->LanDo();
	return 0;
}

HANDLE hEv = CreateEvent(NULL, FALSE, FALSE, L"Emulator::LanDo()");

struct Head
{
	int offset, sensor;
};

void Emulator::LanDo()
{
	unsigned x = 0;
	int zzz = 0;
	int counter = 0;

	while(f)
	{
		int count = fread(map->scan, sizeof(map->scan), 1, f);
		if(count == 0)
		{
			double x[400];
			fread(x, sizeof(x), 1, f);
			fread(map->scan, sizeof(map->scan), 1, f);
		}
		else
		{
			//ZeroMemory(map->scan, sizeof(map->scan));
			for(int i = 0; i < sizeof(map->scan); i += 474)
			{
				Head *c = (Head *)&map->scan[i];
				c->offset = counter / 3;
				c->sensor = counter % 3;
				++counter;
			}
		}

		SetEvent(hEv);
		Sleep(100);
	}

}


void Emulator::StartLan()
{
	if(start) return;
	start = true;
	value = 0;
	f = _wfopen(L"D:\\Projects\\project\\chelabinsk\\tubes\\яно 114_11 141022 1200.dat", L"rb");
	if(f)
	{
		double x[400];
		fread(x, sizeof(x), 1, f);
		int count = fread(map->scan, sizeof(map->scan), 1, f);
		CloseHandle(CreateThread(NULL, 0, LAN_DO, this, 0, NULL));
		printf("  Start Lan count %d\n", count);
	}
}

void Emulator::StopLan()
{
	if(!start) return;
	start = false;
	if(f)
	{
		fclose(f);
		f = NULL;
	}
}

void Emulator::LanData(char *buf)
{
	memmove(buf, map->scan, sizeof(map->scan));
}