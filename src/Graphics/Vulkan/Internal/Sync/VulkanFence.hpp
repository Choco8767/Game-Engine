#pragma once

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct Fence {
    VkFence handle = VK_NULL_HANDLE;
};

Fence CreateFence(const LogicalDevice &logicalDevice, bool signaled = false);
void DestroyFence(VkDevice vkDevice, Fence &fence);

void WaitForFence(const LogicalDevice &logicalDevice, const Fence &fence);
void ResetFence(const LogicalDevice &logicalDevice, const Fence &fence);

}
