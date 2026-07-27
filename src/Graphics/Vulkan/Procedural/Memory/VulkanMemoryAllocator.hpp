#pragma once

#include <vulkan/vulkan.h>

#include "../Commands/VulkanCommandPool.hpp"

typedef struct VmaAllocator_T *VmaAllocator;

namespace Engine::Graphics::Vulkan {

struct Instance;
struct PhysicalDevice;
struct LogicalDevice;

struct MemoryAllocator {
    VmaAllocator handle = nullptr;

    CommandPool commandPool;
};

MemoryAllocator CreateMemoryAllocator(
    const Instance &instance,
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice);
void DestroyMemoryAllocator(const LogicalDevice &logicalDevice, MemoryAllocator &memoryAllocator);

}
