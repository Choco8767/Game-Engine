#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct CommandPool {
    VkCommandPool handle = VK_NULL_HANDLE;
};

CommandPool CreateCommandPool(const LogicalDevice &logicalDevice, uint32_t queueFamilyIndex);
void DestroyCommandPool(const LogicalDevice &logicalDevice, CommandPool &commandPool);

}
