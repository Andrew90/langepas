#pragma once
#include <windows.h>
#include "templates/typelist.hpp"
//-----------------------------------------------------------------
class CopyFactory
{
template<class O, class P>struct loc
{
	void operator ()(O *o, P *p)
	{
		o->value = p->get<O>().value;
	}
};
public:
	template<class List>static void Do(TL::Factory<List> &to, TL::Factory<List> &from)
	{
		TL::foreach<List, loc>()(&to, &from);
	}
};
//-------------------------------------------------------------------
#define DLG(n)struct n{static void Do(HWND);};

DLG(Descriptor1730Dlg)
DLG(Outputs1Dlg)
DLG(Inputs1Dlg)

DLG(Outputs2Dlg)
DLG(Inputs2Dlg)

DLG(AddTypeSizeDlg)
DLG(DelTypeSizeDlg)
DLG(DeadZonesDlg)
DLG(AboutWindowDlg)
DLG(IOportsDlg)
DLG(TestTabsDlg)
DLG(RecalculationDlg)
DLG(MedianFiltreDlg)
DLG(ColorItemsDlg)

DLG(OffsetsSensorsDlg)

DLG(StoredDataDlg)
DLG(LoadDataDlg)

DLG(AnalogPlateDlg)
DLG(BaseLengthDlg)

DLG(ProtectiveThickeningDlg)
DLG(StoreXMLDlg)

DLG(ThresholdCrossDlg)
DLG(ThresholdLongDlg)
DLG(ThresholdThickDlg)

DLG(AcfCutOffBordersDlg)

DLG(FilderDlg)

DLG(ThresholdDlg)
DLG(PrimarySignalDlg)

DLG(FrameWindowDlg)

DLG(SolidDlg)
DLG(OffsetsChannelDlg)
DLG(AmplificationChannelDlg)

DLG(LirDlg)
DLG(RotationSpeedDlg)

DLG(ComPortDlg)

DLG(OffsetSensorDlg)

DLG(AdditionalParamsDlg)

DLG(LoadDlg)
DLG(SaveDlg)

DLG(MinimalTubeLengthDlg)

DLG(LaunchExternalProgramDlg)

DLG(ModeChannelDlg)

DLG(Digital502Dlg)

DLG(ModulePositionDlg)
DLG(RotationScanningDevice)

DLG(ThicknessComTest)
DLG(ASUComTest)

DLG(ChannelSamplingRateDlg)


#undef DLG
