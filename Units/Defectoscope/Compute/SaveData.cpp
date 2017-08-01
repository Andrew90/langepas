#include "stdafx.h"
#include "Compute\SaveData.h"
#include <stdio.h>
#include "lir\SubLir.h"

namespace ComputeData
{
	void Save(wchar_t *path)
	{
		FILE *f;
		if(0 == _wfopen_s(&f, path, L"wb+"))
		{
			SubLir &lir = Singleton<SubLir>::Instance();
			fwrite(&lir, sizeof(lir), 1, f);
			ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
			fwrite(&crossData, sizeof(crossData), 1, f);
			ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
			fwrite(&longData, sizeof(longData), 1, f);
			ItemData<Thick> &thickData = Singleton<ItemData<Thick>>::Instance();
			fwrite(&thickData, sizeof(thickData), 1, f);
			fclose(f);
		}
	}

	void Load(wchar_t *path)
	{
		FILE *f;
		if(0 == _wfopen_s(&f, path, L"rb"))
		{
			SubLir &lir = Singleton<SubLir>::Instance();
			fread(&lir, sizeof(lir), 1, f);
			ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
			fread(&crossData, sizeof(crossData), 1, f);
			ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
			fread(&longData, sizeof(longData), 1, f);
			ItemData<Thick> &thickData = Singleton<ItemData<Thick>>::Instance();
			fread(&thickData, sizeof(thickData), 1, f);
			fclose(f);
		}
	}
}