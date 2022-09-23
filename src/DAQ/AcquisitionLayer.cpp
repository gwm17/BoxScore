#include "AcquisitionLayer.h"
#include "Core/BoxScoreCore.h"
#include "Core/DataDistributor.h"

namespace BoxScore {

	static bool SortByHandle(const Digitizer::Ref& digi1, const Digitizer::Ref& digi2)
	{
		return digi1->GetDigitizerArgs().handle < digi2->GetDigitizerArgs().handle;
	}

	AcquisitionLayer::AcquisitionLayer(const BSProject::Ref& project) :
		m_project(project), m_acqThread(nullptr), m_running(false), m_server(52324)
	{
		m_server.PowerOn();
	}

	AcquisitionLayer::~AcquisitionLayer()
	{
		BS_INFO("Shutting down acq");
		if (m_running)
		{
			DestroyAcqThread();
		}
		BS_INFO("Finished");
		m_server.Shutdown();
	}

	void AcquisitionLayer::OnUpdate()
	{

	}

	void AcquisitionLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatch(e);

		dispatch.Dispatch<AcqStartEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqStartEvent));
		dispatch.Dispatch<AcqStopEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqStopEvent));
		dispatch.Dispatch<AcqParametersEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqParametersEvent));
		dispatch.Dispatch<AcqSyncArgsEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqSyncArgsEvent));
		dispatch.Dispatch<AcqDPPModeEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqDPPModeEvent));
		dispatch.Dispatch<AcqDetectBoardsEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqDetectBoardsEvent));
		dispatch.Dispatch<AcqDisconnectBoardsEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqDisconnectBoardsEvent));
	}

	void AcquisitionLayer::DestroyAcqThread()
	{
		m_running = false;
		if (m_acqThread != nullptr && m_acqThread->joinable())
		{
			BS_INFO("Destroying acquisition thread...");
			m_acqThread->join();
			delete m_acqThread;
			m_acqThread = nullptr;
			BS_INFO("Acquisition thread is destroyed.");
		}
		else if(m_acqThread != nullptr)
			BS_WARN("Unable to destroy acquisition thread, but is active");
	}

	bool AcquisitionLayer::OnAcqStartEvent(AcqStartEvent& e)
	{
		if (m_running)
		{
			BS_WARN("Attempted to start a new acquisition while one is already running!");
			return true;
		}
		else if (m_digitizerChain.empty())
		{
			BS_WARN("Cannot start acquisition without any digitizers!");
			return true;
		}
		else if (m_acqThread != nullptr)
		{
			DestroyAcqThread();
		}

		BS_INFO("Starting acquisition thread...");
		m_running = true;
		m_acqThread = new std::thread(&AcquisitionLayer::Run, std::ref(*this));
		BS_INFO("Running.");

		//If we chose to write to disk, start the file handler
		if (e.IsWriteToDisk())
			m_fileIO.StartRun(m_project);
		//If we chose to write to server start the server feed
		if (e.IsWriteToServer())
			m_server.StartDataFeed();
		
		return true;
	}

	bool AcquisitionLayer::OnAcqStopEvent(AcqStopEvent& e)
	{
		DestroyAcqThread();

		if (m_fileIO.IsRunning())
			m_fileIO.StopRun();
		if (m_server.IsFeeding())
			m_server.StopDataFeed();

		return true;
	}

	bool AcquisitionLayer::OnAcqParametersEvent(AcqParametersEvent& e)
	{
		if(m_running)
		{
			BS_WARN("Cannot change digitizer parameters while aquisition is running!");
			return true;
		}

		if (e.GetAccessType() == DigitizerAccessType::All)
		{
			for (Digitizer::Ref& digitizer : m_digitizerChain)
			{
				const DigitizerArgs& args = digitizer->GetDigitizerArgs();
				if (args.firmware == CAEN_DGTZ_DPPFirmware_PHA)
				{
					digitizer->SetDigitizerParameters(m_project->GetDigitizerParameters(args.handle));
					digitizer->SetChannelParameters(m_project->GetPHAParameters(args.handle));
					digitizer->SetWaveformParameters(m_project->GetPHAWaveParameters(args.handle));

					digitizer->LoadSettings();
				}
				else if (digitizer->GetDigitizerArgs().firmware == CAEN_DGTZ_DPPFirmware_PSD)
				{
					digitizer->SetDigitizerParameters(m_project->GetDigitizerParameters(args.handle));
					digitizer->SetChannelParameters(m_project->GetPSDParameters(args.handle));
					digitizer->SetWaveformParameters(m_project->GetPSDWaveParameters(args.handle));

					digitizer->LoadSettings();
				}
			}
		}
		else if(e.GetAccessType() == DigitizerAccessType::Single)
		{
			Digitizer::Ref& digitizer = m_digitizerChain[e.GetBoardHandle()];
			const DigitizerArgs& args = digitizer->GetDigitizerArgs();
			if (args.firmware == CAEN_DGTZ_DPPFirmware_PHA)
			{
				digitizer->SetDigitizerParameters(m_project->GetDigitizerParameters(args.handle));
				digitizer->SetChannelParameters(m_project->GetPHAParameters(args.handle));
				digitizer->SetWaveformParameters(m_project->GetPHAWaveParameters(args.handle));

				digitizer->LoadSettings();
			}
			else if (digitizer->GetDigitizerArgs().firmware == CAEN_DGTZ_DPPFirmware_PSD)
			{
				digitizer->SetDigitizerParameters(m_project->GetDigitizerParameters(args.handle));
				digitizer->SetChannelParameters(m_project->GetPSDParameters(args.handle));
				digitizer->SetWaveformParameters(m_project->GetPSDWaveParameters(args.handle));

				digitizer->LoadSettings();
			}
		}

		//Setting parameters can sometimes lead to CAEN modifying values (invalid value, etc), so inform the project of changes
		m_project->SetDigitizerData(m_digitizerChain);

		return true;
	}

	bool AcquisitionLayer::OnAcqSyncArgsEvent(AcqSyncArgsEvent& e)
	{
		if (m_running)
		{
			BS_WARN("Cannot update synchronization settings while acquisition is running!");
			return true;
		}

		m_digitizerChain.SynchronizeBoards(e.GetArgs());
		return true;
	}

	bool AcquisitionLayer::OnAcqDPPModeEvent(AcqDPPModeEvent& e)
	{
		if (m_running)
		{
			BS_WARN("Cannot update DPP Acquisition settings while aquisitiion is running!");
			return true;
		}

		m_digitizerChain.SetDPPAcqMode(m_project->GetDPPAcqMode());
		m_project->SetDigitizerData(m_digitizerChain);
		return true;
	}

	bool AcquisitionLayer::OnAcqDetectBoardsEvent(AcqDetectBoardsEvent& e)
	{
		BS_INFO("Querying the system for digitizers. WARNING: BoxScore currently only supports OpticalLink connections");
		m_digitizerChain.DetectBoards();
		//Tell the project what happened
		m_project->SetDigitizerData(m_digitizerChain); 

		return true;
	}

	bool AcquisitionLayer::OnAcqDisconnectBoardsEvent(AcqDisconnectBoardsEvent& e)
	{
		if (m_running)
		{
			BS_WARN("Cannot disconnect digitizers while the acquisition is running!");
			return true;
		}

		BS_INFO("Disconnecting digitizers...");

		m_digitizerChain.DisconnectBoards();

		BS_INFO("Digitizers disconnected.");

		//Tell the project what happened
		m_project->SetDigitizerData(m_digitizerChain);
		
		return true;
	}

	std::vector<DigitizerArgs> AcquisitionLayer::GetArgList()
	{
		std::vector<DigitizerArgs> list;
		list.reserve(m_digitizerChain.size());
		for (auto& digitizer : m_digitizerChain)
			list.push_back(digitizer->GetDigitizerArgs());
		return list;
	}

	void AcquisitionLayer::Run()
	{
		if (!m_digitizerChain.Start())
			return;

		std::vector<BSData> recievedData; //local data buffer
		//Run aquisition loop
		while (m_running)
		{
			for (auto& digitizer : m_digitizerChain)
				digitizer->ReadData(recievedData);

			if (recievedData.empty())
				continue;

			DataDistributor::PushData(recievedData);
			recievedData.clear();
		}

		m_digitizerChain.Stop();
	}
}