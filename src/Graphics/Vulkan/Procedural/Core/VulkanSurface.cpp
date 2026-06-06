#include "VulkanSurface.hpp"

#include "VulkanInstance.hpp"
#include "Window/Window.hpp"

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

}
