#include "OpenGLWindow.h"
#include "OpenGLContext.h"
#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace BoxScore {

	static bool s_glfwInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		BS_ERROR("GLFW Error! Code: {0} Message: {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& props) { return new OpenGLWindow(props); }

	OpenGLWindow::OpenGLWindow(const WindowProperties& props)
	{
		Init(props);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		Shutdown();
	}

	void OpenGLWindow::Init(const WindowProperties& props)
	{
		m_data.width = props.width;
		m_data.height = props.height;
		m_data.name = props.title;

		BS_INFO("Creating OpenGL Window with width {0}, height {1}, and name {2}", m_data.width, m_data.height, m_data.name);

		if (!s_glfwInitialized)
		{
			int passed = glfwInit();
			BS_INFO("Initializing GLFW ... status {0}", passed);
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		//Apple in their effort to force Metal on everyone, has deprecated OpenGL. We have to be
		//A little more agressive to make OpenGL and GLFW play well together.
		#ifdef BS_APPLE
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
		#endif

		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.name.c_str(), nullptr, nullptr);
		
		m_context = new OpenGLContext(m_window); //This is the only seriously OpenGL specific code
		m_context->Init();
		
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);
		
		//Set all of the callback functions for the window. 
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;
				WindowResizeEvent event(width, height);
				data.callbackFunc(event);
			});
		
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.callbackFunc(event);
			});
		
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.callbackFunc(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.callbackFunc(event);
					break;
				}
				case GLFW_REPEAT:
				{
					//GLFW doesnt have a hold count, so here we just pass 1 to indicate a hold is happening.
					KeyPressedEvent event(key, 1);
					data.callbackFunc(event);
					break;
				}
				}
			});
		
		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int character)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(character);
				data.callbackFunc(event);
			});
		
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.callbackFunc(event);
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.callbackFunc(event);
				}
				}
			});
		
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.callbackFunc(event);
			});
		
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
			{
				Data& data = *(Data*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xpos, (float)ypos);
				data.callbackFunc(event);
			});
	}

	void OpenGLWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void OpenGLWindow::OnUpdate()
	{
		glfwPollEvents();
		m_context->SwapBuffers();
	}

	void OpenGLWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.isVSync = enabled;
	}

}