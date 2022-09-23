#ifndef BS_PROJECT_H
#define BS_PROJECT_H

#include "DAQ/DigitizerDefs.h"
#include "DAQ/Digitizer.h"
#include "DAQ/DigitizerChain.h"

#include <filesystem>

namespace BoxScore {

	class BSProject
	{
	public:
		using Ref = std::shared_ptr<BSProject>;

		BSProject();
		~BSProject();

		bool SetProjectPath(const std::filesystem::path& path);
		//void SetDigitizerData(const std::vector<Digitizer::Ref>& chain);
		void SetDigitizerData(const DigitizerChain& chain);

		void SetDigitizerArgsList(const std::vector<DigitizerArgs>& args);
		void SetDigitizerParameterList(const std::vector<DigitizerParameters>& params);
		void SetDigitizerParameters(int handle, const DigitizerParameters& params);

		void SetPHAParameters(int handle, const std::vector<PHAParameters>& params);
		void SetPSDParameters(int handle, const std::vector<PSDParameters>& params);
		void SetPHAWaveParameters(int handle, const PHAWaveParameters& params);
		void SetPSDWaveParameters(int handle, const PSDWaveParameters& params);

		void SetRunNumber(uint32_t number);
		void IncrementRunNumber();
		void SetDPPAcqMode(DPPAcqMode mode);

		const std::filesystem::path& GetProjectPath();

		const std::vector<DigitizerArgs>& GetDigitizerArgsList();
		DigitizerArgs GetDigitizerArgs(int handle);
		const std::vector<DigitizerParameters>& GetDigitizerParameterList();
		DigitizerParameters GetDigitizerParameters(int handle);

		const std::vector<PHAParameters>& GetPHAParameters(int handle);
		const std::vector<PSDParameters>& GetPSDParameters(int handle);
		PHAWaveParameters GetPHAWaveParameters(int handle);
		PSDWaveParameters GetPSDWaveParameters(int handle);

		uint32_t GetRunNumber();
		DPPAcqMode GetDPPAcqMode();
		size_t GetNumberOfBoards();

		std::filesystem::path CreateRunDirectory();

	private:
		void InternalClear();

		std::vector<DigitizerArgs> m_argList;
		std::vector<DigitizerParameters> m_boardParamList;
		std::unordered_map<int, std::vector<PHAParameters>> m_phaChannelMap;
		std::unordered_map<int, std::vector<PSDParameters>> m_psdChannelMap;
		std::unordered_map<int, PHAWaveParameters> m_phaWaveMap;
		std::unordered_map<int, PSDWaveParameters> m_psdWaveMap;
		std::filesystem::path m_projectPath;
		uint32_t m_runNumber;
		DPPAcqMode m_dppMode;

		//some null result stuff
		std::vector<PHAParameters> m_nullphaChannels;
		std::vector<PSDParameters> m_nullpsdChannels;
	};
}

#endif