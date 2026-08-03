#include <vk_mem_alloc.h>

#include "VulkanMemoryAllocation.hpp"

#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>

#include "VulkanMemoryAllocator.hpp"

#include "../Commands/VulkanCommandPool.hpp"
#include "../Core/VulkanInstance.hpp"
#include "../Core/VulkanLogicalDevice.hpp"
#include "../Core/VulkanPhysicalDevice.hpp"
#include "../Resources/VulkanBuffer.hpp"

namespace Engine::Graphics::Vulkan {

void MapMemory(
    const MemoryAllocator &memoryAllocator,
    const MemoryAllocation &memoryAllocation,
    const void *data,
    std::size_t size,
    std::size_t offset)
{
    if (data == nullptr || size == 0)
        return;

    if (memoryAllocation.mappedData != nullptr) {
        std::memcpy(static_cast<uint8_t *>(memoryAllocation.mappedData) + offset, data, size);
        return;
    }

    void *mappedData = nullptr;
    VkResult vkResult = vmaMapMemory(memoryAllocator.handle, memoryAllocation.handle, &mappedData);
    if (vkResult != VK_SUCCESS) {
        throw std::runtime_error(std::format(
            "Failed to Map VMA Memory. Error Code: {}", static_cast<int>(vkResult)));
    }

    std::memcpy(static_cast<uint8_t *>(mappedData) + offset, data, size);

    vmaFlushAllocation(memoryAllocator.handle, memoryAllocation.handle, offset, size);
    vmaUnmapMemory(memoryAllocator.handle, memoryAllocation.handle);
}

Buffer CreateBuffer(
    const MemoryAllocator &memoryAllocator,
    const VkBufferCreateInfo &vkBufferCreateInfo,
    const VmaAllocationCreateInfo &vmaAllocationCreateInfo,
    std::size_t offset)
{
    VkBuffer vkBuffer = VK_NULL_HANDLE;
    VmaAllocation vmaAllocation = nullptr;
    VmaAllocationInfo vmaAllocationInfo {};

    VkResult vkResult = vmaCreateBuffer(
        memoryAllocator.handle,
        &vkBufferCreateInfo,
        &vmaAllocationCreateInfo,
        &vkBuffer,
        &vmaAllocation,
        &vmaAllocationInfo);

    if (vkResult != VK_SUCCESS) {
        throw std::runtime_error(std::format(
            "Failed to Create Vulkan Buffer. Error Code: {}",
            static_cast<int>(vkResult)));
    }

    return Buffer {
        .handle = vkBuffer,
        .allocation = {
            .handle = vmaAllocation,
            .mappedData = vmaAllocationInfo.pMappedData,
            .size = vkBufferCreateInfo.size,
            .offset = offset,
        },
    };
}

void DestroyBuffer(const MemoryAllocator &memoryAllocator, Buffer &buffer)
{
    if (buffer.handle != VK_NULL_HANDLE && buffer.allocation.handle != nullptr) {
        vmaDestroyBuffer(memoryAllocator.handle, buffer.handle, buffer.allocation.handle);
        buffer.handle = VK_NULL_HANDLE;
        buffer.allocation = MemoryAllocation {};
    }
}

}
