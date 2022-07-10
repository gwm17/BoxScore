#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace BoxScore {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_window;
	};
}

#endif