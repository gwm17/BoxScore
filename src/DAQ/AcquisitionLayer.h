#ifndef ACQUISITION_LAYER_H
#define ACQUISITION_LAYER_H

#include "Core/Layer.h"
#include "Events/AcqEvent.h"
#include "Digitizer.h"
#include "Synchronize.h"
#include "Core/BSProject.h"

#include <thread>
#include <mutex>
#include <atomic>

namespace BoxScore {

	class AcquisitionLayer : public Layer
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		AcquisitionLayer(const BSProject::Ref& project);
		~AcquisitionLayer();

		void SetEventCallback(const EventCallbackFunc& func) { m_callbackFunction = func; }

		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;

		bool IsRunning() { return m_running; }

	private:
		//Event handlers
		bool OnAcqStartEvent(AcqStartEvent& e);
		bool OnAcqStopEvent(AcqStopEvent& e);
		bool OnAcqDetectBoardsEvent(AcqDetectBoardsEvent& e);
		bool OnAcqPHAParametersEvent(AcqPHAParametersEvent& e);
		bool OnAcqPSDParametersEvent(AcqPSDParametersEvent& e);
		bool OnAcqSyncArgsEvent(AcqSyncArgsEvent& e);
		bool OnAcqDisconnectBoardsEvent(AcqDisconnectBoardsEvent& e);

		//Functionality
		void CreateAcqThread();
		void DestroyAcqThread();
		void SetSynchronization(const SyncArgs& args);
		bool StartDigitizers();
		bool StopDigitizers();
		std::vector<DigitizerArgs> GetArgList();

		//Acquistion loop
		void Run();

		EventCallbackFunc m_callbackFunction;

		BSProject::Ref m_project;

		std::vector<Digitizer::Ref> m_digitizerChain;
		SyncArgs m_syncStatus;
		
		std::thread* m_acqThread;
		std::mutex m_acqMutex;
		std::atomic<bool> m_running;
	};
}

#endif