#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "Renderer/RendererAPI.h"

namespace BoxScore {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(float color[4]) override;
		virtual void Clear() override;
		virtual double GetFrameTime() override;
	};

}

#endif