#include "stdafx.h"
#include "tools_debug/DebugMess.h"

DebugMess::DebugMess(){}
void DebugMess::print(char *, ...)
{
}

class DebugMess::Initialization
{
	DebugMess debug;
public:
	static DebugMess &Instance()
	{
		static Initialization x; 
		return x.debug;
	}
};
DebugMess &debug = DebugMess::Initialization::Instance();