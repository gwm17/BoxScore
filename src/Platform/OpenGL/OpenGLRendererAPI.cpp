#include "OpenGLRendererAPI.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace BoxScore {

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(float color[4])
	{
		glClearColor(color[0], color[1], color[2], color[3]);
	}

	double OpenGLRendererAPI::GetFrameTime()
	{
		return glfwGetTime();
	}
}