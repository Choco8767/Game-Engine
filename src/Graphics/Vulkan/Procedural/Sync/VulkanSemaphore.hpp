#pragma once

#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct LogicalDevice;

struct Semaphore {
    VkSemaphore handle = VK_NULL_HANDLE;
};

std::optional<Semaphore> CreateSemaphore(const LogicalDevice &logicalDevice);
void DestroySemaphore(VkDevice vkDevice, Semaphore &semaphore);

}
