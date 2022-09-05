#include "BSRun.h"
#include "Core/RingBuffer.h"

namespace BoxScore {

	//Maps two uint32_t's to a single uint32_t
	static uint32_t SzudzikPairingFunction(uint32_t v1, uint32_t v2)
	{
		return v1 >= v2 ? (v1 * v1 + v1 + v2) : (v2 * v2 + v1);
	}

	BSRun::BSRun() :
		m_ringConsumerID(-1), m_isRunning(false), m_processingThread(nullptr)
	{
	}

	BSRun::~BSRun()
	{
		StopRun();
	}

	void BSRun::InitFiles(const BSProject::Ref& project)
	{
		std::filesystem::path runPath = project->CreateRunDirectory();

		const std::vector<DigitizerArgs>& argList = project->GetDigitizerArgsList();

		std::string filename;
		uint32_t key;
		for (const auto& args : argList)
		{
			for (int i = 0; i < args.channels; i++)
			{
				filename = args.name + "_CH_" + std::to_string(i) + ".bsbin";
				key = SzudzikPairingFunction(args.handle, i);
				m_files.insert_or_assign(key, BSFile(runPath / filename));
				if (!(m_files[key].IsOpen()))
				{
					BS_WARN("File named {0} could not be opened!", filename);
				}
			}
		}
	}

	void BSRun::StartRun(const BSProject::Ref& project)
	{
		if (m_isRunning)
			StopRun();

		m_ringConsumerID = RingBuffer::Attach();
		InitFiles(project);
		m_isRunning = true;

		m_processingThread = new std::thread(&BSRun::ProcessData, std::ref(*this));
	}

	void BSRun::StopRun()
	{
		m_isRunning = false;
		if (m_processingThread != nullptr && m_processingThread->joinable())
		{
			m_processingThread->join();
			delete m_processingThread;
			m_processingThread = nullptr;
			RingBuffer::Detach(m_ringConsumerID); 

			//Close all files and clear the map to be ready for re-use
			for (auto& fileIter : m_files)
				fileIter.second.Close();
			m_files.clear();
		}
		else
			BS_WARN("Unable to destroy the processing thread for file writing!");
	}

	void BSRun::ProcessData()
	{
		std::vector<BSData> dataBuffer;
		uint32_t key;
		while (m_isRunning)
		{
			if (RingBuffer::PopData(m_ringConsumerID, dataBuffer))
			{
				for (auto& datum : dataBuffer)
				{
					key = SzudzikPairingFunction(datum.board, datum.channel);
					m_files[key].Write(datum);
				}
			}
		}
	}
}