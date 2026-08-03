#pragma once

#include <vector>

#include <volk.h>

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics::Vulkan {

struct Instance;
struct PhysicalDevice;

struct Surface {
    VkSurfaceKHR handle = VK_NULL_HANDLE;
    VkSurfaceFormatKHR surfaceFormat {};
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
};

Surface CreateSurface(const Instance &instance, Engine::Window::Window &window);
void DestroySurface(VkInstance vkInstance, Surface &surface);

VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &presentModes);
VkExtent2D ChooseSwapchainExtent(const Engine::Window::Window &window, const VkSurfaceCapabilitiesKHR &capabilities);

void PopulateSurfaceDetails(
    const PhysicalDevice &physicalDevice,
    Surface &surface);

}
