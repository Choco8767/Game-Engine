#pragma once

#include <optional>

#include <vulkan/vulkan.h>

namespace Engine {

class Window;

}

namespace Engine::Vulkan {

struct Instance;

struct Surface {
    VkSurfaceKHR handle = VK_NULL_HANDLE;
};

std::optional<Surface> CreateSurface(const Instance &instance, Window &window);
void DestroySurface(VkInstance vkInstance, Surface &surface);

}
