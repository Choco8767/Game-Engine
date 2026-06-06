#include "GLFWWindow.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

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

    return true;
}

VkSurfaceKHR WindowBackend::CreateVulkanWindowSurface(VkInstance vkInstance)
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkResult vkResult = glfwCreateWindowSurface(vkInstance, m_handle, nullptr, &surface);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Window Surface. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Window Surface Created Successfully.\n";

    return surface;
}

void WindowBackend::Destroy()
{
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void WindowBackend::Update()
{
    glfwPollEvents();

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
    int width = INT_MAX;
    int height = INT_MAX;
    glfwGetFramebufferSize(m_handle, &width, &height);

    return WindowFramebufferSize {
        .width = width,
        .height = height
    };
}

std::vector<const char *> WindowBackend::GetRequiredInstanceExtensions() const
{
    uint32_t count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);

    return std::vector<const char *>(extensions, extensions + count);
}

}
