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

std::optional<VkSurfaceKHR> WindowBackend::CreateVulkanWindowSurface(VkInstance vkInstance)
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkResult vkResult = glfwCreateWindowSurface(vkInstance, m_handle, nullptr, &surface);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Window Surface. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Window Surface Created Successfully.\n";

    return surface;
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

int WindowBackend::GetWidth() const
{
    int width;
    glfwGetWindowSize(m_handle, &width, nullptr);

    return width;
}

int WindowBackend::GetHeight() const
{
    int height;
    glfwGetWindowSize(m_handle, nullptr, &height);

    return height;
}

std::vector<const char *> WindowBackend::GetRequiredInstanceExtensions() const
{
    uint32_t count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);

    return std::vector<const char *>(extensions, extensions + count);
}

}
