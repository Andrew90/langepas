#include "stdafx.h"
#include "L502.h"
#include "App/AppBase.h"
#include "App/Config.h"
#include "tools_debug\DebugMess.h"
#ifndef DEBUG_ITEMS
#include "L502/l502api.h"
#pragma comment(lib, "../../Common/L502/l502api.lib")
#include <stdio.h>
#include "App/AppBase.h"

L502::L502()
	: READ_TIMEOUT(100)
{}

bool L502::Init()
{
	hnd = L502_Create();
	if (hnd==NULL)
	{
		dprint("error module!");
	}
	else
	{
		int err = L502_Open((t_l502_hnd)hnd, NULL);
		if (err)
		{
			dprint("error connection with module: %s!", L502_GetErrorString(err));
			L502_Free((t_l502_hnd)hnd);
			hnd = NULL;
		}
	}
	return NULL != hnd;
}

void L502::Destroy()
{
	 /* закрываем связь с модулем */
        L502_Close((t_l502_hnd)hnd);
        /* освобождаем описатель */
        L502_Free((t_l502_hnd)hnd);
}

namespace
{
	template<class List>struct __set_array__
	{
		template<class O, class P>struct loc
		{
			void operator()(O *o, P *p)
			{
				p[TL::IndexOf<List, O>::value] = o->value;
			}
		};
	};
};

bool L502::SetupParams()
{
	static const int buf_size = TL::Length<L502RangeTable::items_list>::value;
	int f_channels[buf_size];
	TL::foreach<L502RangeTable::items_list, __set_array__<L502RangeTable::items_list>::loc>()(&Singleton<L502RangeTable>::Instance().items, f_channels);
	int f_ch_modes[buf_size];
	for(int i = 0; i < buf_size; ++i) f_ch_modes[i] = L502_LCH_MODE_COMM;
	int f_ch_ranges[buf_size];
	TL::foreach<L502OffsetsTable::items_list, __set_array__<L502OffsetsTable::items_list>::loc>()(&Singleton<L502OffsetsTable>::Instance().items, f_ch_ranges);

	int err = L502_SetLChannelCount((t_l502_hnd)hnd, buf_size);
	for (int i=0; (i < buf_size) && !err; i++)
        err = L502_SetLChannel((t_l502_hnd)hnd, i, f_channels[i], f_ch_modes[i], f_ch_ranges[i], 0);
	/* устанавливаем частоты ввода для АЦП и цифровых входов */
	int ADC_FREQ = Singleton<L502ParametersTable>::Instance().items.get<Frequency502>().value;
	double f_adc = ADC_FREQ;
	double f_frame = (double)ADC_FREQ/buf_size;
    if (!err)
    {
        err = L502_SetAdcFreq((t_l502_hnd)hnd, &f_adc, &f_frame);
    }

    /* записываем настройки в модуль */
    if (!err)
	{
		dprint("frequency adc = %0.0f frequency chenell = %0.0f", f_adc, f_frame);
        err = L502_Configure((t_l502_hnd)hnd, 0);
	}

    /* разрешаем синхронные потоки */
    if (!err)
    {
        err = L502_StreamsEnable((t_l502_hnd)hnd, L502_STREAM_ADC);
    }

    return 0 == err;
}
int L502::Start()
{
	int err = L502_StreamsStart((t_l502_hnd)hnd);
	if (err)
		dprint("error collections date: %s!", L502_GetErrorString(err));
	return err;
}

int L502::Stop()
{
	/* останавливаем поток сбора данных (независимо от того, была ли ошибка) */
	int err = L502_StreamsStop((t_l502_hnd)hnd);
	if (err)
	{
		dprint("error colecton date: %s", L502_GetErrorString(err));
	}
	return err;
}

int L502::Read(unsigned &startChannel, double *data, unsigned &count)
{
	unsigned rcv_buf[buffer_length];
	int cnt = L502_Recv((t_l502_hnd)hnd, rcv_buf, buffer_length, READ_TIMEOUT);
	if(cnt > 0)
	{
		L502_GetNextExpectedLchNum((t_l502_hnd)hnd, &startChannel);
		int err = L502_ProcessData((t_l502_hnd)hnd, rcv_buf, cnt, L502_PROC_FLAGS_VOLT,
			data, &count, NULL, NULL);
		if (err)
		{
			dprint("error computing date: %s", L502_GetErrorString(err));
		}
		return err;
	}
	return cnt;
}
bool L502::ReadAsync(unsigned ch, int range, double &value)
{
	/* устанавливаем 1 логический канал в управляющей таблице */
	int32_t err = X502_SetLChannelCount(hnd, 1);
	if (err == X502_ERR_OK) {
		/* логический канал соответствует измерению 7 канала
		в диф. режиме */
		err = X502_SetLChannel(hnd,0 ,ch,X502_LCH_MODE_COMM,range,0);
	}
	if (err == X502_ERR_OK) {
		/* передаем настройки в модуль */
		err = X502_Configure(hnd,0);
	}
	if (err == X502_ERR_OK) {
		/* Считываем кадр данных АЦП из одного отсчета */
		return X502_ERR_OK == X502_AsyncGetAdcFrame((t_l502_hnd)hnd, X502_PROC_FLAGS_VOLT, 1000, &value);
	}
	return false;
}
bool L502::BitOut(unsigned ch, bool value)
{
	unsigned bits = 1 << ch;
	return X502_ERR_OK == int32_t X502_AsyncOutDig((t_l502_hnd)hnd, value ? bits: 0, ~bits);
}
#else
#include "Emulator\Emulator.h"
L502::L502(){}
bool  L502::Init(){return true;}
void  L502::Destroy(){}
bool   L502::SetupParams(){return true;}
int   L502::Start(){return 0;}
int   L502::Stop(){return 0;}
int L502::Read(unsigned &startChennel, double *data, unsigned &count)
{
	Singleton<Emulator>::Instance().Read(startChennel, data, count, 50);
	return 1;
}
bool L502::ReadAsync(unsigned ch, int, double &value)
{
	value = ch;
	return true;
}

bool L502::BitOut(unsigned ch, bool value)
{
	return true;
}
#endif