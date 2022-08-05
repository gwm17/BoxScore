#include "AcquisitionLayer.h"
#include "Core/BoxScoreCore.h"

namespace BoxScore {

	static bool SortByHandle(const Digitizer::Ref& digi1, const Digitizer::Ref& digi2)
	{
		return digi1->GetDigitizerArgs().handle < digi2->GetDigitizerArgs().handle;
	}

	AcquisitionLayer::AcquisitionLayer(const BSProject::Ref& project) :
		m_project(project), m_acqThread(nullptr), m_running(false)
	{
	}

	AcquisitionLayer::~AcquisitionLayer()
	{
		if (m_running)
		{
			DestroyAcqThread();
		}
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
		dispatch.Dispatch<AcqDetectBoardsEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqDetectBoardsEvent));
		dispatch.Dispatch<AcqDisconnectBoardsEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqDisconnectBoardsEvent));
	}
	
	void AcquisitionLayer::CreateAcqThread()
	{
		if (m_running)
		{
			BS_WARN("Attempted to start a new acquisition while one is already running!");
			return;
		}
		else if (m_digitizerChain.empty())
		{
			BS_WARN("Cannot start acquisition without any digitizers!");
			return;
		}
		else if (m_acqThread != nullptr)
		{
			DestroyAcqThread();
		}

		BS_INFO("Starting acquisition thread...");
		m_running = true;
		m_acqThread = new std::thread(&AcquisitionLayer::Run, std::ref(*this));
		BS_INFO("Running.");
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
		CreateAcqThread();
		return true;
	}

	bool AcquisitionLayer::OnAcqStopEvent(AcqStopEvent& e)
	{
		DestroyAcqThread();
		return true;
	}

	bool AcquisitionLayer::OnAcqParametersEvent(AcqParametersEvent& e)
	{
		if(m_running)
		{
			BS_WARN("Cannot change digitizer parameters while aquisition is running!");
			return true;
		}

		int handle = e.GetBoardHandle();
		Digitizer::Ref& digitizer = m_digitizerChain[handle];
		if(digitizer->GetDigitizerArgs().firmware == CAEN_DGTZ_DPPFirmware_PHA)
		{
			std::shared_ptr<DigitizerPHA> phaDigitizer = std::static_pointer_cast<DigitizerPHA>(digitizer);
			if(phaDigitizer == nullptr)
			{
				BS_ERROR("Digitizer data indicates PHA, but digitizer is not of type PHA in AcquisitionLayer::OnAcqParametersEvent!");
				return true;
			}
			phaDigitizer->SetDigitizerParameters(m_project->GetDigitizerParameterList()[handle]);
			phaDigitizer->SetChannelParameters(m_project->GetPHAParameters(handle));
			phaDigitizer->SetWaveformParameters(m_project->GetPHAWaveParameters(handle));

			phaDigitizer->LoadSettings();
		}
		else if(digitizer->GetDigitizerArgs().firmware == CAEN_DGTZ_DPPFirmware_PSD)
		{
			std::shared_ptr<DigitizerPSD> psdDigitizer = std::static_pointer_cast<DigitizerPSD>(digitizer);
			if(psdDigitizer == nullptr)
			{
				BS_ERROR("Digitizer data indicates PSD, but digitizer is not of type PSD in AcquisitionLayer::OnAcqParametersEvent!");
				return true;
			}
			psdDigitizer->SetDigitizerParameters(m_project->GetDigitizerParameterList()[handle]);
			psdDigitizer->SetChannelParameters(m_project->GetPSDParameters(handle));
			psdDigitizer->SetWaveformParameters(m_project->GetPSDWaveParameters(handle));

			psdDigitizer->LoadSettings();
		}

		return true;
	}

	bool AcquisitionLayer::OnAcqSyncArgsEvent(AcqSyncArgsEvent& e)
	{
		if (m_running)
		{
			BS_WARN("Cannot update synchronization settings while acquisition is running!");
			return true;
		}

		SetSynchronization(e.GetArgs());
		return true;
	}

	bool AcquisitionLayer::OnAcqDetectBoardsEvent(AcqDetectBoardsEvent& e)
	{
		BS_INFO("Querying the system for digitizers. WARNING: BoxScore currently only supports OpticalLink connections");

		m_digitizerChain.clear();

		static int nLinks = 4;
		static int nNodes = 8;

		DigitizerArgs args;
		for(int link=0; link<nLinks; link++)
		{
			for(int node=0; node<nNodes; node++)
			{
				args = DigitizerArgs();
				args.conetNode = node;
				args.linkNumber = link;
				Digitizer::Ref digi = OpenDigitizer(args);
				if(digi != nullptr)
				{
					args = digi->GetDigitizerArgs();
					m_digitizerChain.push_back(digi);
					BS_INFO("Found digitizer named {0} at link {1} on node {2}", args.name, args.linkNumber, args.conetNode);
				}
			}
		}

		if (m_digitizerChain.size() == 0)
			BS_WARN("No digitizers found... check to see that they are on and connected to the system via optical link");
		else
			std::sort(m_digitizerChain.begin(), m_digitizerChain.end(), SortByHandle);

		m_project->SetDigitizerArgsList(GetArgList());

		//Emit back out the argument list so that the editor gets updated
		AcqBoardsFoundEvent bf_event;
		m_callbackFunction(bf_event);

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

		m_digitizerChain.clear();

		BS_INFO("Digitizers disconnected.");

		m_project->SetDigitizerArgsList(GetArgList());

		//Emit back out the argument list so that the editor gets updated
		AcqBoardsFoundEvent bf_event;
		m_callbackFunction(bf_event);
		
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

	void AcquisitionLayer::SetSynchronization(const SyncArgs& args)
	{
		if (m_digitizerChain.size() <= 1)
		{
			BS_WARN("Cannot synchronize digitizer chain of size {0}", m_digitizerChain.size());
			return;
		}

		int error_code = SetChainSynchronize(args, m_digitizerChain);
		if (error_code != CAEN_DGTZ_Success)
		{
			BS_ERROR("Unable to synchronize digitizers, CAEN reports error {0}", error_code);
			return;
		}

		m_project->SetDigitizerData(m_digitizerChain);
		m_syncStatus = args;
	}

	bool AcquisitionLayer::StartDigitizers()
	{
		//Start digitizers
		if (m_digitizerChain.size() == 0)
		{
			BS_ERROR("Cannot run data aquisition without any digitizers!");
			return false;
		}
		else if (m_digitizerChain.size() == 1)
		{
			m_digitizerChain[0]->StartAquisition();
			if (!m_digitizerChain[0]->IsActive())
			{
				BS_ERROR("Unable to start single digitizer config!");
				return false;
			}
		}
		else
		{
			int ec = StartSynchronizedRun(m_syncStatus, m_digitizerChain);
			if (ec != CAEN_DGTZ_Success)
			{
				BS_ERROR("Unable to start synchronized digitizer chain");
				return false;
			}
		}

		return true;
	}

	bool AcquisitionLayer::StopDigitizers()
	{
		//Stop digitizers
		if (m_digitizerChain.size() == 0)
		{
			BS_ERROR("How the hell did you get here?");
			return false;
		}
		else if (m_digitizerChain.size() == 1)
		{
			m_digitizerChain[0]->StopAquisition();
			if (m_digitizerChain[0]->IsActive())
			{
				BS_ERROR("Single digitizer stop failed...");
				return false;
			}
		}
		else
		{
			int ec = StopSynchronizedRun(m_syncStatus, m_digitizerChain);
			if (ec != CAEN_DGTZ_Success)
			{
				BS_ERROR("Failed to stop digitizer chain...");
				return false;
			}
		}

		return true;
	}

	void AcquisitionLayer::Run()
	{
		if (!StartDigitizers())
			return;

		std::vector<BSData> recievedData;
		
		//Run aquisition loop
		while (m_running)
		{
			for (auto& digitizer : m_digitizerChain)
			{
				recievedData = digitizer->ReadData();
				if (recievedData.empty())
					continue;

				//Do some stuff with data...
				m_project->PipeData(recievedData);
			}
		}

		StopDigitizers();
	}
}