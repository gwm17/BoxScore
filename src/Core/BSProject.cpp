#include "BSProject.h"

namespace BoxScore {

	BSProject::BSProject() :
		m_runNumber(0)
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

	void BSProject::SetProjectPath(const std::filesystem::path& path)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_projectPath = path;
	}

	void BSProject::SetDigitizerData(const std::vector<Digitizer::Ref>& chain)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		InternalClear();

		for (auto& digitizer : chain)
		{
			auto& args = digitizer->GetDigitizerArgs();
			m_argList.push_back(args);
			m_boardParamList.push_back(digitizer->GetDigitzerParameters());
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
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_argList = args;
	}

	void BSProject::SetDigitizerParameterList(const std::vector<DigitizerParameters>& params)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_boardParamList = params;
	}

	void BSProject::SetPHAParameters(int handle, const std::vector<PHAParameters>& params)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_phaChannelMap[handle] = params;
	}

	void BSProject::SetPSDParameters(int handle, const std::vector<PSDParameters>& params)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_psdChannelMap[handle] = params;
	}

	void BSProject::SetPHAWaveParameters(int handle, const PHAWaveParameters& params)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_phaWaveMap[handle] = params;
	}

	void BSProject::SetPSDWaveParameters(int handle, const PSDWaveParameters& params)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_psdWaveMap[handle] = params;
	}

	void BSProject::SetRunNumber(uint32_t number)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_runNumber = number;
	}

	void BSProject::IncrementRunNumber()
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		++m_runNumber;
	}

	const std::filesystem::path& BSProject::GetProjectPath()
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		return m_projectPath;
	}

	const std::vector<DigitizerArgs>& BSProject::GetDigitizerArgsList()
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		return m_argList;
	}

	const std::vector<DigitizerParameters>& BSProject::GetDigitizerParameterList()
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		return m_boardParamList;
	}

	const std::vector<PHAParameters>& BSProject::GetPHAParameters(int handle)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		auto iter = m_phaChannelMap.find(handle);
		if (iter != m_phaChannelMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return m_nullphaChannels;
	}
	
	const std::vector<PSDParameters>& BSProject::GetPSDParameters(int handle)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		auto iter = m_psdChannelMap.find(handle);
		if (iter != m_psdChannelMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return m_nullpsdChannels;
	}

	PHAWaveParameters BSProject::GetPHAWaveParameters(int handle)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		auto iter = m_phaWaveMap.find(handle);
		if (iter != m_phaWaveMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return PHAWaveParameters();
	}

	PSDWaveParameters BSProject::GetPSDWaveParameters(int handle)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		auto iter = m_psdWaveMap.find(handle);
		if (iter != m_psdWaveMap.end())
			return iter->second;
		BS_ERROR("Invalid handle {0} given to BSProject::GetPHAParameters!", handle);
		return PSDWaveParameters();
	}

	uint32_t BSProject::GetRunNumber()
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		return m_runNumber;
	}

	void BSProject::PipeData(const std::vector<BSData>& data)
	{
		//Figure out what to do here hahaha
		return;
	}
}