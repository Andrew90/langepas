#include "stdafx.h"
#include "ControlMode.h"
#include "Automat.h"
#include "Automat.hpp"

namespace Mode
{
	using namespace AutomatN;
	void ReturnTube(Data &data)
	{
	   dprint("ReturnTube\n");

	   SET_BITS(On<oPR_OP>);
	}
}