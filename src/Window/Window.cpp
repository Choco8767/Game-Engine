#include "Window.hpp"

#include <stdexcept>

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
        throw std::runtime_error("Failed to Create Window. Unknown or Unsupported Window API.");

    if (!window->Init())
        throw std::runtime_error("Failed to Initialize Window.");

    return window;
}

}
