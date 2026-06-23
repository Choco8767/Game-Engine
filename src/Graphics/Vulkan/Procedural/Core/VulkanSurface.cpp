#include "VulkanSurface.hpp"

#include <algorithm>

#include "Window/Window.hpp"

#include "VulkanInstance.hpp"
#include "VulkanPhysicalDevice.hpp"

namespace Engine::Graphics::Vulkan {

Surface CreateSurface(const Instance &instance, Window::Window &window)
{
    VkSurfaceKHR handle = window.CreateVulkanWindowSurface(instance.handle);

    return Surface {
        .handle = handle
    };
}

void DestroySurface(VkInstance vkInstance, Surface &surface)
{
    if (surface.handle != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(vkInstance, surface.handle, nullptr);
        surface.handle = VK_NULL_HANDLE;
    }
}

VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
{
    for (const auto &format : formats) {
        if (
            format.format == VK_FORMAT_B8G8R8A8_SRGB
            && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    return formats[0];
}

VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &presentModes)
{
    for (const auto &presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return presentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ChooseSwapchainExtent(const Engine::Window::Window &window, const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;

    int width = window.GetFramebufferWidth();
    int height = window.GetFramebufferHeight();

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

void PopulateSurfaceDetails(
    const PhysicalDevice &physicalDevice,
    Surface &surface)
{
    auto swapchainSupport = QuerySwapchainSupport(surface, physicalDevice);

    surface.surfaceFormat = ChooseSwapchainSurfaceFormat(swapchainSupport.formats);
    surface.presentMode = ChooseSwapchainPresentMode(swapchainSupport.presentModes);
}

}
