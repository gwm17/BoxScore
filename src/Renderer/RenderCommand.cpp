#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace BoxScore {

	RendererAPI* RenderCommand::s_api = new OpenGLRendererAPI();

}