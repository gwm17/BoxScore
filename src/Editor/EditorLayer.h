#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Core/BoxScoreCore.h"
#include "Core/Layer.h"
#include "FileDialog.h"

#include "imgui.h"

namespace BoxScore {

	class EditorLayer : public Layer
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		EditorLayer();
		~EditorLayer();

		void SetEventCallback(const EventCallbackFunc& func) { m_eventCallback = func; }

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		EventCallbackFunc m_eventCallback;

		FileDialog m_fileDialog;

		//ImGui Settings
		bool dockspaceOpen = true;
		bool opt_fullscreen = true;
		bool opt_fullscreen_persistant = true;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	};
}

#endif