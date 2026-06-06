#include "Window.hpp"

#include "GLFW/GLFWWindow.hpp"

namespace Engine::Window {

std::unique_ptr<Window> CreateWindow(API api)
{
    std::unique_ptr<Window> window;

    switch (api) {
    case API::GLFW:
        window = std::make_unique<GLFW::WindowBackend>();
        break;
    default:
        break;
    }

    if (!window)
        return nullptr;

    window->Init();

    return window;
}

}
