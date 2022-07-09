#ifndef APPLICATION_H
#define APPLICATION_H

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

        const ApplicationArgs& GetArgs() { return m_args; }

        static Application& GetInstance() { return *s_instance; }
    private:
        ApplicationArgs m_args;

        static Application* s_instance;
    };

    Application* CreateApplication(const ApplicationArgs& args);
}

#endif