#ifndef ACQ_EVENT_H
#define ACQ_EVENT_H

#include "Event.h"
#include "DAQ/DigitizerDefs.h"

namespace BoxScore {

	//For now all pure message, not sure what exactly needed yet

	class AcqStartEvent : public Event
	{
	public:
		AcqStartEvent(bool writeToDisk, bool writeToServer) :
			m_isWriteToDisk(writeToDisk), m_isWriteToServer(writeToServer)
		{
		}

		bool IsWriteToDisk() const { return m_isWriteToDisk; }
		bool IsWriteToServer() const { return m_isWriteToServer; }

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqStart);

	private:
		bool m_isWriteToDisk;
		bool m_isWriteToServer;
	};

	class AcqStopEvent : public Event
	{
	public:
		AcqStopEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqStop);
	};

	class AcqParametersEvent : public Event
	{
	public:
		AcqParametersEvent(DigitizerAccessType type, int boardHandle = -1) :
			m_boardHandle(boardHandle)
		{
		}

		int GetBoardHandle() { return m_boardHandle; }
		DigitizerAccessType GetAccessType() { return m_type; }

		virtual std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << " board handle: " << m_boardHandle;
			return stream.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqParameters);

	private:
		int m_boardHandle;
		DigitizerAccessType m_type;
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

	class AcqDisconnectBoardsEvent : public Event
	{
	public:
		AcqDisconnectBoardsEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqDisconnectBoards);
	};

	class AcqDPPModeEvent : public Event
	{
	public:
		AcqDPPModeEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqDPPMode);

	};
}

#endif