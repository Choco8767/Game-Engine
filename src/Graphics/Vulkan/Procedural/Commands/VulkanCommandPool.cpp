#include "VulkanCommandPool.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "../Core/VulkanLogicalDevice.hpp"

namespace Engine::Graphics::Vulkan {

CommandPool CreateCommandPool(const LogicalDevice &logicalDevice, uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo vkCommandPoolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex
    };

    VkCommandPool handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateCommandPool(logicalDevice.handle, &vkCommandPoolCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Command Pool. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Command Pool Created Successfully.\n";

    return CommandPool {
        .handle = handle
    };
}

void DestroyCommandPool(VkDevice vkDevice, CommandPool &commandPool)
{
    if (commandPool.handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(vkDevice, commandPool.handle, nullptr);
        commandPool.handle = VK_NULL_HANDLE;
    }
}

}
