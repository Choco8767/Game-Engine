#include "VulkanLogicalDevice.hpp"

#include <iostream>
#include <set>
#include <vector>

#include "VulkanConfig.hpp"
#include "VulkanPhysicalDevice.hpp"

namespace Engine::Vulkan {

std::optional<LogicalDevice> CreateLogicalDevice(const PhysicalDevice &physicalDevice)
{
    float queuePriority = 1.0f;

    std::set<uint32_t> uniqueQueueFamilies = {
        physicalDevice.queueFamilyIndices.graphicsFamily.value(),
        physicalDevice.queueFamilyIndices.presentFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo> vkQueueCreateInfos;
    vkQueueCreateInfos.reserve(uniqueQueueFamilies.size());

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo vkQueueCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        };

        vkQueueCreateInfos.emplace_back(vkQueueCreateInfo);
    }

    VkDeviceCreateInfo vkDeviceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(vkQueueCreateInfos.size()),
        .pQueueCreateInfos = vkQueueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size()),
        .ppEnabledExtensionNames = DEVICE_EXTENSIONS.data(),
        .pEnabledFeatures = &physicalDevice.deviceFeatures
    };

    VkDevice handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateDevice(physicalDevice.handle, &vkDeviceCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Logical Device. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    VkQueue vkGraphicsQueue = VK_NULL_HANDLE;
    vkGetDeviceQueue(handle, physicalDevice.queueFamilyIndices.graphicsFamily.value(), 0, &vkGraphicsQueue);

    VkQueue vkPresentQueue = VK_NULL_HANDLE;
    vkGetDeviceQueue(handle, physicalDevice.queueFamilyIndices.presentFamily.value(), 0, &vkPresentQueue);

    std::cout << "Vulkan Logical Device Created Successfully.\n";

    return LogicalDevice {
        .handle = handle,
        .graphicsQueue = vkGraphicsQueue,
        .presentQueue = vkPresentQueue
    };
}

void DestroyLogicalDevice(LogicalDevice &logicalDevice)
{
    logicalDevice.graphicsQueue = VK_NULL_HANDLE;
    logicalDevice.presentQueue = VK_NULL_HANDLE;

    if (logicalDevice.handle != VK_NULL_HANDLE) {
        vkDestroyDevice(logicalDevice.handle, nullptr);
        logicalDevice.handle = VK_NULL_HANDLE;
    }
}

void WaitIdle(const LogicalDevice &logicalDevice)
{
    vkDeviceWaitIdle(logicalDevice.handle);
}

}
