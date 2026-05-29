#include "Window.hpp"

#include "GLFW/GLFWWindow.hpp"

namespace Engine {

std::unique_ptr<Window> CreateWindow(WindowAPI api)
{
    std::unique_ptr<Window> window;

    switch (api) {
    case WindowAPI::GLFW:
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
