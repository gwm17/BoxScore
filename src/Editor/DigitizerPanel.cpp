#include "DigitizerPanel.h"

namespace BoxScore {

	static std::string PanelTypeToString(DigitizerPanel::Type type)
	{
		switch (type)
		{
		case DigitizerPanel::Type::PHA: return "DPP PHA";
		case DigitizerPanel::Type::PSD: return "DPP PSD";
		case DigitizerPanel::Type::None: return "Invalid firmware";
		}
	}

	static std::string BoardIOToString(int value)
	{
		switch (value)
		{
		case CAEN_DGTZ_IOLevel_NIM: return "NIM";
		case CAEN_DGTZ_IOLevel_TTL: return "TTL";
		}
		return "None";
	}

	static std::string PolarityToString(int value)
	{
		switch (value)
		{
		case CAEN_DGTZ_PulsePolarityPositive: return "Positive";
		case CAEN_DGTZ_PulsePolarityNegative: return "Negative";
		}

		return "None";
	}

	static std::string DynamicRangeToString(DynamicRange range)
	{
		switch (range)
		{
		case DynamicRange::MilliVolt_500: return "0.5V";
		case DynamicRange::Volt_2: return "2.0V";
		}
		return "None";
	}

	//Simple integer power calc
	static int Power(int base, int order)
	{
		int value = 1;
		for (int i = 1; i <= order; i++)
			value = value * base;
		return value;
	}

	static std::string IntSwitchToString(int value)
	{
		switch (value)
		{
		case 0: return "Off";
		case 1: return "On";
		}
		return "None";
	}

	DigitizerPanel::DigitizerPanel(const DigitizerArgs& args) :
		m_args(args)
	{
		Init();
	}

	DigitizerPanel::~DigitizerPanel() {}

	void DigitizerPanel::Init()
	{
		if (m_args.firmware == CAEN_DGTZ_DPPFirmware_PHA)
		{
			m_panelType = Type::PHA;
			m_panelName = m_args.name + "_PHA";
			m_phaChannels.resize(m_args.channels);
		}
		else if (m_args.firmware == CAEN_DGTZ_DPPFirmware_PSD)
		{
			m_panelType = Type::PSD;
			m_panelName = m_args.name + "_PSD";
			m_psdChannels.resize(m_args.channels);
		}
		else
		{
			m_panelType = Type::None;
			m_panelName = m_args.name + "_None";
		}
	}

	void DigitizerPanel::SetDigitizerParameters(const DigitizerParameters& params)
	{
		m_params = params;
	}

	void DigitizerPanel::SetPHAParameters(const std::vector<PHAParameters>& ch_params, const PHAWaveParameters& wv_params)
	{
		if (ch_params.size() != m_phaChannels.size())
		{
			BS_ERROR("Trying to load channel parameters of size {0} to panel with {1} channels", ch_params.size(), m_phaChannels.size());
			return;
		}

		m_phaChannels = ch_params;
		m_phaWaves = wv_params;
	}

	void DigitizerPanel::SetPSDParameters(const std::vector<PSDParameters>& ch_params, const PSDWaveParameters& wv_params)
	{
		if (ch_params.size() != m_psdChannels.size())
		{
			BS_ERROR("Trying to load channel parameters of size {0} to panel with {1} channels", ch_params.size(), m_psdChannels.size());
			return;
		}

		m_psdChannels = ch_params;
		m_psdWaves = wv_params;
	}

	bool DigitizerPanel::OnImGuiRender()
	{
		bool changed = false;
		if (ImGui::BeginTabItem(m_panelName.c_str()))
		{
			ImGui::Text(PanelTypeToString(m_panelType).c_str());
			ImGui::Text(fmt::format("Optical Link: {0} CONET Node: {1}", m_args.linkNumber, m_args.conetNode).c_str());
			ImGui::Text("Settings file: ");
			ImGui::SameLine();
			ImGui::Text(m_settingsFile.c_str());
			ImGui::Button("Open...");
			ImGui::SameLine();
			ImGui::Button("Save...");

			ImGui::NewLine();
			ImGui::Text("Board Global Settings");
			changed |= RenderDigitizerParams();

			ImGui::NewLine();
			ImGui::Text("PHA Channel Settings");
			changed |= RenderPHAParameters();

			ImGui::EndTabItem();
		}

		return changed;
	}

