#ifndef APP_EVENT_H
#define APP_EVENT_H

#include "Event.h"

namespace BoxScore {

	//Pure message, no data transferred
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryApp);
		EVENT_TYPE_SETUP(WindowClose);
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height) :
			m_width(width), m_height(height)
		{
		}

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

		virtual std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << "(" << m_width << ", " << m_height << ")";
			return stream.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryApp);
		EVENT_TYPE_SETUP(WindowResize);

	private:
		int m_width, m_height;
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryApp);
		EVENT_TYPE_SETUP(AppUpdate);
	};
}

#endif