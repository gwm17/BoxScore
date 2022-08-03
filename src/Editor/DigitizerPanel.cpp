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
			m_digitizerEnabled = true;
		}
		else if (m_args.firmware == CAEN_DGTZ_DPPFirmware_PSD)
		{
			m_panelType = Type::PSD;
			m_panelName = m_args.name + "_PSD";
			m_psdChannels.resize(m_args.channels);
			m_digitizerEnabled = true;
		}
		else
		{
			m_panelType = Type::None;
			m_panelName = m_args.name + "_None";
			m_digitizerEnabled = false;
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

			switch (m_panelType)
			{
				case Type::PHA:
				{
					ImGui::NewLine();
					ImGui::Text("PHA Channel Settings");
					changed |= RenderPHAParameters();
					ImGui::NewLine();
					ImGui::Text("PHA Waves Settings");
					changed |= RenderPHAWaveParameters();
					break;
				}
				case Type::PSD:
				{
					ImGui::NewLine();
					ImGui::Text("PSD Channel Settings");
					changed |= RenderPSDParameters();
					ImGui::NewLine();
					ImGui::Text("PSD Waves Settings");
					changed |= RenderPSDWaveParameters();
					break;
				}
				case Type::None: break;
			}

			ImGui::EndTabItem();
		}

		return changed;
	}

	bool DigitizerPanel::RenderDigitizerParams()
	{
		bool changed = false;
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
			if (ImGui::RadioButton("##globalenable", m_digitizerEnabled))
			{
				changed |= true;
				m_digitizerEnabled = !m_digitizerEnabled;
				if (m_digitizerEnabled)
					m_params.channelMask = 0xffff;
				else
					m_params.channelMask = 0;
			}
			if (!m_digitizerEnabled)
				ImGui::BeginDisabled();
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
			if (!m_digitizerEnabled)
				ImGui::EndDisabled();
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
		if (!m_digitizerEnabled)
			ImGui::BeginDisabled();
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
				ImGui::Text("%d", i);
				ImGui::TableNextColumn();
				auto& channel = m_phaChannels[i];
				if (ImGui::RadioButton(fmt::format("##channelEnable_{0}", i).c_str(), channel.isEnabled))
				{
					changed |= true;
					channel.isEnabled = !channel.isEnabled;
				}
				if (!channel.isEnabled)
					ImGui::BeginDisabled();

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
				if (ImGui::InputInt(fmt::format("##trigHold_{0}", i).c_str(), &channel.triggerHoldOff, stepSlow_Int, stepFast_Int))
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

				if (!channel.isEnabled)
					ImGui::EndDisabled();
			}
			ImGui::EndTable();
		}
		if (!m_digitizerEnabled)
			ImGui::EndDisabled();
		return changed;
	}

	bool DigitizerPanel::RenderPSDParameters()
	{
		bool changed = false;
		static uint32_t stepFast = 100;
		static uint32_t stepSlow = 10;
		static uint32_t stepFast_Int = 100;
		static uint32_t stepSlow_Int = 10;
		static float stepFast_Float = 100.0f;
		static float stepSlow_Float = 10.0f;
		static std::string tempString; //useful for comps in widgets
		if (!m_digitizerEnabled)
			ImGui::BeginDisabled();
		if (ImGui::BeginTable("PSD Channel Parameters", 22, tableFlags | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2(0, 300)))
		{
			ImGui::TableSetupColumn("Channel");
			ImGui::TableSetupColumn("Enable/Disable");
			ImGui::TableSetupColumn("Pre-Trigger Samples");
			ImGui::TableSetupColumn("DC Offset (%)");
			ImGui::TableSetupColumn("Polarity");
			ImGui::TableSetupColumn("Dynamic Range");
			ImGui::TableSetupColumn("Baseline Threshold");
			ImGui::TableSetupColumn("Trigger Threshold");
			ImGui::TableSetupColumn("Trigger Hold-Off");
			ImGui::TableSetupColumn("Self-Trigger");
			ImGui::TableSetupColumn("Charge Sensitivity");
			ImGui::TableSetupColumn("Short Gate");
			ImGui::TableSetupColumn("Long Gate");
			ImGui::TableSetupColumn("Pre-Gate");
			ImGui::TableSetupColumn("Validation Window");
			ImGui::TableSetupColumn("Baseline Samples");
			ImGui::TableSetupColumn("Discrimintaor Mode");
			ImGui::TableSetupColumn("CFD Fraction");
			ImGui::TableSetupColumn("CFD Delay");
			ImGui::TableSetupColumn("Trigger Config.");
			ImGui::TableSetupColumn("PileUp Reject.");
			ImGui::TableSetupColumn("Purity Gap");
			ImGui::TableHeadersRow();

			for (size_t i = 0; i < m_psdChannels.size(); i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%d", i);
				ImGui::TableNextColumn();
				auto& channel = m_psdChannels[i];
				if (ImGui::RadioButton(fmt::format("##channelEnable_{0}", i).c_str(), channel.isEnabled))
				{
					changed |= true;
					channel.isEnabled = !channel.isEnabled;
				}
				if (!channel.isEnabled)
					ImGui::BeginDisabled();

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
				if (ImGui::InputInt(fmt::format("##baseThresh_{0}", i).c_str(), &channel.baselineThreshold, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##triggerThresh_{0}", i).c_str(), &channel.triggerThreshold, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##trigHold_{0}", i).c_str(), &channel.triggerHoldOff, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				tempString = IntSwitchToString(channel.selfTrigger);
				if (ImGui::BeginCombo(fmt::format("##selfTrig_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("On", "On" == tempString))
					{
						changed = true;
						channel.selfTrigger = 1;
					}
					if (ImGui::Selectable("Off", "Off" == tempString))
					{
						changed = true;
						channel.selfTrigger = 0;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = ChargeSensToString(channel.chargeSensitivity, channel.dynamicRange);
				if (ImGui::BeginCombo(fmt::format("##charge_sens{0}", i).c_str(), tempString.c_str()))
				{
					for (int val = 0; val < 6; val++)
					{
						if (ImGui::Selectable(ChargeSensToString(val, channel.dynamicRange).c_str(), val == channel.chargeSensitivity))
						{
							changed = true;
							channel.chargeSensitivity = val;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##shortGate_{0}", i).c_str(), &channel.shortGate, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##longGate_{0}", i).c_str(), &channel.longGate, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##preGate_{0}", i).c_str(), &channel.preGate, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##trigValidWindow_{0}", i).c_str(), &channel.triggerValidationWindow, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				tempString = channel.samplesBasline == 0 ? "Absolute" : std::to_string(Power(4, 1 + channel.samplesBasline));
				if (ImGui::BeginCombo(fmt::format("##samplesBase_{0}", i).c_str(), tempString.c_str()))
				{
					int val;
					if (ImGui::Selectable("Absolute", channel.samplesBasline == 0))
					{
						changed = true;
						channel.samplesBasline = 0;
					}
					for (int pow = 1; pow < 5; pow++)
					{
						val = Power(4, pow + 1);
						if (ImGui::Selectable(std::to_string(val).c_str(), pow == channel.samplesBasline))
						{
							changed = true;
							channel.samplesBasline = pow;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = DiscriminatorModeToString(channel.discrminatorType);
				if (ImGui::BeginCombo(fmt::format("##discrMode_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("CFD", CAEN_DGTZ_DPP_DISCR_MODE_CFD == channel.discrminatorType))
					{
						changed = true;
						channel.discrminatorType = CAEN_DGTZ_DPP_DISCR_MODE_CFD;
					}
					if (ImGui::Selectable("Leading-Edge", CAEN_DGTZ_DPP_DISCR_MODE_LED == channel.discrminatorType))
					{
						changed = true;
						channel.discrminatorType = CAEN_DGTZ_DPP_DISCR_MODE_LED;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = CFDFractionToString(channel.cfdFraction);
				if (ImGui::BeginCombo(fmt::format("##cfdFrac_{0}", i).c_str(), tempString.c_str()))
				{
					for (int val = 0; val < 4; val++)
					{
						if (ImGui::Selectable(CFDFractionToString(val).c_str(), val = channel.cfdFraction))
						{
							changed = true;
							channel.cfdFraction = val;
						}
					}
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##cfdDelay_{0}", i).c_str(), &channel.cfdDelay, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}
				ImGui::TableNextColumn();
				tempString = TriggerConfigToString(channel.triggerConfig);
				if (ImGui::BeginCombo(fmt::format("##trigConfig_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("Normal", CAEN_DGTZ_DPP_TriggerMode_Normal == channel.triggerConfig))
					{
						changed = true;
						channel.discrminatorType = CAEN_DGTZ_DPP_TriggerMode_Normal;
					}
					if (ImGui::Selectable("Leading-Edge", CAEN_DGTZ_DPP_TriggerMode_Coincidence == channel.discrminatorType))
					{
						changed = true;
						channel.discrminatorType = CAEN_DGTZ_DPP_TriggerMode_Coincidence;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				tempString = PileUpModeToString(channel.pileUpRejection);
				if (ImGui::BeginCombo(fmt::format("##purSwitch_{0}", i).c_str(), tempString.c_str()))
				{
					if (ImGui::Selectable("On", CAEN_DGTZ_DPP_PSD_PUR_Enabled == channel.pileUpRejection))
					{
						changed = true;
						channel.discrminatorType = CAEN_DGTZ_DPP_PSD_PUR_Enabled;
					}
					if (ImGui::Selectable("Off", CAEN_DGTZ_DPP_PSD_PUR_DetectOnly == channel.pileUpRejection))
					{
						changed = true;
						channel.discrminatorType = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
					}
					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				if (ImGui::InputInt(fmt::format("##purityGap_{0}", i).c_str(), &channel.purgap, stepSlow_Int, stepFast_Int))
				{
					changed = true;
				}

				if (!channel.isEnabled)
					ImGui::EndDisabled();
			}
			ImGui::EndTable();
		}
		if (!m_digitizerEnabled)
			ImGui::EndDisabled();
		return changed;
	}

	bool DigitizerPanel::RenderPHAWaveParameters()
	{
		bool changed = false;
		static std::string tempString;
		if (!m_digitizerEnabled)
			ImGui::BeginDisabled();
		if (ImGui::BeginTable("PHAWaveParams", 3, tableFlags))
		{
			ImGui::TableSetupColumn("VirtualProbe1");
			ImGui::TableSetupColumn("VirtualProbe2");
			ImGui::TableSetupColumn("DigitalProbe");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			tempString = PHAVirtualProbe1ToString(m_phaWaves.analogProbe1);
			if (ImGui::BeginCombo("##Vprobe1", tempString.c_str()))
			{
				if (ImGui::Selectable("Input", m_phaWaves.analogProbe1 == PHAVP1_Input))
				{
					changed = true;
					m_phaWaves.analogProbe1 = PHAVP1_Input;
				}
				if (ImGui::Selectable("Trapezoid", m_phaWaves.analogProbe1 == PHAVP1_Trapezoid))
				{
					changed = true;
					m_phaWaves.analogProbe1 = PHAVP1_Trapezoid;
				}
				if (ImGui::Selectable("Delta", m_phaWaves.analogProbe1 == PHAVP1_Delta))
				{
					changed = true;
					m_phaWaves.analogProbe1 = PHAVP1_Delta;
				}
				if (ImGui::Selectable("Delta2", m_phaWaves.analogProbe1 == PHAVP1_Delta2))
				{
					changed = true;
					m_phaWaves.analogProbe1 = PHAVP1_Delta2;
				}
				ImGui::EndCombo();
			}
			ImGui::TableNextColumn();
			tempString = PHAVirtualProbe2ToString(m_phaWaves.analogProbe2);
			if (ImGui::BeginCombo("##Vprobe2", tempString.c_str()))
			{
				if (ImGui::Selectable("Input", m_phaWaves.analogProbe2 == PHAVP2_Input))
				{
					changed = true;
					m_phaWaves.analogProbe2 = PHAVP2_Input;
				}
				if (ImGui::Selectable("Baseline", m_phaWaves.analogProbe2 == PHAVP2_Baseline))
				{
					changed = true;
					m_phaWaves.analogProbe2 = PHAVP2_Baseline;
				}
				if (ImGui::Selectable("Threshold", m_phaWaves.analogProbe2 == PHAVP2_Threshold))
				{
					changed = true;
					m_phaWaves.analogProbe2 = PHAVP2_Threshold;
				}
				if (ImGui::Selectable("Trap. Reduced", m_phaWaves.analogProbe2 == PHAVP2_TrapezoidReduced))
				{
					changed = true;
					m_phaWaves.analogProbe2 = PHAVP2_TrapezoidReduced;
				}
				if (ImGui::Selectable("None", m_phaWaves.analogProbe2 == PHAVP2_None))
				{
					changed = true;
					m_phaWaves.analogProbe2 = PHAVP2_None;
				}
				ImGui::EndCombo();
			}
			ImGui::TableNextColumn();
			tempString = PHADigitalProbeToString(m_phaWaves.digitalProbe1);
			if (ImGui::BeginCombo("##Dprobe1", tempString.c_str()))
			{
				if (ImGui::Selectable("Acq. Veto", m_phaWaves.digitalProbe1 == PHADP_ACQVeto))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_ACQVeto;
				}
				if (ImGui::Selectable("Armed", m_phaWaves.digitalProbe1 == PHADP_Armed))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_Armed;
				}
				if (ImGui::Selectable("Baseline Freeze", m_phaWaves.digitalProbe1 == PHADP_BaselineFreeze))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_BaselineFreeze;
				}
				if (ImGui::Selectable("BFM Veto", m_phaWaves.digitalProbe1 == PHADP_BFMVeto))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_BFMVeto;
				}
				if (ImGui::Selectable("Busy", m_phaWaves.digitalProbe1 == PHADP_Busy))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_Busy;
				}
				if (ImGui::Selectable("Coincidence Window", m_phaWaves.digitalProbe1 == PHADP_CoincidenceWindow))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_CoincidenceWindow;
				}
				if (ImGui::Selectable("Ext. Trigger", m_phaWaves.digitalProbe1 == PHADP_ExternalTrig))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_ExternalTrig;
				}
				if (ImGui::Selectable("Peaking", m_phaWaves.digitalProbe1 == PHADP_Peaking))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_Peaking;
				}
				if (ImGui::Selectable("PileUp", m_phaWaves.digitalProbe1 == PHADP_PileUp))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_PileUp;
				}
				if (ImGui::Selectable("PrgVeto", m_phaWaves.digitalProbe1 == PHADP_PrgVeto))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_PrgVeto;
				}
				if (ImGui::Selectable("Trig. Hold-Off", m_phaWaves.digitalProbe1 == PHADP_TriggerHoldoff))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_TriggerHoldoff;
				}
				if (ImGui::Selectable("Trig. Value", m_phaWaves.digitalProbe1 == PHADP_TriggerValue))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_TriggerValue;
				}
				if (ImGui::Selectable("Trig. Window", m_phaWaves.digitalProbe1 == PHADP_TriggerWindow))
				{
					changed = true;
					m_phaWaves.digitalProbe1 = PHADP_TriggerWindow;
				}
				ImGui::EndCombo();
			}

			ImGui::EndTable();
		}
		if (!m_digitizerEnabled)
			ImGui::EndDisabled();
		return changed;
	}

	bool DigitizerPanel::RenderPSDWaveParameters()
	{
		bool changed = false;
		static std::string tempString;
		if (!m_digitizerEnabled)
			ImGui::BeginDisabled();
		if (ImGui::BeginTable("PSDWaveParams", 4, tableFlags))
		{
			ImGui::TableSetupColumn("VirtualProbe1");
			ImGui::TableSetupColumn("VirtualProbe2");
			ImGui::TableSetupColumn("DigitalProbe1");
			ImGui::TableSetupColumn("DigitalProbe2");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			tempString = PSDVirtualProbe1ToString(m_psdWaves.analogProbe1);
			if (ImGui::BeginCombo("##virtprobe1", tempString.c_str()))
			{
				if (ImGui::Selectable("Input", m_psdWaves.analogProbe1 == PSDVP1_Input))
				{
					changed = true;
					m_psdWaves.analogProbe1 = PSDVP1_Input;
				}
				if (ImGui::Selectable("CFD", m_psdWaves.analogProbe1 == PSDVP1_CFD))
				{
					changed = true;
					m_psdWaves.analogProbe1 = PSDVP1_CFD;
				}
				ImGui::EndCombo();
			}
			ImGui::TableNextColumn();
			tempString = PSDVirtualProbe2ToString(m_psdWaves.analogProbe2);
			if (ImGui::BeginCombo("##virtprobe2", tempString.c_str()))
			{
				if (ImGui::Selectable("Baseline", m_psdWaves.analogProbe2 == PSDVP2_Baseline))
				{
					changed = true;
					m_psdWaves.analogProbe2 = PSDVP2_Baseline;
				}
				if (ImGui::Selectable("CFD", m_psdWaves.analogProbe2 == PSDVP2_CFD))
				{
					changed = true;
					m_psdWaves.analogProbe2 = PSDVP2_CFD;
				}
				if (ImGui::Selectable("None", m_psdWaves.analogProbe2 == PSDVP2_None))
				{
					changed = true;
					m_psdWaves.analogProbe2 = PSDVP2_None;
				}
				ImGui::EndCombo();
			}
			ImGui::TableNextColumn();
			tempString = PSDDigitalProbe1ToString(m_psdWaves.digitalProbe1);
			if (ImGui::BeginCombo("##digiprobe1", tempString.c_str()))
			{
				if (ImGui::Selectable("Coincidence", m_psdWaves.digitalProbe1 == PSDDP1_Coincidence))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_Coincidence;
				}
				if (ImGui::Selectable("Coincidence Window", m_psdWaves.digitalProbe1 == PSDDP1_CoincidenceWindow))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_CoincidenceWindow;
				}
				if (ImGui::Selectable("Gate", m_psdWaves.digitalProbe1 == PSDDP1_Gate))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_Gate;
				}
				if (ImGui::Selectable("Over Threshold", m_psdWaves.digitalProbe1 == PSDDP1_OverThreshold))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_OverThreshold;
				}
				if (ImGui::Selectable("PileUp", m_psdWaves.digitalProbe1 == PSDDP1_PileUp))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_PileUp;
				}
				if (ImGui::Selectable("Trigger", m_psdWaves.digitalProbe1 == PSDDP1_Trigger))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_Trigger;
				}
				if (ImGui::Selectable("TriggerOut", m_psdWaves.digitalProbe1 == PSDDP1_TriggerOut))
				{
					changed = true;
					m_psdWaves.digitalProbe1 = PSDDP1_TriggerOut;
				}
				ImGui::EndCombo();
			}
			ImGui::TableNextColumn();
			tempString = PSDDigitalProbe2ToString(m_psdWaves.digitalProbe2);
			if (ImGui::BeginCombo("##digiprobe2", tempString.c_str()))
			{
				if (ImGui::Selectable("Coincidence", m_psdWaves.digitalProbe2 == PSDDP2_Coincidence))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_Coincidence;
				}
				if (ImGui::Selectable("Short Gate", m_psdWaves.digitalProbe2 == PSDDP2_GateShort))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_GateShort;
				}
				if (ImGui::Selectable("Over Threshold", m_psdWaves.digitalProbe2 == PSDDP2_OverThreshold))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_OverThreshold;
				}
				if (ImGui::Selectable("PileUp", m_psdWaves.digitalProbe2 == PSDDP2_PileUp))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_PileUp;
				}
				if (ImGui::Selectable("Trigger", m_psdWaves.digitalProbe2 == PSDDP2_Trigger))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_Trigger;
				}
				if (ImGui::Selectable("Trig. Hold-Off", m_psdWaves.digitalProbe2 == PSDDP2_TriggerHoldoff))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_TriggerHoldoff;
				}
				if (ImGui::Selectable("Trig. Value", m_psdWaves.digitalProbe2 == PSDDP2_TriggerVal))
				{
					changed = true;
					m_psdWaves.digitalProbe2 = PSDDP2_TriggerVal;
				}
				ImGui::EndCombo();
			}

			ImGui::EndTable();
		}
		if (!m_digitizerEnabled)
			ImGui::EndDisabled();
		return changed;
	}
}