#ifndef EVENT_H
#define EVENT_H

#include "Core/BoxScoreCore.h"

namespace BoxScore {

	enum class EventType
	{
		None=0,
		WindowClose,WindowResize,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved,
		AppUpdate,
		AcqStart, AcqStop, AcqUpdate, AcqDetectBoards
	};

	enum EventCategory
	{
		EventCategoryNone=0,
		EventCategoryKey = BIT(1),
		EventCategoryMouse = BIT(2),
		EventCategoryApp = BIT(3),
		EventCategoryAcq = BIT(4),
		EventCategoryInput = BIT(5)
	};

	//Some ease of code generation macros
	#define EVENT_CATEGORY_SETUP(cat) virtual int GetCategoryFlags() const override { return cat; }

	#define EVENT_TYPE_SETUP(type) static EventType GetStaticType() { return EventType::type; } \
								   virtual EventType GetEventType() const override { return GetStaticType(); } \
								   virtual const char* GetName() const override { return #type; }

	//Event base class
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		bool IsCategory(EventCategory cat) const { return cat & GetCategoryFlags(); }
		bool handled = false;
	};

	//Class which wraps event messaging.
	//When an event is created, a dispatcher is also created. The dispatcher takes the created event,
	//and passes it to handling callbacks via the Dispatch function. If the event is handled, Dispatch returns true.
	//If one wants the event to only be handled by a single case, have the callback return true to set the event internal status as handled.
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e) :
			m_event(e)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& function)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.handled = function(static_cast<T&>(m_event));
				return true;
			}
			else
				return false;
		}
	private:
		Event& m_event;
	};

	//So that events can be logged easily
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

#endif