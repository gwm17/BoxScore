#include "DigitizerDefs.h"

namespace BoxScore {

	std::string BoardIOToString(int value)
	{
		switch (value)
		{
		case CAEN_DGTZ_IOLevel_NIM: return "NIM";
		case CAEN_DGTZ_IOLevel_TTL: return "TTL";
		}
		return "None";
	}

	std::string PolarityToString(int value)
	{
		switch (value)
		{
		case CAEN_DGTZ_PulsePolarityPositive: return "Positive";
		case CAEN_DGTZ_PulsePolarityNegative: return "Negative";
		}

		return "None";
	}

	std::string DynamicRangeToString(DynamicRange range)
	{
		switch (range)
		{
		case DynamicRange::MilliVolt_500: return "0.5V";
		case DynamicRange::Volt_2: return "2.0V";
		}
		return "None";
	}

	//Simple integer power calc
	int Power(int base, int order)
	{
		int value = 1;
		for (int i = 1; i <= order; i++)
			value = value * base;
		return value;
	}

	std::string IntSwitchToString(int value)
	{
		switch (value)
		{
		case 0: return "Off";
		case 1: return "On";
		}
		return "None";
	}

	std::string DiscriminatorModeToString(int mode)
	{
		switch (mode)
		{
		case CAEN_DGTZ_DPP_DISCR_MODE_CFD: return "CFD";
		case CAEN_DGTZ_DPP_DISCR_MODE_LED: return "Leading-Edge";
		}
		return "None";
	}

	std::string CFDFractionToString(int val)
	{
		switch (val)
		{
		case 0: return "0.25";
		case 1: return "0.5";
		case 2: return "0.75";
		case 3: return "1.0";
		}
		return "None";
	}

	std::string TriggerConfigToString(int val)
	{
		switch (val)
		{
		case CAEN_DGTZ_DPP_TriggerMode_Normal: return "Normal";
		case CAEN_DGTZ_DPP_TriggerMode_Coincidence: return "External Coinc.";
		}
		return "None";
	}

	std::string PileUpModeToString(int val)
	{
		switch (val)
		{
		case CAEN_DGTZ_DPP_PSD_PUR_DetectOnly: return "Off";
		case CAEN_DGTZ_DPP_PSD_PUR_Enabled: return "On";
		}
		return "None";
	}

	//Requires both dynamic range and charge sens. value (different ranges = different values)
	std::string ChargeSensToString(int cs_val, DynamicRange range)
	{
		if (range == DynamicRange::MilliVolt_500)
		{
			switch (cs_val)
			{
			case 0: return "1.25 fC/LSB";
			case 1: return "5 fC/LSB";
			case 2: return "20 fC/LSB";
			case 3: return "80 fC/LSB";
			case 4: return "320 fC/LSB";
			case 5: return "1.28 pC/LSB";
			}
		}
		else if (range == DynamicRange::Volt_2)
		{
			switch (cs_val)
			{
			case 0: return "5 fC/LSB";
			case 1: return "20 fC/LSB";
			case 2: return "80 fC/LSB";
			case 3: return "320 fC/LSB";
			case 4: return "1.28 pC/LSB";
			case 5: return "5.12 pC/LSB";
			}
		}
		return "None";
	}

	std::string PHAVirtualProbe1ToString(int val)
	{
		switch (val)
		{
		case PHAVirtualProbe1Options::PHAVP1_Input: return "Input";
		case PHAVirtualProbe1Options::PHAVP1_Trapezoid: return "Trapezoid";
		case PHAVirtualProbe1Options::PHAVP1_Delta: return "Delta";
		case PHAVirtualProbe1Options::PHAVP1_Delta2: return "Delta2";
		}
		return "None";
	}

	std::string PHAVirtualProbe2ToString(int val)
	{
		switch (val)
		{
		case PHAVirtualProbe2Options::PHAVP2_Input: return "Input";
		case PHAVirtualProbe2Options::PHAVP2_Baseline: return "Baseline";
		case PHAVirtualProbe2Options::PHAVP2_Threshold: return "Threshold";
		case PHAVirtualProbe2Options::PHAVP2_TrapezoidReduced: return "Trap. Reduced";
		case PHAVirtualProbe2Options::PHAVP2_None: return "None";
		}
		return "None";
	}

	std::string PHADigitalProbeToString(int val)
	{
		switch (val)
		{
		case PHADigitalProbe1Options::PHADP_ACQVeto: return "Acq. Veto";
		case PHADigitalProbe1Options::PHADP_Armed: return "Armed";
		case PHADigitalProbe1Options::PHADP_BaselineFreeze: return "Baseline Freeze";
		case PHADigitalProbe1Options::PHADP_BFMVeto: return "BFM Veto";
		case PHADigitalProbe1Options::PHADP_Busy: return "Busy";
		case PHADigitalProbe1Options::PHADP_CoincidenceWindow: return "Coincidence Window";
		case PHADigitalProbe1Options::PHADP_ExternalTrig: return "External Trig.";
		case PHADigitalProbe1Options::PHADP_Peaking: return "Peaking";
		case PHADigitalProbe1Options::PHADP_PileUp: return "PileUp";
		case PHADigitalProbe1Options::PHADP_PrgVeto: return "PrgVeto";
		case PHADigitalProbe1Options::PHADP_TriggerHoldoff: return "Trig. Hold-Off";
		case PHADigitalProbe1Options::PHADP_TriggerValue: return "Trig. Value";
		case PHADigitalProbe1Options::PHADP_TriggerWindow: return "Trig. Window";
		}
		return "None";
	}

