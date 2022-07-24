#include "BSProject.h"

namespace BoxScore {

	BSProject::BSProject() :
		m_runNumber(0)
	{
	}

	BSProject::~BSProject() {}

	void BSProject::SetProjectPath(const std::filesystem::path& path)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_projectPath = path;
	}

	void BSProject::SetDigitizerArgsList(const std::vector<DigitizerArgs>& args)
	{
		std::scoped_lock<std::mutex> guard(m_projectMutex);
		m_argList = args;
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