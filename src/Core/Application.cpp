#include "Application.h"
#include <filesystem>


namespace BoxScore {

    Application* Application::s_instance = nullptr;

    Application* CreateApplication(const ApplicationArgs& args) { return new Application(args); }

    Application::Application(const ApplicationArgs& args) :
        m_args(args), m_running(true)
    {
        if(s_instance != nullptr)
        {
            BS_ERROR("Attempting to create more than one application! CreateApplication should only be called once!");
        }

        s_instance = this;

        BS_INFO("Application created.");

        //Set current path to find assets
        if(!m_args.runtimePath.empty())
            std::filesystem::current_path(m_args.runtimePath);

        BS_INFO("Runtime path set to {0}", std::filesystem::current_path().string());

    }

    Application::~Application() {}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatch(e);
        dispatch.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowCloseEvent));

        for (auto iter = m_layerStack.rbegin(); iter != m_layerStack.rend(); iter++)
        {
            if (e.handled)
                break;

            (*iter)->OnEvent(e);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_layerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
    {
        m_running = false;
        BS_INFO("Window close event recieved. Shutting down.");
        return true;
    }

    void Application::Run()
    {
        if (!m_running)
            BS_ERROR("Application attempting to run after having already exited");

        while (m_running)
        {
            BS_INFO("Application running...");
            WindowCloseEvent e;
            OnEvent(e);
        }

    }
}