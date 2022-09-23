#include "DigitizerChain.h"
#include "CAENDigitizer.h"

namespace BoxScore {

	static bool SortByHandle(const Digitizer::Ref& digi1, const Digitizer::Ref& digi2)
	{
		return digi1->GetDigitizerArgs().handle < digi2->GetDigitizerArgs().handle;
	}

	DigitizerChain::DigitizerChain()
	{
	}

	DigitizerChain::~DigitizerChain()
	{
	}

	void DigitizerChain::DetectBoards()
	{
		m_chain.clear();

		static int nLinks = 4;
		static int nNodes = 8;

		DigitizerArgs args;
		for (int link = 0; link < nLinks; link++)
		{
			for (int node = 0; node < nNodes; node++)
			{
				args = DigitizerArgs();
				args.conetNode = node;
				args.linkNumber = link;
				Digitizer::Ref digi = OpenDigitizer(args);
				if (digi != nullptr)
				{
					args = digi->GetDigitizerArgs();
					m_chain.push_back(digi);
					BS_INFO("Found digitizer named {0} at link {1} on node {2}", args.name, args.linkNumber, args.conetNode);
				}
			}
		}

		if (size() == 0)
			BS_WARN("No digitizers found... check to see that they are on and connected to the system via optical link");
		else
			std::sort(begin(), end(), SortByHandle); //in general seems to not be necessary, but just to be safe
	}

	void DigitizerChain::DisconnectBoards()
	{
		//Maybe someday more complicated stuff here?
		clear();
	}

	void DigitizerChain::SetDPPAcqMode(DPPAcqMode mode)
	{
		CAEN_DGTZ_DPP_AcqMode_t modeIn;
		CAEN_DGTZ_DPP_AcqMode_t modeOut;
		CAEN_DGTZ_DPP_SaveParam_t saveIn = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime;
		CAEN_DGTZ_DPP_SaveParam_t saveOut;

		switch (mode)
		{
		case DPPAcqMode::List: modeIn = CAEN_DGTZ_DPP_ACQ_MODE_List; break;
		case DPPAcqMode::Waves: modeIn = CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope; break;
		case DPPAcqMode::None: BS_WARN("Attempted to set DPPAcqMode::None!"); return;
		}

		int ec = 0;
		for (auto& digitizer : m_chain)
		{
			ec |= CAEN_DGTZ_SetDPPAcquisitionMode(digitizer->GetDigitizerArgs().handle, modeIn, saveIn);
			ec |= CAEN_DGTZ_GetDPPAcquisitionMode(digitizer->GetDigitizerArgs().handle, &modeOut, &saveOut);
			DigitizerParameters params = digitizer->GetDigitizerParameters();
			params.dppAcqMode = modeOut;
			params.dppSaveMode = saveOut;
			digitizer->SetDigitizerParameters(params);
		}
		if (ec != 0)
		{
			BS_ERROR("Error code {0} recieved from DigitizerChain::SetDPPAcqMode!", ec);
		}
	}

	void DigitizerChain::SynchronizeBoards(const SyncArgs& args)
	{
		if (size() <= 1)
		{
			BS_WARN("Cannot synchronize digitizer chain of size {0}", size());
			return;
		}

		CAEN_DGTZ_RunSyncMode_t value = CAEN_DGTZ_RUN_SYNC_Disabled;
		CAEN_DGTZ_RunSyncMode_t setSyncValue;
		CAEN_DGTZ_AcqMode_t setAcqMode;

		switch (args.syncMethod)
		{
		case SyncMethod::SIn_TrigOut: value = CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain; break;
		case SyncMethod::None: value = CAEN_DGTZ_RUN_SYNC_Disabled; break;
		}

		int ec = 0;
		for (auto& digitizer : m_chain)
		{
			ec |= CAEN_DGTZ_SetRunSynchronizationMode(digitizer->GetDigitizerArgs().handle, value);
			ec |= CAEN_DGTZ_GetRunSynchronizationMode(digitizer->GetDigitizerArgs().handle, &setSyncValue);
			ec |= CAEN_DGTZ_GetAcquisitionMode(digitizer->GetDigitizerArgs().handle, &setAcqMode);
			auto params = digitizer->GetDigitizerParameters();
			params.syncMode = setSyncValue;
			params.acqMode = setAcqMode;
			digitizer->SetDigitizerParameters(params);
		}
		if (ec != 0)
		{
			BS_ERROR("Error code {0} recieved from DigitizerChain::SynchronizeBoards!", ec);
		}
	}

	bool DigitizerChain::Start()
	{
		if (size() == 0)
		{
			BS_ERROR("Cannot run data aquisition without any digitizers!");
			return false;
		}
		else if (size() == 1)
		{
			m_chain[0]->StartAquisition();
			if (!m_chain[0]->IsActive())
			{
				BS_ERROR("Unable to start single digitizer config!");
				return false;
			}
		}
		else
			return StartSynchronizedRun();

		return true;
	}

	void DigitizerChain::Stop()
	{
		if (size() == 0)
		{
			BS_ERROR("How the hell did you get here?");
			return;
		}
		else if (size() == 1)
		{
			m_chain[0]->StopAquisition();
			if (m_chain[0]->IsActive())
			{
				BS_ERROR("Single digitizer stop failed...");
				return;
			}
		}
		else
			StopSynchronizedRun();
	}

	bool DigitizerChain::StartSynchronizedRun()
	{
		static uint32_t softwareTrigger = 0x4;
		static uint32_t hardwareLevelTrigger = 0x5;

		if (m_syncArgs.syncMethod != SyncMethod::SIn_TrigOut)
		{
			BS_WARN("Unsupported sync method {0} at StartSynchronizedRun!", SyncMethodToString(m_syncArgs.syncMethod));
			return false;
		}

		int error_code = 0;
		switch (m_syncArgs.startType)
		{
		case SyncStartType::HardwareControlled:
		{
			error_code |= CAEN_DGTZ_WriteRegister(m_chain[0]->GetDigitizerArgs().handle, CAEN_DGTZ_ACQ_CONTROL_ADD, softwareTrigger);
			break;
		}
		case SyncStartType::SoftwareControlled:
		{
			error_code |= CAEN_DGTZ_WriteRegister(m_chain[0]->GetDigitizerArgs().handle, CAEN_DGTZ_ACQ_CONTROL_ADD, hardwareLevelTrigger);
			break;
		}
		}

		if (error_code != 0)
		{
			BS_ERROR("Error code {0} recieved from DigitizerChain::StartSynchronizedRun!", error_code);
			return false;
		}
		return true;
	}

	void DigitizerChain::StopSynchronizedRun()
	{
		if (m_syncArgs.syncMethod != SyncMethod::SIn_TrigOut)
		{
			BS_WARN("Unsupported sync method {0} at StartSynchronizedRun!", SyncMethodToString(m_syncArgs.syncMethod));
			return;
		}

		int ec = CAEN_DGTZ_SWStopAcquisition(m_chain[0]->GetDigitizerArgs().handle); //Seems pretty universal, for all methods 0 indicates stop
		if(ec != 0)
			BS_ERROR("Error code {0} recieved from DigitizerChain::StopSynchronizedRun!", ec);
	}
}