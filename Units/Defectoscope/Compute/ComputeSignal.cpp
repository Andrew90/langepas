#include "stdafx.h"
#include "ComputeSignal.h"
#include "DataItem\DataItem.h"
#include "Lan\LanParameters.h"
#include "DataItem\ThicknessData.h"
#include "Windows\SignalViewer.h"
#include "FFT\FFT.h"
#include "Windows\SignalWindow.h"
#include "Windows\ThicknessWindow.h"

namespace
{
	struct __data__
	{
		int sensor;
		int offset;
		char *data;
	};
	template<class O, class P>struct __sensor__;	
	template<int N, class P>struct __sensor__<Line<ThickWindow, N>, P>
	{
		typedef Line<ThickWindow, N> O;
		bool operator()(O &o, P &p)
		{
			if(N == p.sensor)
			{
				p.data = o.dataViewer.firstScan[p.offset];
				return false;
			}
			return true;
		}
	};

	struct Head
	{
		int offset, sensor;
	};
}

void  ComputeSignal<SignalViewer>::operator()(SignalViewer &owner, int zone, int sensor, int offsetInZone)
{
	int packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;	
	HWND h = FindWindow(WindowClass<ThickWindow>()(), 0);
	if(NULL == h) return;
	ThickWindow &window = *(ThickWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	__data__ data = {sensor, offsetInZone, NULL};
	TL::find<ThickWindow::line_list, __sensor__>()(window.viewers, data);
	double *scan = owner.scan;
	for(int i = 0; i < packetSize; ++i)
	{
		scan[i] = data.data[i];
	}
	
	owner.tchart.minAxesY = -128;
	owner.tchart.maxAxesY = 127;
	owner.tchart.minAxesX = 0;
	owner.tchart.maxAxesX = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value - 1;

	owner.tchart.items.get<LineSeries>().SetData(scan, packetSize, 0, packetSize - 1);
}

namespace
{
	struct __cut_off__
	{
		int sensor, left, right;
	};
	template<class O, class P>struct __set_border__
	{
		void operator()(O &, P &){}
	};
	template<int N, class P>struct __set_border__<LeftBorderACFCutOff<N>, P>
	{
		typedef LeftBorderACFCutOff<N> O;
		void operator()(O &o, P &p)
		{
			if(N == p.sensor)
			{
				p.left = o.value;
			}
		}
	};
	template<int N, class P>struct __set_border__<RightBorderACFCutOff<N>, P>
	{
		typedef RightBorderACFCutOff<N> O;
		void operator()(O &o, P &p)
		{
			if(N == p.sensor)
			{
				p.right = o.value;
			}
		}
	};
}

void  ComputeSignal<ACFViewer>::operator()(ACFViewer &owner, int zone, int sensor, int offsetInZone)
{
	int packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;	
	HWND h = FindWindow(WindowClass<ThickWindow>()(), 0);
	if(NULL == h) return;
	ThickWindow &window = *(ThickWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	__data__ data = {sensor, offsetInZone, NULL};
	TL::find<ThickWindow::line_list, __sensor__>()(window.viewers, data);
	double (&scan)[1024] = owner.scan;
	for(int i = 0; i < packetSize; ++i)
	{
		scan[i] = data.data[i];
	}

	ZeroMemory(&scan[packetSize], sizeof(double) * (dimention_of(scan) - packetSize));

	owner.tchart.minAxesX = 0;
	unsigned n = packetSize;

	CFFT fft;
	fft.Init(packetSize);
	int k = 1;
	while(n >>= 1) k <<= 1;
	owner.tchart.maxAxesX = k - 1;

	fft.Direct(scan);
	fft.Spectrum(scan);

	double t = 0;

	for(int i = 0; i < k; ++i)
	{
	  if(scan[i] > t) t = scan[i];
	}

	owner.tchart.minAxesY = 0;
	owner.tchart.maxAxesY = t;
	
	owner.tchart.items.get<LineSeries>().SetData(scan, k, 0, k - 1);
}



void  ComputeSignal<FFTViewer>::operator()(FFTViewer &owner, int zone, int sensor, int offsetInZone)
{
	int packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;	
	HWND h = FindWindow(WindowClass<ThickWindow>()(), 0);
	if(NULL == h) return;
	ThickWindow &window = *(ThickWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	__data__ data = {sensor, offsetInZone, NULL};
	TL::find<ThickWindow::line_list, __sensor__>()(window.viewers, data);

	double (&scan)[1024] = owner.scan;
	for(int i = 0; i < packetSize; ++i)
	{
		scan[i] = data.data[i];
	}

	dprint("sensor %d  offset %d\n", ((Head *)data.data)->sensor,  ((Head *)data.data)->offset);

	ZeroMemory(&scan[packetSize], sizeof(double) * (dimention_of(scan) - packetSize));
	
	unsigned n = packetSize;

	CFFT fft;
	fft.Init(packetSize);
	int k = 1;
	while(n >>= 1) k <<= 1;

	fft.Direct(scan);
	fft.Spectrum(scan);

	ZeroMemory(scan, owner.owner.leftAcfCutOff * sizeof(double));
	ZeroMemory(scan + owner.owner.rightAcfCutOff, sizeof(scan) - owner.owner.rightAcfCutOff * sizeof(double));

	fft.Direct(scan);
	fft.Spectrum(scan);

	double t = 0;

	for(int i = 0; i < k; ++i)
	{
	  if(scan[i] > t) t = scan[i];
	}
	
	owner.tchart.minAxesY = 0;
	owner.tchart.maxAxesY = t;
	owner.tchart.minAxesX = 0;
	owner.tchart.maxAxesX = k - 1;
	owner.tchart.items.get<LineSeries>().SetData(scan, k, 0, k - 1);
}

