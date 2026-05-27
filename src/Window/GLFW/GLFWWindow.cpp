#include "GLFWWindow.hpp"

#include <iostream>

namespace Engine::GLFW {

WindowBackend::~WindowBackend()
{
    Destroy();
}

bool WindowBackend::Init(
    int width,
    int height,
    const char *title)
{
    if (!glfwInit()) {
        std::cerr << "GLFW Failed to Initialize.\n";
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    return true;
}

void WindowBackend::Destroy()
{
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void WindowBackend::PollEvents()
{
    glfwPollEvents();
}

bool WindowBackend::ShouldClose() const
{
    return glfwWindowShouldClose(m_handle);
}

std::vector<const char *> WindowBackend::GetRequiredInstanceExtensions() const
{
    uint32_t count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);

    return std::vector<const char *>(extensions, extensions + count);
}

}
