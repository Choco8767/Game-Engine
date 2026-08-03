#pragma once

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct PhysicalDevice;

struct LogicalDevice {
    VkDevice handle = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkQueue transferQueue = VK_NULL_HANDLE;
};

LogicalDevice CreateLogicalDevice(const PhysicalDevice &physicalDevice);
void DestroyLogicalDevice(LogicalDevice &logicaldevice);

void WaitIdle(const LogicalDevice &logicalDevice);

}
