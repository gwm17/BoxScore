#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace BoxScore {

	OpenGLContext::OpenGLContext(GLFWwindow* window) :
		m_window(window)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		BS_TRACE("Loaded OpenGL with glad Init status {0}", status);

		BS_INFO("Loaded OpenGL renderer");
		BS_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		BS_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		BS_INFO("Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}