	bool DigitizerPanel::RenderDigitizerParams()
	{
		bool changed = false;
		static bool globalEnable = true;
		static uint32_t stepFast = 100;
		static uint32_t stepSlow = 10;
		if (ImGui::BeginTable("Digitizer Parameters", 4, tableFlags))
		{
			ImGui::TableSetupColumn("Enable/Disable");
			ImGui::TableSetupColumn("Record Length");
			ImGui::TableSetupColumn("Event Aggregation");
			//ImGui::TableSetupColumn("Acq. Mode");
			//ImGui::TableSetupColumn("DPP Acq. Mode");
			ImGui::TableSetupColumn("Board IO Level");
			//ImGui::TableSetupColumn("Trigger Mode");
			//ImGui::TableSetupColumn("Sync Mode");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::RadioButton("##globalenable", globalEnable))
			{
				changed |= true;
				globalEnable = !globalEnable;
				if (globalEnable)
					m_params.channelMask = 0xffff;
				else
					m_params.channelMask = 0;
			}
			ImGui::TableNextColumn();
			if (ImGui::InputScalar("##recordlength", ImGuiDataType_U32, &m_params.recordLength, &stepSlow, &stepFast))
			{
				changed |= true;
			}
			ImGui::TableNextColumn();
			if (ImGui::InputScalar("##eventAggregate", ImGuiDataType_U32, &m_params.eventAggr, &stepSlow, &stepFast))
			{
				changed |= true;
			}
			ImGui::TableNextColumn();
			if (ImGui::BeginCombo("##boardIO", BoardIOToString(m_params.IOlevel).c_str()))
			{
				if (ImGui::Selectable("NIM", "NIM" == BoardIOToString(m_params.IOlevel)))
				{
					changed |= true;
					m_params.IOlevel = CAEN_DGTZ_IOLevel_NIM;
				}
				if (ImGui::Selectable("TTL", "TTL" == BoardIOToString(m_params.IOlevel)))
				{
					changed |= true;
					m_params.IOlevel = CAEN_DGTZ_IOLevel_TTL;
				}
				ImGui::EndCombo();
			}

			ImGui::EndTable();
		}

		return changed;
	}

