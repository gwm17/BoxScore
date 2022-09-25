#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include "RendererAPI.h"

namespace BoxScore {

	class RenderCommand
	{
	public:

		static void SetClearColor(float color[4]) { s_api->SetClearColor(color); }
		static void Clear() { s_api->Clear(); }
		static double GetFrameTime() { return s_api->GetFrameTime(); }

	private:
		static RendererAPI* s_api;
	};
}

#endif