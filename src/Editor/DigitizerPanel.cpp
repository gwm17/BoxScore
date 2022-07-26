#include "DigitizerPanel.h"

#include "imgui.h"

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
			ImGui::Text("Settings file: ");
			ImGui::SameLine();
			ImGui::Text(m_settingsFile.c_str());
			ImGui::Button("Open...");
			ImGui::SameLine();
			ImGui::Button("Save...");

			ImGui::EndTabItem();
		}

		return changed;
	}
}