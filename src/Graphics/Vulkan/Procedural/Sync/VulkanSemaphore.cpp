#include "VulkanSemaphore.hpp"

#include <iostream>

#include "../Core/VulkanLogicalDevice.hpp"

namespace Engine::Vulkan {

std::optional<Semaphore> CreateSemaphore(const LogicalDevice &logicalDevice)
{
    VkSemaphoreCreateInfo vkSemaphoreCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkSemaphore handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateSemaphore(logicalDevice.handle, &vkSemaphoreCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Semaphore. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Semaphore Created Successfully.\n";

    return Semaphore {
        .handle = handle
    };
}

void DestroySemaphore(VkDevice vkDevice, Semaphore &semaphore)
{
    if (semaphore.handle != VK_NULL_HANDLE) {
        vkDestroySemaphore(vkDevice, semaphore.handle, nullptr);
        semaphore.handle = VK_NULL_HANDLE;
    }
}

}
