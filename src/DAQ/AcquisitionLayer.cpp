#include "AcquisitionLayer.h"
#include "Core/BoxScoreCore.h"

namespace BoxScore {

	AcquisitionLayer::AcquisitionLayer() :
		m_acqThread(nullptr), m_running(false)
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
		dispatch.Dispatch<AcqUpdateEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqUpdateEvent));
		dispatch.Dispatch<AcqDetectBoardsEvent>(BIND_EVENT_FUNCTION(AcquisitionLayer::OnAcqDetectBoardsEvent));
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

	//TODO: Need to handle modifications to the acquisition.
	bool AcquisitionLayer::OnAcqUpdateEvent(AcqUpdateEvent& e)
	{
		return true;
	}

	//TODO: Need to handle board detection
	bool AcquisitionLayer::OnAcqDetectBoardsEvent(AcqDetectBoardsEvent& e)
	{
		return true;
	}

	void AcquisitionLayer::Run()
	{
		
		while (m_running)
		{
			//Do the acq stuff hahaha
		}

	}
}