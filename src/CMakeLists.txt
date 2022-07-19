add_executable(BoxScore)

target_include_directories(BoxScore
    PUBLIC ../vendor/imgui
    ../vendor/glfw/include
    ../vendor/spdlog/include
    ../vendor/glad/include
    ./
)

#For Windows, CAENDigitizer is installed non-globally (we assume default pathing cause why would ever be otherwise)
if(MSVC)
    target_include_directories(BoxScore SYSTEM PUBLIC ${CAEN_WIN_INCL_PATH})
endif()

target_precompile_headers(BoxScore PRIVATE bxscpch.h)

target_sources(BoxScore PRIVATE
    Core/Digitizer.h
    Core/Digitizer.cpp
    Core/DigitizerDefs.h
    Core/Synchronize.h
    Core/Synchronize.cpp
    Core/Logger.h
    Core/Logger.cpp
    Core/main.cpp
    Core/Application.h
    Core/Application.cpp
    Events/Event.h
    Core/BoxScoreCore.h
    Events/AppEvent.h
    Events/MouseEvent.h
    Events/AcqEvent.h
    Core/Layer.h
    Core/Layer.cpp
    Core/LayerStack.h
    Core/LayerStack.cpp
    Core/Window.h
    Platform/OpenGL/OpenGLWindow.h
    Renderer/GraphicsContext.h
    Platform/OpenGL/OpenGLContext.h
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLWindow.cpp
    Renderer/RendererAPI.h
    Renderer/RendererAPI.cpp
    Renderer/RenderCommand.h
    Renderer/RenderCommand.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp
    ImGui/ImGuiLayer.h
    ImGui/ImGuiLayer.cpp
    ImGui/ImGuiBuild.cpp
    Editor/EditorLayer.h
    Editor/EditorLayer.cpp
    Editor/FileDialog.h
    Editor/FileDialog.cpp
)

#Packaged dependencies
target_link_libraries(BoxScore PRIVATE imgui glfw glad)

#CAEN dependencies
target_link_libraries(BoxScore PRIVATE ${CAEN_DGTZ})

#Link OS graphics implementations
set(THREADS_PREFER_PTHREAD_FLAG On)
find_package(Threads REQUIRED)
if(APPLE)
    target_link_libraries(BoxScore PRIVATE "-framework Cocoa" "-framework CoreVideo" "-framework IOKit" "-framework OpenGL" "-framework Carbon" ${CMAKE_DL_LIBS} Threads::Threads)
    target_compile_definitions(BoxScore PRIVATE BS_APPLE)
elseif(UNIX)
    target_link_libraries(BoxScore PRIVATE libGL.so libX11.so ${CMAKE_DL_LIBS} Threads::Threads)
    target_compile_definitions(BoxScore PRIVATE BS_LINUX)
elseif(MSVC)
    target_link_libraries(BoxScore PRIVATE opengl32.lib Threads::Threads)
    target_compile_definitions(BoxScore PRIVATE BS_WINDOWS)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(BoxScore PRIVATE BS_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(BoxScore PRIVATE BS_DEBUG)
endif()

#Some extra defs
#once we get there
#target_compile_definitions(BoxScore PRIVATE GLFW_INCLUDE_NONE YAML_CPP_STATIC_DEFINE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_compile_definitions(BoxScore PRIVATE GLFW_INCLUDE_NONE IMGUI_IMPL_OPENGL_LOADER_GLAD)

set_target_properties(BoxScore PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BS_BINARY_DIR})