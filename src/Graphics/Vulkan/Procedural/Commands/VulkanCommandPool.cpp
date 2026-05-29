#include "VulkanCommandPool.hpp"

#include <iostream>

#include "../Core/VulkanLogicalDevice.hpp"
#include "../Core/VulkanPhysicalDevice.hpp"

namespace Engine::Vulkan {

std::optional<CommandPool> CreateCommandPool(
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice)
{
    VkCommandPoolCreateInfo vkCommandPoolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = physicalDevice.queueFamilyIndices.graphicsFamily.value()
    };

    VkCommandPool handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateCommandPool(logicalDevice.handle, &vkCommandPoolCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Command Pool. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

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
