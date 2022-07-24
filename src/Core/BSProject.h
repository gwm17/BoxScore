#ifndef BS_PROJECT_H
#define BS_PROJECT_H

#include "DAQ/DigitizerDefs.h"

#include <mutex>
#include <filesystem>

namespace BoxScore {

	class BSProject
	{
	public:
		using Ref = std::shared_ptr<BSProject>;

		BSProject();
		~BSProject();

		void SetProjectPath(const std::filesystem::path& path);
		void SetDigitizerArgsList(const std::vector<DigitizerArgs>& args);
		void SetRunNumber(uint32_t number);
		void IncrementRunNumber();

		void PipeData(const std::vector<BSData>& data);

		const std::filesystem::path& GetProjectPath();
		const std::vector<DigitizerArgs>& GetDigitizerArgsList();
		uint32_t GetRunNumber();

	private:
		std::vector<DigitizerArgs> m_argList;
		std::filesystem::path m_projectPath;
		uint32_t m_runNumber;

		std::mutex m_projectMutex;
	};
}

#endif