#pragma once
#include "Inc/bdaqctrl.h" 
#include "App/Config.h"
class Device1730
{
	static const int startPoint = 0;
	Automation::BDaq::InstantDoCtrl *instantDoCtrl;
	Automation::BDaq::InstantDiCtrl *instantDiCtrl;
	CRITICAL_SECTION cs;
	bool isOpen;	
#ifdef DEBUG_ITEMS
	int num;
#endif
public:	
	Device1730();
	bool Init(wchar_t *deviceDescription);
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write(unsigned);
	unsigned ReadOutput();

	void WriteOutput(unsigned, unsigned = (unsigned)-1);

	void AddBits(unsigned);
};



