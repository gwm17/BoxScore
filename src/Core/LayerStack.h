#ifndef LAYER_STACK_H
#define LAYER_STACK_H

#include "Layer.h"

namespace BoxScore {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_stack.begin(); }
		std::vector<Layer*>::iterator end() { return m_stack.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_stack.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_stack.rend(); }
	private:
		std::vector<Layer*> m_stack;
		size_t m_insertIndex = 0;
	};
}

#endif