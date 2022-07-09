#include "Application.h"
#include <filesystem>

namespace BoxScore {

    Application* Application::s_instance = nullptr;

    Application* CreateApplication(const ApplicationArgs& args) { return new Application(args); }

    Application::Application(const ApplicationArgs& args) :
        m_args(args)
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
}