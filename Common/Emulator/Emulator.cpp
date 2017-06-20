#include "stdafx.h"
#include "Emulator/Emulator.h"
#include <Windows.h>
#include "PerformanceCounter\PerformanceCounter.h"
#include "templates\typelist.hpp"
#include "App/AppBase.h"
#include <math.h>
#include "App/Config.h"
namespace {
	wchar_t name[] = L"Parallel hatch in input data";
	wchar_t eventName[] = L"Event input data";

	HANDLE hMapFile;


	HANDLE h;
	struct EMapData
	{
		bool isStart;
		unsigned start;
		unsigned count;
		unsigned bits;
		unsigned outs;
		double data[4 * 20 * 1024];
		char scan[100 * App::count_Thick_sensors * 474];
	};
	EMapData *map;
	Emulator &emulator = Singleton<Emulator>::Instance();
}

int start_time;

unsigned &inControlCircuts = Singleton<InputBitTable>::Instance().items.get<iÑontrolÑircuits>().value;
unsigned &inCross0 = Singleton<InputBitTable>::Instance().items.get<iCross0>().value;
unsigned &inCross1 = Singleton<InputBitTable>::Instance().items.get<iCross1>().value;

unsigned &inLong0 = Singleton<InputBitTable>::Instance().items.get<iLong0>().value;
unsigned &inLong1 = Singleton<InputBitTable>::Instance().items.get<iLong1>().value;

unsigned &inThick0 = Singleton<InputBitTable>::Instance().items.get<iThick0>().value;
unsigned &inThick1 = Singleton<InputBitTable>::Instance().items.get<iThick1>().value;

unsigned &inSolid = Singleton<InputBitTable>::Instance().items.get<iSolid>().value;
unsigned &inReady= Singleton<InputBitTable>::Instance().items.get<iReady>().value;

static const int dl = 25000;
static const int xl = 2000;

unsigned last;
bool tst()
{
	if(map->bits != last)
	{
		last = map->bits;
		return true;
	}
	return false;
}

void Bits()
{
	unsigned t = Performance::Counter() - start_time;
	if(dl + 20000 < t)
	{
		map->bits &= ~inControlCircuts;
		map->isStart = false;
		printf("STOP %x time %d\n", map->bits, t);
		emulator.StopLan();
	}
	else if(dl + 17000 < t)
	{
		map->bits &= ~inSolid;
		if(tst())printf("Solid off %x time %d\n", map->bits, t);
	}
	else if(dl + 16000 < t)
	{
		map->bits &= ~(inThick1); 
		if(tst())printf("Thick1 off %x time %d\n", map->bits, t);

	}
	else if(dl + 15000 < t)
	{
		map->bits &= ~(inThick0);
		if(tst())printf("Thick0 off %x time %d\n", map->bits, t);
	}
	else if(dl + 13000 < t)
	{
		map->bits &= ~(inLong1); 
		if(tst())printf("Long1 off %x time %d\n", map->bits, t);
	}
	else if(dl + 12000 < t)
	{
		map->bits &= ~(inLong0);
		if(tst())printf("Long0 off %x time %d\n", map->bits, t);
	}
	else if(dl + 10000 < t)
	{
		map->bits &= ~(inCross1); 
		if(tst())printf("Cross1 off %x time %d\n", map->bits, t);
	}
	else if(dl + 9000 < t)
	{
		map->bits &= ~(inCross0);
		if(tst())printf("Cross0 off %x time %d\n", map->bits, t);
	}
	else if(xl + 9500 < t)
	{
		map->bits &= ~inReady;
		if(tst())printf("Ready off %x time %d\n", map->bits, t);
	}
	else if(xl + 9000 < t)
	{
		map->bits |= inSolid;
		if(tst())printf("Solid on %x time %d\n", map->bits, t);
	}
	else if(xl + 8000 < t)
	{
		map->bits |= (inThick1);
		if(tst())printf("Thick1 on %x time %d\n", map->bits, t);
	}
	else if(xl + 7000 < t)
	{
		map->bits |= (inThick0);
		if(tst())printf("Thick0 on %x time %d\n", map->bits, t);
		emulator.StartLan();
	}
	else if(xl + 5000 < t)
	{
		map->bits |= (inLong1); 
		if(tst())printf("Long1 on %x time %d\n", map->bits, t);
	}
	else if(xl + 4000 < t)
	{
		map->bits |= (inLong0);
		if(tst())printf("Long0 on %x time %d\n", map->bits, t);
	}
	else if(xl + 2000 < t)
	{
		map->bits |= (inCross1); 
		if(tst())printf("Cross1 on %x time %d\n", map->bits, t);
	}
	else if(xl + 1000 < t)
	{
		map->bits |= (inCross0);
		if(tst())printf("Cross0 on %x time %d\n", map->bits, t);
	}
	else if(xl + 10 < t)
	{
		map->bits |= (inReady);
		if(tst()) printf("inReady on %x time %d\n", map->bits, t);
	}
	else if(50 < t)
	{
		map->bits |= (inControlCircuts);
		if(tst()) printf("ControlCircuts on %x time %d\n", map->bits, t);
	}

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
	int t = Performance::Counter();
	while(map->count < count) Sleep(5);
	memmove(data, map->data, sizeof(double) * count);
	map->count = 0;
	map->start = 0;
	start = map->start % 18;
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


void Emulator::Do()
{
	int z = 0;
	bool b = false;
	int countSamples = 200;
	while(map->isStart)
	{
		Bits();

		int k = map->start;	
		for(int i = 0; i < 400 * 18; ++i)
		{
			int x = i % 18;
			if(x == 16)
			{
				group.Do(map->data[k], map->data[k + 1]);
			}
			else if(x == 17)
			{}
			else
			{
				if(!b)
				{
					b = 0 == (z % 30000);
					map->data[k] = 0;
				}
				else
				{
					if(b)
					{
						countSamples = rand() % 700 + 3;
					}
					b = 0 != (z % countSamples);
					map->data[k] = 100.0 * rand() / RAND_MAX;
				}					
			}

			++z;
			if(k < dimention_of(map->data)) ++k;
		}
		map->count = k;
		Sleep(5);
		if(0 == map->count)
		{
			map->start = 0;
		}
		else
		{
			if(k < dimention_of(map->data))	map->start = k;
		}
	}
}

DWORD WINAPI Proc(PVOID x)
{
	((Emulator *)x)->Do();
	return 0;
}

void Emulator::Start()
{
	if(!map->isStart)
	{
		last = 0x0badc0de;
		start_time = Performance::Counter();
		map->isStart = true;
		CloseHandle(CreateThread(NULL, 0, Proc, this, 0, NULL));
	}  
}

void Emulator::Stop()
{
	map->bits &= ~inControlCircuts;
	map->isStart = false;
}

unsigned Emulator::Inputs()
{
	return map->bits;
}

void Emulator::Outputs(unsigned o)
{
	map->outs = o;
}

unsigned Emulator::Outputs()
{
	return map->outs;
}

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
		//	unsigned t = GetTickCount();
		//	printf("%d %d %d\n", map->scan[0], t - x, ++zzz);
		//	x = t;
	}

}


void Emulator::StartLan()
{
	if(start) return;
	start = true;
	value = 0;
	f = _wfopen(L"D:\\Projects\\project\\chelabinsk\\tubes\\ÑÎÏ 114_11 141022 1200.dat", L"rb");
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