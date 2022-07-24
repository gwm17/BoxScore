#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Core/BoxScoreCore.h"
#include "Core/Layer.h"
#include "FileDialog.h"
#include "Events/AcqEvent.h"
#include "Core/BSProject.h"

#include "imgui.h"

namespace BoxScore {

	class EditorLayer : public Layer
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		EditorLayer(const BSProject::Ref& project);
		~EditorLayer();

		void SetEventCallback(const EventCallbackFunc& func) { m_eventCallback = func; }

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		bool OnAcqBoardsFoundEvent(AcqBoardsFoundEvent& e);

		EventCallbackFunc m_eventCallback;

		BSProject::Ref m_project;
		std::string m_projectPath;
		uint32_t m_runNumber;
		std::vector<DigitizerArgs> m_digitizerArgList;

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