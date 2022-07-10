#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace BoxScore {

	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const WindowProperties& props);
		virtual ~OpenGLWindow();

		virtual void OnUpdate() override;
		virtual uint32_t GetWidth() override { return m_data.width; }
		virtual uint32_t GetHeight() override { return m_data.height; }
		virtual std::string GetName() override { return m_data.name; }

		virtual void SetEventCallback(EventCallbackFunc function) override { m_data.callbackFunc = function; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() override { return m_data.isVSync; }

		virtual void* GetNativeWindow() const override { return m_window; } //dirty

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();

		GraphicsContext* m_context;
		GLFWwindow* m_window;

		struct Data
		{
			int width, height;
			std::string name;
			bool isVSync;
			EventCallbackFunc callbackFunc;
		};

		Data m_data;
	};
}


#endif