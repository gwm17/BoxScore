#ifndef DIGITIZER_CHAIN_H
#define DIGITIZER_CHAIN_H

#include "Digitizer.h"
#include "Events/AcqEvent.h"

namespace BoxScore {

	/*
		Wrapper around std::vector list of digitizers in a setup
	*/
	class DigitizerChain
	{
	public:
		DigitizerChain();
		~DigitizerChain();

		const Digitizer::Ref& operator[](std::size_t index) const
		{
			return m_chain[index];
		}

		Digitizer::Ref& operator[](std::size_t index)
		{
			return m_chain[index];
		}


		//Mimic stl
		std::size_t size() const { return m_chain.size(); }
		bool empty() const { return m_chain.empty(); }
		void clear() { m_chain.clear(); }

		void DetectBoards();
		void DisconnectBoards();
	
		void SetDPPAcqMode(DPPAcqMode mode);
		void SynchronizeBoards(const SyncArgs& e);

		bool Start();
		void Stop();

		std::vector<Digitizer::Ref>::iterator begin() { return m_chain.begin(); }
		std::vector<Digitizer::Ref>::iterator end() { return m_chain.end(); }

		std::vector<Digitizer::Ref>::const_iterator begin() const { return m_chain.begin(); }
		std::vector<Digitizer::Ref>::const_iterator end() const { return m_chain.end(); }

	private:
		bool StartSynchronizedRun();
		void StopSynchronizedRun();

		std::vector<Digitizer::Ref> m_chain;
		SyncArgs m_syncArgs;
	};
}

#endif