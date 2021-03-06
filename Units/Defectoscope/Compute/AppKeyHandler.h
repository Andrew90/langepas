#pragma once
#include "resource.h"
#include "templates/typelist.hpp"

namespace AppKeyHandler
{
	void Init();
	void Run();
	void Continue();
	void Stop();
	void RunContine();

	void ExitTubeMode();
	void ReturnTubeMode();

	void KeyPressed(unsigned );

	typedef TL::MkTlst<
		TL::IntToType<IDB_CycleBtn>
		, TL::IntToType<IDB_Continue>
		, TL::IntToType<IDB_Reset>
		, TL::IntToType<IDB_QueryBtn>
		, TL::IntToType<IDB_arrow_down>
		, TL::IntToType<IDB_arrow_up>
		, TL::IntToType<IDB_arrow_left>
		, TL::IntToType<IDB_arrow_right>
		, TL::IntToType<IDB_ExitTubeBtn>
		, TL::IntToType<IDB_ReturnTubeBtn>
		//, TL::IntToType<IDB_ClampBtn>
	    //, TL::IntToType<IDB_SensorsUnit>
	>::Result button_list;
}