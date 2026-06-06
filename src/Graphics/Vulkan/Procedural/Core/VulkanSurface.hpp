#pragma once

#include <vulkan/vulkan.h>

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics::Vulkan {

struct Instance;

struct Surface {
    VkSurfaceKHR handle = VK_NULL_HANDLE;
};

Surface CreateSurface(const Instance &instance, Engine::Window::Window &window);
void DestroySurface(VkInstance vkInstance, Surface &surface);

}
