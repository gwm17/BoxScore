#ifndef DIGITIZER_PANEL_H
#define DIGITIZER_PANEL_H

#include "DAQ/DigitizerDefs.h"
#include "imgui.h"

namespace BoxScore {

	class DigitizerPanel
	{
	public:
		enum class Type
		{
			PHA,
			PSD,
			None
		};

		DigitizerPanel(const DigitizerArgs& args);
		~DigitizerPanel();

		void SetDigitizerParameters(const DigitizerParameters& params);
		void SetPHAParameters(const std::vector<PHAParameters>& ch_params, const PHAWaveParameters& wv_params);
		void SetPSDParameters(const std::vector<PSDParameters>& ch_params, const PSDWaveParameters& wv_params);

		//Retrieve settings
		const DigitizerParameters& GetDigitizerParameters() const { return m_params; }
		const std::vector<PHAParameters>& GetPHAChannelParameters() const { return m_phaChannels; }
		const std::vector<PSDParameters>& GetPSDChannelParameters() const { return m_psdChannels; }
		const PHAWaveParameters& GetPHAWaveParameters() const { return m_phaWaves; }
		const PSDWaveParameters& GetPSDWaveParameters() const { return m_psdWaves; }

		Type GetPanelType() const { return m_panelType; }
		int GetDigitizerHandle() { return m_args.handle; }

		bool OnImGuiRender();

	private:
		void Init();
		bool RenderDigitizerParams();
		bool RenderPHAParameters();
		bool RenderPSDParameters();
		bool RenderPHAWaveParameters();
		bool RenderPSDWaveParameters();

		DigitizerArgs m_args;
		DigitizerParameters m_params;
		std::vector<PHAParameters> m_phaChannels;
		std::vector<PSDParameters> m_psdChannels;
		PHAWaveParameters m_phaWaves;
		PSDWaveParameters m_psdWaves;
		Type m_panelType;

		std::string m_panelName;
		std::string m_settingsFile;

		bool m_digitizerEnabled;

		const ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Hideable;
	};

}

#endif