#include "GLFWWindow.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include <GLFW/glfw3.h>

namespace Engine::Window::GLFW {

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

    m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_handle == nullptr) {
        std::cerr << "GLFW Failed to Create Window.\n";
        return false;
    }

    return true;
}

void WindowBackend::Destroy()
{
    if (m_handle == nullptr)
        return;

    glfwDestroyWindow(m_handle);
    m_handle = nullptr;

    glfwTerminate();
}

void WindowBackend::Update()
{
    glfwPollEvents();

    if (!m_framebufferSizeInitialized) {
        m_currentFramebufferSize = GetFramebufferSize();
        m_lastFramebufferSize = m_currentFramebufferSize;
        m_framebufferSizeInitialized = true;
        return;
    }

    m_lastFramebufferSize = m_currentFramebufferSize;

    m_currentFramebufferSize = GetFramebufferSize();
}

bool WindowBackend::ShouldClose() const
{
    return glfwWindowShouldClose(m_handle);
}

bool WindowBackend::HasResized() const
{
    if (m_currentFramebufferSize.width != m_lastFramebufferSize.width)
        return true;

    if (m_currentFramebufferSize.height != m_lastFramebufferSize.height)
        return true;

    return false;
}

int WindowBackend::GetFramebufferWidth() const { return GetFramebufferSize().width; }
int WindowBackend::GetFramebufferHeight() const { return GetFramebufferSize().height; }

WindowFramebufferSize WindowBackend::GetFramebufferSize() const
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_handle, &width, &height);

    return WindowFramebufferSize {
        .width = width,
        .height = height
    };
}

NativeHandle WindowBackend::GetNativeHandle() const
{
    return m_handle;
}

}
