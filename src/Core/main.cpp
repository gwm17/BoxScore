#include "Application.h"

int main(int argc, char** argv)
{
    BoxScore::Logger::Init();
    BS_INFO("Welcome to BoxScore.");

    BoxScore::ApplicationArgs args;
    args.name = "BoxScore";
    args.runtimePath = "";
    args.argc = argc;
    args.argv = argv;

    BoxScore::Application* app = BoxScore::CreateApplication(args);

    app->Run();

    delete app;
}