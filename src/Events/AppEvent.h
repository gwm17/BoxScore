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
		WindowResizeEvent(uint32_t width, uint32_t height) :
			m_width(width), m_height(height)
		{
		}

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

		virtual std::string ToString() const override
		{
			std::stringstream stream;
			stream << GetName() << "(" << m_width << ", " << m_height << ")";
			return stream.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryApp);
		EVENT_TYPE_SETUP(WindowResize);

	private:
		uint32_t m_width, m_height;
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