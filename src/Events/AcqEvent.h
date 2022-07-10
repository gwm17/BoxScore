#ifndef ACQ_EVENT_H
#define ACQ_EVENT_H

#include "Event.h"

namespace BoxScore {

	//For now all pure message, not sure what exactly needed yet

	class AcqStartEvent : Event
	{
	public:
		AcqStartEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqStart);
	};

	class AcqStopEvent : Event
	{
	public:
		AcqStopEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqStop);
	};

	class AcqUpdateEvent : Event
	{
	public:
		AcqUpdateEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryAcq);
		EVENT_TYPE_SETUP(AcqUpdate);
	};
}

#endif