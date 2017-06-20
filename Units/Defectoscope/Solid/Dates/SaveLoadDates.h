#pragma once
#include <windows.h>

struct SaveDateFile
{
	static void Do(HWND );
};

struct LoadDateFile
{
	static void Do(HWND );
	static bool Do(wchar_t *);
};

void StoreDataFile(wchar_t *path);

bool FileExist(wchar_t *subDir, wchar_t *fileName);

wchar_t *CreateNameFile(wchar_t *subDir, wchar_t *typeSize, wchar_t *solidGroup, wchar_t (&)[1024]);