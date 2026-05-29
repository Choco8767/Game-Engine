#pragma once

#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct PhysicalDevice;
struct LogicalDevice;

struct CommandPool {
    VkCommandPool handle = VK_NULL_HANDLE;
};

std::optional<CommandPool> CreateCommandPool(
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice);
void DestroyCommandPool(VkDevice vkDevice, CommandPool &commandPool);

}
