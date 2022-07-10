#ifndef WINDOW_H
#define WINDOW_H

#include "BoxScoreCore.h"
#include "Events/Event.h"

namespace BoxScore {

	struct WindowProperties
	{
		WindowProperties(const std::string& t = "BoxScore", uint32_t w=1280, uint32_t h=720) :
			title(t), width(w), height(h)
		{
		}

		std::string title;
		uint32_t width;
		uint32_t height;
	};

	class Window
	{
	public:

		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual std::string GetName() = 0;

		virtual void SetEventCallback(EventCallbackFunc function) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;

		virtual void* GetNativeWindow() const = 0; //dirty

		static Window* Create(const WindowProperties& props = WindowProperties());

	};
}

#endif