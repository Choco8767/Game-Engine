#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

#include <vk_mem_alloc.h>

#include "VulkanMemoryAllocator.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "../Commands/VulkanCommandPool.hpp"
#include "../Core/VulkanInstance.hpp"
#include "../Core/VulkanLogicalDevice.hpp"
#include "../Core/VulkanPhysicalDevice.hpp"

namespace Engine::Graphics::Vulkan {

MemoryAllocator CreateMemoryAllocator(
    const Instance &instance,
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice)
{
    VmaVulkanFunctions vmaVulkanFunctions { };
    vmaVulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vmaVulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo vmaAllocatorCreateInfo {
        .physicalDevice = physicalDevice.handle,
        .device = logicalDevice.handle,
        .pVulkanFunctions = &vmaVulkanFunctions,
        .instance = instance.handle,
        .vulkanApiVersion = VK_API_VERSION_1_4
    };

    VmaAllocator handle = nullptr;

    VkResult vkResult = vmaCreateAllocator(&vmaAllocatorCreateInfo, &handle);
    if (vkResult != VK_SUCCESS) {
        throw std::runtime_error(std::format(
            "Failed to Create Vulkan Memory Allocator. Error Code: {}",
            static_cast<int>(vkResult)));
    }

    std::cout << "Vulkan Memory Allocator Created Successfully.\n";

    auto commandPool = CreateCommandPool(
        logicalDevice,
        physicalDevice.queueFamilyIndices.transferFamily.value());

    return MemoryAllocator {
        .handle = handle,
        .commandPool = commandPool
    };
}

void DestroyMemoryAllocator(const LogicalDevice &logicalDevice, MemoryAllocator &memoryAllocator)
{
    WaitIdle(logicalDevice);

    DestroyCommandPool(logicalDevice, memoryAllocator.commandPool);

    if (memoryAllocator.handle != nullptr) {
        vmaDestroyAllocator(memoryAllocator.handle);
        memoryAllocator.handle = nullptr;
    }
}

void MapMemory(
    VmaAllocator allocator,
    VmaAllocation allocation,
    const void *data,
    std::size_t size)
{
    void *mappedData = nullptr;

    VkResult vkResult = vmaMapMemory(allocator, allocation, &mappedData);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Map VMA Memory. Error Code: {}", static_cast<int>(vkResult)));

    std::memcpy(mappedData, data, size);

    vmaUnmapMemory(allocator, allocation);
}

}
