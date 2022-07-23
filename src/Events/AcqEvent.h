#ifndef ACQ_EVENT_H
#define ACQ_EVENT_H

#include "Event.h"
#include "DAQ/DigitizerDefs.h"
#include "DAQ/Synchronize.h"

namespace BoxScore {

	//For now all pure message, not sure what exactly needed yet

	class AcqStartEvent : public Event
	{
	public:
		AcqStartEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqStart);
	};

	class AcqStopEvent : public Event
	{
	public:
		AcqStopEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqStop);
	};

	class AcqPHAParametersEvent : public Event
	{
	public:
		AcqPHAParametersEvent(int board, const DigitizerParameters& boardParams, const std::vector<PHAParameters>& channelParams, const PHAWaveParameters& waveParams) :
			m_board(board), m_wholeBoardParams(boardParams), m_channelParams(channelParams), m_waveParams(waveParams)
		{
		}

		int GetBoard() const { return m_board; }
		const DigitizerParameters& GetDigitizerParams() const { return m_wholeBoardParams; }
		const std::vector<PHAParameters>& GetChannelParams() const { return m_channelParams; }
		const PHAWaveParameters& GetWaveformParams() const { return m_waveParams; }

		virtual std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << " board: " << m_board;
			return stream.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqPHAParameters);

	private:
		int m_board;
		DigitizerParameters m_wholeBoardParams;
		std::vector<PHAParameters> m_channelParams;
		PHAWaveParameters m_waveParams;
	};

	class AcqPSDParametersEvent : public Event
	{
	public:
		AcqPSDParametersEvent(int board, const DigitizerParameters& boardParams, const std::vector<PSDParameters>& channelParams, const PSDWaveParameters& waveParams) :
			m_board(board), m_wholeBoardParams(boardParams), m_channelParams(channelParams), m_waveParams(waveParams)
		{
		}

		int GetBoard() const { return m_board; }
		const DigitizerParameters& GetDigitizerParams() const { return m_wholeBoardParams; }
		const std::vector<PSDParameters>& GetChannelParams() const { return m_channelParams; }
		const PSDWaveParameters& GetWaveformParams() const { return m_waveParams; }

		virtual std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << " board: " << m_board;
			return stream.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqPHAParameters);

	private:
		int m_board;
		DigitizerParameters m_wholeBoardParams;
		std::vector<PSDParameters> m_channelParams;
		PSDWaveParameters m_waveParams;
	};

	class AcqSyncArgsEvent : public Event
	{
	public:
		AcqSyncArgsEvent(SyncArgs& args) :
			m_args(args)
		{
		}

		const SyncArgs& GetArgs() const { return m_args; }
		
		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqSyncArgs);

	private:
		SyncArgs m_args;
	};

	class AcqDetectBoardsEvent : public Event
	{
	public:
		AcqDetectBoardsEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqDetectBoards);
	};
}

#endif