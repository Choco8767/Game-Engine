#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanDebugMessenger.hpp"

namespace Engine::Vulkan {

struct Instance {
    VkInstance handle = VK_NULL_HANDLE;

    std::optional<DebugMessenger> debugMessenger = std::nullopt;
};

std::optional<Instance> CreateInstance(const std::vector<const char *> &requiredExtensions);
void DestroyInstance(Instance &instance);

}
