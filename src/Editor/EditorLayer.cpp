#include "EditorLayer.h"
#include "Core/Application.h"
#include "Events/AcqEvent.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Core/ProjectSerializer.h"

namespace BoxScore {

    EditorLayer::EditorLayer(const BSProject::Ref& project) :
        Layer("EditorLayer"), m_project(project)
	{
        m_projectPath = m_project->GetProjectPath().string();
        m_runNumber = m_project->GetRunNumber();
        m_digitizerArgList = m_project->GetDigitizerArgsList();

        //temp
        m_digiPanels.emplace_back(DigitizerArgs());
	}

	EditorLayer::~EditorLayer() {}

	void EditorLayer::OnAttach() {}

	void EditorLayer::OnDetach() {}

	void EditorLayer::OnUpdate() {}

	void EditorLayer::OnEvent(Event& e)
    {
    }

    void EditorLayer::UpdateDigitizerPanels()
    {
        if (m_project->GetNumberOfBoards() != m_digiPanels.size())
        {
            m_digitizerArgList = m_project->GetDigitizerArgsList();
            m_digiPanels.clear();
            for (auto& args : m_digitizerArgList)
                m_digiPanels.emplace_back(args);
        }

        std::vector<DigitizerParameters> boardParams = m_project->GetDigitizerParameterList();
        int handle;
        for (auto& panel : m_digiPanels)
        {
            handle = panel.GetDigitizerHandle();
            if (handle == -1)
                continue;
            panel.SetDigitizerParameters(boardParams[handle]);
            switch (panel.GetPanelType())
            {
            case DigitizerPanel::Type::PHA: panel.SetPHAParameters(m_project->GetPHAParameters(handle), m_project->GetPHAWaveParameters(handle)); break;
            case DigitizerPanel::Type::PSD: panel.SetPSDParameters(m_project->GetPSDParameters(handle), m_project->GetPSDWaveParameters(handle)); break;
            case DigitizerPanel::Type::None: break;
            }
        }
    }

