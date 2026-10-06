#include "VulkanWindow.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanInstance.hpp"

namespace Engine::Graphics::Vulkan {

std::vector<const char *> GetRequiredInstanceExtensions()
{
    std::uint32_t count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);

    if (extensions == nullptr)
        throw std::runtime_error("GLFW Reported No Vulkan Support for Required Instance Extensions.");

    return std::vector<const char *>(extensions, extensions + count);
}

Surface CreateSurface(const Instance &instance, const Window::NativeHandle &window)
{
    auto glfwWindow = static_cast<GLFWwindow *>(window);

    VkSurfaceKHR handle = VK_NULL_HANDLE;

    VkResult vkResult = glfwCreateWindowSurface(instance.handle, glfwWindow, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Window Surface. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Window Surface Created Successfully.\n";

    return Surface {
        .handle = handle
    };
}

}