	//PHA options; given per channel in the digitizer
	bool DigitizerPanel::RenderPHAParameters()
	{
		bool changed = false;
		static uint32_t stepFast = 100;
		static uint32_t stepSlow = 10;
		static uint32_t stepFast_Int = 100;
		static uint32_t stepSlow_Int = 10;
		static float stepFast_Float = 100.0f;
		static float stepSlow_Float = 10.0f;
		static std::string tempString; //useful for comps in widgets
		if (ImGui::BeginTable("PHA Channel Parameters", 22, tableFlags | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2(0, 300)))
		{
			ImGui::TableSetupColumn("Channel");
			ImGui::TableSetupColumn("Enable/Disable");
			ImGui::TableSetupColumn("Pre-Trigger Samples");
			ImGui::TableSetupColumn("DC Offset (%)");
			ImGui::TableSetupColumn("Polarity");
			ImGui::TableSetupColumn("Dynamic Range");
			ImGui::TableSetupColumn("Decay Time Const");
			ImGui::TableSetupColumn("Trap. Flat Top");
			ImGui::TableSetupColumn("Trap. Rise Time");
			ImGui::TableSetupColumn("Flat Top Delay");
			ImGui::TableSetupColumn("Smoothing");
			ImGui::TableSetupColumn("Input Rise Time");
			ImGui::TableSetupColumn("Threshold");
			ImGui::TableSetupColumn("Baseline Samples");
			ImGui::TableSetupColumn("Peak Samples");
			ImGui::TableSetupColumn("Peak Hold-off");
			ImGui::TableSetupColumn("Baseline Hold-off");
			ImGui::TableSetupColumn("Trigger Hold-off");
			ImGui::TableSetupColumn("Validation Window");
			ImGui::TableSetupColumn("Rise Time Disc.");
			ImGui::TableSetupColumn("Probe Gain");
			ImGui::TableSetupColumn("Input Samples");
			ImGui::TableHeadersRow();

			for (int i = 0; i < m_phaChannels.size(); i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				auto& channel = m_phaChannels[i];
				if (ImGui::RadioButton(fmt::format("##channelEnable_{0}", i).c_str(), channel.isEnabled))
				{
					changed |= true;
					channel.isEnabled = !channel.isEnabled;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputScalar(fmt::format("##pretrigger_{0}", i).c_str(), ImGuiDataType_U32, &channel.preTriggerSamples, &stepSlow, &stepFast))
				{
					changed |= true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputFloat(fmt::format("##dcoff_{0}", i).c_str(), &channel.dcOffset, stepSlow_Float, stepFast_Float))
				{
					changed |= true;
				}
				ImGui::TableNextColumn();
				tempString = PolarityToString(channel.pulsePolarity);
				if (ImGui::BeginCombo(fmt::format("##polarity_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("Positive", "Positive" == tempString))
					{
						changed |= true;
						channel.pulsePolarity = CAEN_DGTZ_PulsePolarityPositive;
					}
					if (ImGui::Selectable("Negative", "Negative" == tempString))
					{
						changed |= true;
						channel.pulsePolarity = CAEN_DGTZ_PulsePolarityNegative;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = DynamicRangeToString(channel.dynamicRange);
				if (ImGui::BeginCombo(fmt::format("##dynamicRange_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("0.5V", "0.5V" == tempString))
					{
						changed |= true;
						channel.dynamicRange = DynamicRange::MilliVolt_500;
					}
					if (ImGui::Selectable("2.0V", "2.0V" == tempString))
					{
						changed |= true;
						channel.dynamicRange = DynamicRange::Volt_2;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##decayTC_{0}", i).c_str(), &channel.decayTimeConst, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##trapFlatTop_{0}", i).c_str(), &channel.trapFlatTop, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##trapRiseT_{0}", i).c_str(), &channel.trapRiseTime, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##flatTopDelay_{0}", i).c_str(), &channel.decayTimeConst, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				tempString = std::to_string(channel.triggerFilterSmoothing);
				if(ImGui::BeginCombo(fmt::format("##smoothing_{0}", i).c_str(), tempString.c_str()))
				{
					int val;
					for (int pow = 0; pow < 6; pow++)
					{
						val = Power(2, pow);
						if (ImGui::Selectable(std::to_string(val).c_str(), val == channel.triggerFilterSmoothing))
						{
							changed = true;
							channel.triggerFilterSmoothing = val;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##inputRiseTime_{0}", i).c_str(), &channel.inputRiseTime, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##triggerThresh_{0}", i).c_str(), &channel.triggerThreshold, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				tempString = std::to_string(Power(4, 1+channel.samplesBaseLineMean));
				if (ImGui::BeginCombo(fmt::format("##samplesBase_{0}", i).c_str(), tempString.c_str()))
				{
					int val;
					for (int pow = 1; pow < 7; pow++)
					{
						val = Power(4, pow + 1);
						if (ImGui::Selectable(std::to_string(val).c_str(), pow == channel.samplesBaseLineMean))
						{
							changed = true;
							channel.samplesBaseLineMean = pow;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = std::to_string(Power(4, channel.samplesPeakMean));
				if (ImGui::BeginCombo(fmt::format("##samplesPeak_{0}", i).c_str(), tempString.c_str()))
				{
					int val;
					for (int pow = 0; pow < 4; pow++)
					{
						val = Power(4, pow);
						if (ImGui::Selectable(std::to_string(val).c_str(), pow == channel.samplesPeakMean))
						{
							changed = true;
							channel.samplesPeakMean = pow;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##peakHold_{0}", i).c_str(), &channel.peakHoldOff, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##baseHold_{0}", i).c_str(), &channel.baseLineHoldOff, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##baseHold_{0}", i).c_str(), &channel.triggerHoldOff, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##validWin_{0}", i).c_str(), &channel.riseTimeValidationWindow, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				tempString = IntSwitchToString(channel.riseTimeDiscrimination);
				if (ImGui::BeginCombo(fmt::format("##riseTimeDisc_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("On", "On" == tempString))
					{
						changed = true;
						channel.riseTimeDiscrimination = 1;
					}
					if (ImGui::Selectable("Off", "Off" == tempString))
					{
						changed = true;
						channel.riseTimeDiscrimination = 0;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = std::to_string(Power(2, channel.digitalProbeGain));
				if (ImGui::BeginCombo(fmt::format("##digitalGain_{0}", i).c_str(), tempString.c_str()))
				{
					int value;
					for (int pow = 0; pow < 4; pow++)
					{
						value = Power(2, pow);
						if (ImGui::Selectable(std::to_string(value).c_str(), pow == channel.digitalProbeGain))
						{
							changed = true;
							channel.digitalProbeGain = pow;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = std::to_string(Power(2, channel.inputDecimation));
				if (ImGui::BeginCombo(fmt::format("##inputDeci_{0}", i).c_str(), tempString.c_str()))
				{
					int value;
					for (int pow = 0; pow < 4; pow++)
					{
						value = Power(2, pow);
						if (ImGui::Selectable(std::to_string(value).c_str(), pow == channel.inputDecimation))
						{
							changed = true;
							channel.inputDecimation = pow;
						}
					}
					ImGui::EndCombo();
				}
			}
			ImGui::EndTable();
		}

		return changed;
	}
}