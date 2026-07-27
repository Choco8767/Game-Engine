#include "VulkanSemaphore.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "../Core/VulkanLogicalDevice.hpp"

namespace Engine::Graphics::Vulkan {

Semaphore CreateSemaphore(const LogicalDevice &logicalDevice)
{
    VkSemaphoreCreateInfo vkSemaphoreCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkSemaphore handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateSemaphore(logicalDevice.handle, &vkSemaphoreCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        throw std::runtime_error(std::format("Failed to Create Vulkan Semaphore. Error Code: {}", static_cast<int>(vkResult)));
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
