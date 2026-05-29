#include "VulkanSurface.hpp"

#include "VulkanInstance.hpp"
#include "Window/Window.hpp"

namespace Engine::Vulkan {

std::optional<Surface> CreateSurface(const Instance &instance, Window &window)
{
    std::optional<VkSurfaceKHR> optHandle = window.CreateVulkanWindowSurface(instance.handle);
    if (!optHandle.has_value())
        return std::nullopt;

    VkSurfaceKHR handle = optHandle.value();

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

}