	std::string PSDVirtualProbe1ToString(int val)
	{
		switch (val)
		{
		case PSDVirtualProbe1Options::PSDVP1_Input: return "Input";
		case PSDVirtualProbe1Options::PSDVP1_CFD: return "CFD";
		}
		return "None";
	}

	std::string PSDVirtualProbe2ToString(int val)
	{
		switch (val)
		{
		case PSDVirtualProbe2Options::PSDVP2_Baseline: return "Baseline";
		case PSDVirtualProbe2Options::PSDVP2_CFD: return "CFD";
		case PSDVirtualProbe2Options::PSDVP2_None: return "None";
		}

		return "None";
	}

	std::string PSDDigitalProbe1ToString(int val)
	{
		switch (val)
		{
		case PSDDigitalProbe1Options::PSDDP1_Coincidence: return "Coincidence";
		case PSDDigitalProbe1Options::PSDDP1_CoincidenceWindow: return "Coincidence Window";
		case PSDDigitalProbe1Options::PSDDP1_Gate: return "Gate";
		case PSDDigitalProbe1Options::PSDDP1_OverThreshold: return "Over Threshold";
		case PSDDigitalProbe1Options::PSDDP1_PileUp: return "PileUp";
		case PSDDigitalProbe1Options::PSDDP1_Trigger: return "Trigger";
		case PSDDigitalProbe1Options::PSDDP1_TriggerOut: return "TriggerOut";
		}

		return "None";
	}

	std::string PSDDigitalProbe2ToString(int val)
	{
		switch (val)
		{
		case PSDDigitalProbe2Options::PSDDP2_Coincidence: return "Coincidence";
		case PSDDigitalProbe2Options::PSDDP2_GateShort: return "Short Gate";
		case PSDDigitalProbe2Options::PSDDP2_OverThreshold: return "Over Threshold";
		case PSDDigitalProbe2Options::PSDDP2_PileUp: return "PileUp";
		case PSDDigitalProbe2Options::PSDDP2_Trigger: return "Trigger";
		case PSDDigitalProbe2Options::PSDDP2_TriggerHoldoff: return "Trig. Hold-Off";
		case PSDDigitalProbe2Options::PSDDP2_TriggerVal: return "Trig. Value";
		}

		return "None";
	}

	std::string SyncModeToString(int val)
	{
		switch (val)
		{
		case CAEN_DGTZ_RunSyncMode_t::CAEN_DGTZ_RUN_SYNC_Disabled: return "Disabled";
		case CAEN_DGTZ_RunSyncMode_t::CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain: return "TrigOut-S In";
		case CAEN_DGTZ_RunSyncMode_t::CAEN_DGTZ_RUN_SYNC_SinFanout: return "S In Fanout";
		case CAEN_DGTZ_RunSyncMode_t::CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain: return "TrigOut-TrigIn";
		case CAEN_DGTZ_RunSyncMode_t::CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain: return "Broken";
		}

		return "None";
	}

	std::string AcqModeToString(int val)
	{
		switch (val)
		{
		case CAEN_DGTZ_AcqMode_t::CAEN_DGTZ_FIRST_TRG_CONTROLLED: return "FirstTrg";
		case CAEN_DGTZ_AcqMode_t::CAEN_DGTZ_LVDS_CONTROLLED: return "LVDS";
		case CAEN_DGTZ_AcqMode_t::CAEN_DGTZ_SW_CONTROLLED: return "Software";
		case CAEN_DGTZ_AcqMode_t::CAEN_DGTZ_S_IN_CONTROLLED: return "S In";
		}

		return "None";
	}

	std::string FirmwareTypeToString(int val)
	{
		switch (val)
		{
		case CAEN_DGTZ_DPPFirmware_PHA: return "PHA";
		case CAEN_DGTZ_DPPFirmware_PSD: return "PSD";
		case CAEN_DGTZ_DPPFirmware_CI: return "CI";
		case CAEN_DGTZ_DPPFirmware_DAW: return "DAW";
		case CAEN_DGTZ_DPPFirmware_QDC: return "QDC";
		case CAEN_DGTZ_DPPFirmware_ZLE: return "ZLE";
		case CAEN_DGTZ_DPPFirmwareNotSupported: return "NotSupported";
		}

		return "None";
	}

	std::string SyncMethodToString(SyncMethod method)
	{
		switch (method)
		{
		case SyncMethod::SIn_TrigOut: return "S-In TrigOut";
		case SyncMethod::None: return "None";
		}

		return "None";
	}

	double GetSamplingPeriod(CAEN_DGTZ_BoardModel_t model)
	{
		switch (model)
		{
		case CAEN_DGTZ_V1730: return 2.0; //ns -> 500 MSamples/s
		case CAEN_DGTZ_V1725: return 4.0; //ns -> 250 MSamples/s
		}

		return 4.0; //Idk? This is a fairly common value...
	}
}