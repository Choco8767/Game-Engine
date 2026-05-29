#pragma once

#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct PhysicalDevice;

struct LogicalDevice {
    VkDevice handle = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
};

std::optional<LogicalDevice> CreateLogicalDevice(const PhysicalDevice &physicalDevice);
void DestroyLogicalDevice(LogicalDevice &logicaldevice);

void WaitIdle(const LogicalDevice &logicalDevice);

}