	void EditorLayer::OnImGuiRender()
	{
        static uint32_t stepSize = 1;
        static uint32_t fastStepSize = 5;
        static bool autoIncrFlag = true;
        static std::string dppModeString = "";
        static bool dataServer = false;
        static bool writeDataToDisk = false;
        static std::string runString = "Start Run";
        static bool disableAll = false;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
       // because it would be confusing to have two docking targets within each others.
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("MyTestSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (disableAll)
            ImGui::BeginDisabled();

        //Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Project..."))
                {
                    m_fileDialog.OpenDialog(FileDialog::Type::OpenFile);
                }
                if (ImGui::MenuItem("Save Project"))
                {
                    m_fileDialog.OpenDialog(FileDialog::Type::SaveFile);
                }
                if (ImGui::MenuItem("Exit"))
                {
                    WindowCloseEvent wc_event;
                    m_eventCallback(wc_event);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Digitizer"))
            {
                if (ImGui::MenuItem("Scan for boards..."))
                {
                    AcqDetectBoardsEvent db_event;
                    m_eventCallback(db_event);
                    UpdateDigitizerPanels();
                }
                if (ImGui::MenuItem("Disconnect boards..."))
                {
                    AcqDisconnectBoardsEvent db_event;
                    m_eventCallback(db_event);
                    UpdateDigitizerPanels();
                }
                if (ImGui::MenuItem("Synchronize boards..."))
                {
                    m_syncDialog.OpenDialog();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (disableAll)
            ImGui::EndDisabled();
        //End of menu bar

        //Begin main ImGui window
        if (ImGui::Begin("Controls"))
        {
            //Begin internal tab bar
            if (ImGui::BeginTabBar("BS_TabBar", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Project"))
                {
                    if (ImGui::Button(runString.c_str()))
                    {
                        if (runString == "Start Run")
                        {
                            AcqStartEvent e;
                            m_eventCallback(e);
                            runString = "Stop Run";
                            disableAll = true;
                        }
                        else if (runString == "Stop Run")
                        {
                            AcqStopEvent e;
                            m_eventCallback(e);
                            runString = "Start Run";
                            disableAll = false;
                        }
                    }

                    if (disableAll)
                        ImGui::BeginDisabled();

                    ImGui::Text("Project Directory: ");
                    ImGui::SameLine();
                    ImGui::Text("%s", m_projectPath.c_str());

                    ImGui::InputScalar("Run Number", ImGuiDataType_U32, &m_runNumber, &stepSize, &fastStepSize);
                    ImGui::SameLine();
                    ImGui::Checkbox("Auto-increment", &autoIncrFlag);

                    if (ImGui::BeginCombo("DPP Acquisition Mode", dppModeString.c_str()))
                    {
                        if (ImGui::Selectable("List", dppModeString == "List"))
                        {
                            dppModeString = "List";
                            m_project->SetDPPAcqMode(DPPAcqMode::List);
                            AcqDPPModeEvent e;
                            m_eventCallback(e);
                        }
                        if (ImGui::Selectable("Waves", dppModeString == "Waves"))
                        {
                            dppModeString = "Waves";
                            m_project->SetDPPAcqMode(DPPAcqMode::Waves);
                            AcqDPPModeEvent e;
                            m_eventCallback(e);
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Checkbox("Write data to disk", &writeDataToDisk);
                    ImGui::Checkbox("Data Server", &dataServer);

                    ImGui::EndTabItem();
                }

                bool panel_status;
                for (auto& panel : m_digiPanels)
                {
                    panel_status = panel.OnImGuiRender();
                    //handle state change
                    if (panel_status)
                    {
                        //Update project
                        m_project->SetDigitizerParameters(panel.GetDigitizerHandle(), panel.GetDigitizerParameters());
                        if (panel.GetPanelType() == DigitizerPanel::Type::PHA)
                        {
                            m_project->SetPHAParameters(panel.GetDigitizerHandle(), panel.GetPHAChannelParameters());
                            m_project->SetPHAWaveParameters(panel.GetDigitizerHandle(), panel.GetPHAWaveParameters());
                        }
                        else if (panel.GetPanelType() == DigitizerPanel::Type::PSD)
                        {
                            m_project->SetPSDParameters(panel.GetDigitizerHandle(), panel.GetPSDChannelParameters());
                            m_project->SetPSDWaveParameters(panel.GetDigitizerHandle(), panel.GetPSDWaveParameters());
                        }
                        else
                        {
                            BS_WARN("Unidentified digitizer type attempting to communicate with project");
                        }
                        //Emit event to update aquisition side
                        AcqParametersEvent e(panel.GetDigitizerHandle());
                        m_eventCallback(e);
                    }

                }
                ImGui::EndTabBar();
            }
            //End internal tab bar

            if (disableAll)
                ImGui::EndDisabled();
        }
        ImGui::End();
        

        auto fd_result = m_fileDialog.RenderFileDialog(".yaml");
        if (!fd_result.first.empty())
        {
            switch (fd_result.second)
            {
                case FileDialog::Type::OpenFile:
                {
                    ProjectSerializer serializer(fd_result.first);
                    serializer.DeserializeData(m_project);
                    m_project->SetProjectPath("");
                    m_projectPath = m_project->GetProjectPath().string();
                    break;
                }
                case FileDialog::Type::SaveFile:
                {
                    ProjectSerializer serializer("./test.yaml");
                    serializer.SerializeData(m_project);
                    break;
                }
                case FileDialog::Type::OpenDir: break; //Unused
                case FileDialog::Type::None: break; //Null result
            }
            
        }

        bool test = m_syncDialog.OnImGuiRender();
        if (test)
        {
            AcqSyncArgsEvent e = m_syncDialog.GetSyncEvent();
            m_eventCallback(e);
            UpdateDigitizerPanels();
        }

        ImGui::End();
	}
}