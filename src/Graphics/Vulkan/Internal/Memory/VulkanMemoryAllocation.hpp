#pragma once

#include <cstddef>

#include <volk.h>

typedef struct VmaAllocation_T *VmaAllocation;
struct VmaAllocationCreateInfo;

namespace Engine::Graphics::Vulkan {

struct MemoryAllocator;
struct Buffer;

struct MemoryAllocation {
    VmaAllocation handle = nullptr;
    void *mappedData = nullptr;
    std::size_t size = 0;
    std::size_t offset = 0;
};

void MapMemory(
    const MemoryAllocator &memoryAllocator,
    const MemoryAllocation &memoryAllocation,
    const void *data,
    std::size_t size,
    std::size_t offset);

Buffer CreateBuffer(
    const MemoryAllocator &memoryAllocator,
    const VkBufferCreateInfo &vkBufferCreateInfo,
    const VmaAllocationCreateInfo &vmaAllocationCreateInfo,
    std::size_t offset = 0);
void DestroyBuffer(const MemoryAllocator &memoryAllocator, Buffer &buffer);

}
