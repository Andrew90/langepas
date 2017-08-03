#include "stdafx.h"
#include "SaveLoadDates.h"
#include <stdio.h>
#include "window_tool/DlgFiles.h"
#include "SolidData.h"
#include <ShlWapi.h>
#include <time.h>
#include "templates\templates.hpp"
#include "App\AppBase.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "Dates\CounterTubes.h"
#include "App/Config.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();
}

struct close_file
{
	FILE *f;
	close_file(FILE *f) : f(f){}
	~close_file(){if(NULL != f)fclose(f);}
};
#pragma warning(disable : 4996)

void SaveDateFile::Do(HWND h)
{
	SaveData o(h);
	bool b = false;
	if(solidData.start > 0 && o())
	{
		StoreDataFile(o.sFile);
	}
	if(!b)
	{
		MessageBox(0, L"���� �� �������", L"������ !!!", MB_ICONERROR);
	}
}
void LoadDateFile::Do(HWND h)
{
	OpenData o(h);
	if(o())
	{
		if(!Do(o.sFile))
		{
			MessageBox(0, L"���� �� ��������", L"������ !!!", MB_ICONERROR);
		}
	}
}

bool LoadDateFile::Do(wchar_t *path)
{
	FILE *f = _wfopen(path, L"rb");
	close_file c_f(f);
	bool b = false;
	if(NULL != f)
	{
		int len = wcslen(path);
		wchar_t *s = path;
		for(int i = len; i > 0; --i)
		{
			if(s[i] == '\\' || s[i] == '/')
			{
				computeSolidGroup.currentFile = &s[i+1];
				break;
			}
		}
#ifndef EMUL
		b = fread(solidData.reference, sizeof(solidData.reference), 1, f)
			&& fread(&solidData.signal, sizeof(solidData.signal), 1, f)
			;
		memmove(solidData.referenceNoFiltre, solidData.reference, sizeof(solidData.referenceNoFiltre));
		memmove(solidData.signalNoFiltre, solidData.signal, sizeof(solidData.signalNoFiltre));		
#else
		int size;
		if(fread(&size, sizeof(size), 1, f))
		{
			if(size < SolidData::MAX_ZONES_COUNT)
			{
				b = fread(&solidData.reference[solidData.start], sizeof(double) * size, 1, f)
					&& fread(&solidData.signal[solidData.start], sizeof(double) * size, 1, f)
					;
				memmove(solidData.referenceNoFiltre, solidData.reference, sizeof(solidData.referenceNoFiltre));
				memmove(solidData.signalNoFiltre, solidData.signal, sizeof(solidData.signalNoFiltre));
			}
		}

#endif
	}
	return b;
}


void StoreDataFile(wchar_t *path)
{
	FILE *f= _wfopen(path, L"wb+");
	if(NULL != f)
	{
			fwrite(solidData.referenceNoFiltre, sizeof(solidData.referenceNoFiltre), 1, f)
			&& fwrite(&solidData.signalNoFiltre, sizeof(solidData.signalNoFiltre), 1, f)
			;
		fclose(f);
	}
}

bool FileExist(wchar_t *subDir, wchar_t *fileName)
{
	wchar_t path[1024];
	GetModuleFileName(0, path, 1024);
	PathRemoveFileSpec(path);
	wsprintf(&path[wcslen(path)], L"\\%s\\%s", subDir, fileName);
	return 0 != PathFileExists(path);
}

wchar_t *CreateNameFile(wchar_t *subDir, wchar_t *typeSize, wchar_t *solidGroup, wchar_t (&path)[1024])
{
	GetModuleFileName(0, path, 1024);
	PathRemoveFileSpec(path);
	wsprintf(&path[wcslen(path)], L"\\%s", subDir);
	CreateDirectory(path, NULL);
	wsprintf(&path[wcslen(path)], L"\\%s\\", typeSize);
	CreateDirectory(path, NULL);
	time_t t = time(0);
	struct tm *now = localtime( & t );
	int len = wcslen(path);
	wsprintf(&path[len], L"%06d_%s_%s_%02d%02d%02d%02d%02d%02d.dat"
		, CounterTubes::CountTypeAll()
		, typeSize
		, solidGroup
		, now->tm_year - 100
		, now->tm_mon + 1
		, now->tm_mday
		, now->tm_hour
		, now->tm_min
		, now->tm_sec
		);
	return &path[len];
}