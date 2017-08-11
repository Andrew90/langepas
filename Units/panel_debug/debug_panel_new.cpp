#include "stdafx.h"
#include <clocale>
#include "tools_debug/DebugMess.h"


int _tmain(int argc, _TCHAR* argv[])
{
	ViewerDebugMess x;
	for(;;)
	{
		char *b = x.get();
		if(NULL != b) printf(b);
	}
	return 0;
}

