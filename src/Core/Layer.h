#ifndef LAYER_H
#define LAYER_H

#include "Events/Event.h"

namespace BoxScore {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& e) {};
		virtual void OnImGuiRender(double timestep) {};

		const std::string& GetName() { return m_name; }

	private:
		std::string m_name;
	};
}

#endif