#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanDebugMessenger.hpp"

namespace Engine::Graphics::Vulkan {

struct Instance {
    VkInstance handle = VK_NULL_HANDLE;

    std::optional<DebugMessenger> debugMessenger = std::nullopt;
};

Instance CreateInstance(const std::vector<const char *> &requiredExtensions);
void DestroyInstance(Instance &instance);

}
