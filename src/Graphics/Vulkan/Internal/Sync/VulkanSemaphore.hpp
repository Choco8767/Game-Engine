#pragma once

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct Semaphore {
    VkSemaphore handle = VK_NULL_HANDLE;
};

Semaphore CreateSemaphore(const LogicalDevice &logicalDevice);
void DestroySemaphore(VkDevice vkDevice, Semaphore &semaphore);

}
