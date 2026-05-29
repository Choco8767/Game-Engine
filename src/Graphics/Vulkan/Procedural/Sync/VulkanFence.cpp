#include "VulkanFence.hpp"

#include <iostream>

#include "../Core/VulkanLogicalDevice.hpp"

namespace Engine::Vulkan {

std::optional<Fence> CreateFence(const LogicalDevice &logicalDevice, bool signaled)
{
    VkFenceCreateInfo vkFenceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
    };

    if (signaled)
        vkFenceCreateInfo.flags |= VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateFence(logicalDevice.handle, &vkFenceCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Fence. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Fence Created Successfully.\n";

    return Fence {
        .handle = handle
    };
}

void DestroyFence(VkDevice vkDevice, Fence &fence)
{
    if (fence.handle != VK_NULL_HANDLE) {
        vkDestroyFence(vkDevice, fence.handle, nullptr);
        fence.handle = VK_NULL_HANDLE;
    }
}

void WaitForFence(const LogicalDevice &logicalDevice, const Fence &fence)
{
    vkWaitForFences(logicalDevice.handle, 1, &fence.handle, VK_TRUE, UINT64_MAX);
}

void ResetFence(const LogicalDevice &logicalDevice, const Fence &fence)
{
    vkResetFences(logicalDevice.handle, 1, &fence.handle);
}

}
