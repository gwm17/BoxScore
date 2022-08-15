#include "BSProject.h"

namespace BoxScore {

	BSProject::BSProject() :
		m_runNumber(0), m_dppMode(DPPAcqMode::List)
	{
	}

	BSProject::~BSProject() {}

	void BSProject::InternalClear()
	{
		m_argList.clear();
		m_boardParamList.clear();
		m_psdChannelMap.clear();
		m_phaChannelMap.clear();
		m_psdWaveMap.clear();
		m_phaWaveMap.clear();
	}

	bool BSProject::SetProjectPath(const std::filesystem::path& path)
	{
		bool status = true;
		m_projectPath = path.lexically_normal();
		if (!std::filesystem::exists(m_projectPath))
		{
			status = std::filesystem::create_directory(path);
			if (!status)
				m_projectPath.clear();
		}
		return status;
	}

	void BSProject::SetDigitizerData(const std::vector<Digitizer::Ref>& chain)
	{
		InternalClear();

		for (auto& digitizer : chain)
		{
			auto& args = digitizer->GetDigitizerArgs();
			m_argList.push_back(args);
			m_boardParamList.push_back(digitizer->GetDigitizerParameters());
			if (args.firmware == CAEN_DGTZ_DPPFirmware_PHA)
			{
				m_phaChannelMap[args.handle] = std::static_pointer_cast<DigitizerPHA>(digitizer)->GetChannelParameters();
				m_phaWaveMap[args.handle] = std::static_pointer_cast<DigitizerPHA>(digitizer)->GetWaveformParameters();
			}
			else if(args.firmware == CAEN_DGTZ_DPPFirmware_PSD)
			{
				m_psdChannelMap[args.handle] = std::static_pointer_cast<DigitizerPSD>(digitizer)->GetChannelParameters();
				m_psdWaveMap[args.handle] = std::static_pointer_cast<DigitizerPSD>(digitizer)->GetWaveformParameters();
			}
			else
			{
				BS_ERROR("Invalid digitizer firmware detected at BSProject::SetDigitizerData!");
			}
		}
	}

	void BSProject::SetDigitizerArgsList(const std::vector<DigitizerArgs>& args)
	{
		m_argList = args;
	}

	void BSProject::SetDigitizerParameterList(const std::vector<DigitizerParameters>& params)
	{
		m_boardParamList = params;
	}

	void BSProject::SetDigitizerParameters(int handle, const DigitizerParameters& params)
	{
		if (handle >= m_boardParamList.size() || handle == -1)
		{
			BS_ERROR("Attempting to set digitizer parameters for non-extant board: given handle {0}, number of boards {1}", handle, m_boardParamList.size());
			return;
		}

		m_boardParamList[handle] = params;
	}

	void BSProject::SetPHAParameters(int handle, const std::vector<PHAParameters>& params)
	{
		m_phaChannelMap[handle] = params;
	}

	void BSProject::SetPSDParameters(int handle, const std::vector<PSDParameters>& params)
	{
		m_psdChannelMap[handle] = params;
	}

	void BSProject::SetPHAWaveParameters(int handle, const PHAWaveParameters& params)
	{
		m_phaWaveMap[handle] = params;
	}

	void BSProject::SetPSDWaveParameters(int handle, const PSDWaveParameters& params)
	{
		m_psdWaveMap[handle] = params;
	}

	void BSProject::SetRunNumber(uint32_t number)
	{
		m_runNumber = number;
	}

	void BSProject::IncrementRunNumber()
	{
		++m_runNumber;
	}

	void BSProject::SetDPPAcqMode(DPPAcqMode mode)
	{
		m_dppMode = mode;
	}

	const std::filesystem::path& BSProject::GetProjectPath()
	{
		return m_projectPath;
	}

	const std::vector<DigitizerArgs>& BSProject::GetDigitizerArgsList()
	{
		return m_argList;
	}

	DigitizerArgs BSProject::GetDigitizerArgs(int handle)
	{
		if (handle >= m_argList.size() || handle == -1)
		{
			BS_ERROR("Attempting to get digitizer args for non-extant board: given handle {0}, number of boards {1}", handle, m_argList.size());
			return DigitizerArgs();
		}
		return m_argList[handle];
	}

	const std::vector<DigitizerParameters>& BSProject::GetDigitizerParameterList()
	{
		return m_boardParamList;
	}

	DigitizerParameters BSProject::GetDigitizerParameters(int handle)
	{
		if (handle >= m_boardParamList.size() || handle == -1)
		{
			BS_ERROR("Attempting to get digitizer parameters for non-extant board: given handle {0}, number of boards {1}", handle, m_boardParamList.size());
			return DigitizerParameters();
		}
		return m_boardParamList[handle];
	}

	const std::vector<PHAParameters>& BSProject::GetPHAParameters(int handle)
	{
		auto iter = m_phaChannelMap.find(handle);
		if (iter != m_phaChannelMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return m_nullphaChannels;
	}
	
	const std::vector<PSDParameters>& BSProject::GetPSDParameters(int handle)
	{
		auto iter = m_psdChannelMap.find(handle);
		if (iter != m_psdChannelMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return m_nullpsdChannels;
	}

	PHAWaveParameters BSProject::GetPHAWaveParameters(int handle)
	{
		auto iter = m_phaWaveMap.find(handle);
		if (iter != m_phaWaveMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return PHAWaveParameters();
	}

	PSDWaveParameters BSProject::GetPSDWaveParameters(int handle)
	{
		auto iter = m_psdWaveMap.find(handle);
		if (iter != m_psdWaveMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return PSDWaveParameters();
	}

	uint32_t BSProject::GetRunNumber()
	{
		return m_runNumber;
	}

	DPPAcqMode BSProject::GetDPPAcqMode()
	{
		return m_dppMode;
	}

	size_t BSProject::GetNumberOfBoards()
	{
		return m_argList.size();
	}

}