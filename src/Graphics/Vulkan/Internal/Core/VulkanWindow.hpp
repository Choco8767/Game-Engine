#pragma once

#include <vector>

#include "VulkanSurface.hpp"
#include "Window/NativeHandle.hpp"

namespace Engine::Graphics::Vulkan {

struct Instance;

std::vector<const char *> GetRequiredInstanceExtensions();
Surface CreateSurface(const Instance &instance, const Window::NativeHandle &window);

}
