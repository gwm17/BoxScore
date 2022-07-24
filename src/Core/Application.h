#ifndef APPLICATION_H
#define APPLICATION_H

#include "Events/Event.h"
#include "Events/AppEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "ImGui/ImGuiLayer.h"
#include "BSProject.h"

namespace BoxScore {

    struct ApplicationArgs
    {
        std::string name;
        std::string runtimePath;
        int argc;
        char** argv;
    };

    class Application
    {
    public:
        Application(const ApplicationArgs& args);
        ~Application();

        void Run();
        void Close() { m_running = false; }
        void OnEvent(Event& e);
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        //App Event Callbacks
        bool OnWindowCloseEvent(WindowCloseEvent& e);

        const ApplicationArgs& GetArgs() { return m_args; }
        Window& GetWindow() { return *m_window; }
        static Application& GetInstance() { return *s_instance; }
    private:
        ApplicationArgs m_args;
        bool m_running;

        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;

        ImGuiLayer* m_imguiLayer;

        BSProject::Ref m_project;

        static Application* s_instance;
    };

    Application* CreateApplication(const ApplicationArgs& args);
}

#endif