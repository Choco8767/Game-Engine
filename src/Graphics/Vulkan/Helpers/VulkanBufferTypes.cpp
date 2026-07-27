#include "VulkanBufferTypes.hpp"

namespace Engine::Graphics::Vulkan {

VkBufferUsageFlags MapBufferUsage(BufferUsage usage)
{
    switch (usage) {
    case Engine::Graphics::BufferUsage::VERTEX:
        return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case Engine::Graphics::BufferUsage::INDEX:
        return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    case Engine::Graphics::BufferUsage::UNIFORM:
        return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case Engine::Graphics::BufferUsage::STORAGE:
        return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    default:
        return 0;
    }
}

BufferCreateInfo MapBufferCreateInfo(
    const Graphics::BufferCreateInfo &info,
    bool isStaged)
{
    VkBufferUsageFlags vkBufferUsageFlags = MapBufferUsage(info.usage);
    VmaAllocationCreateFlags vmaAllocationFlags = 0;

    if (isStaged)
        vkBufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    else {
        switch (info.usage) {
        case BufferUsage::VERTEX:
        case BufferUsage::INDEX:
        case BufferUsage::UNIFORM:
            vmaAllocationFlags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
            vmaAllocationFlags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
            break;

        case BufferUsage::STORAGE:
            vmaAllocationFlags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
            vmaAllocationFlags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
            break;
        }
    }

    VkBufferCreateInfo vkBufferCreateInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = info.size,
        .usage = vkBufferUsageFlags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };

    VmaAllocationCreateInfo vmaAllocationCreateInfo {
        .flags = vmaAllocationFlags,
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    return BufferCreateInfo {
        vkBufferCreateInfo,
        vmaAllocationCreateInfo
    };
}

}
