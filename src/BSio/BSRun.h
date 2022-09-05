#ifndef BS_RUN_H
#define BS_RUN_H

#include "BSFile.h"
#include "Core/BSProject.h"

#include <atomic>
#include <thread>

namespace BoxScore {

	class BSRun
	{
	public:
		BSRun();
		~BSRun();

		void StartRun(const BSProject::Ref& project);
		void StopRun();

	private:
		void InitFiles(const BSProject::Ref& project);
		void ProcessData();
		
		std::unordered_map<uint32_t, BSFile> m_files;
		std::uint64_t m_ringConsumerID;
		std::atomic<bool> m_isRunning;
		std::thread* m_processingThread;
	};
}

#endif