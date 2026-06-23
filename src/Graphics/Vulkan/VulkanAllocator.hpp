#pragma once

#include <cstddef>
#include <vector>

#include "../Allocator.hpp"

#include "VMA.hpp"

#include "Procedural/Commands/VulkanCommandPool.hpp"
#include "Procedural/Resources/VulkanBuffer.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;

struct Instance;
struct PhysicalDevice;
struct LogicalDevice;

struct Buffer;

class AllocatorBackend final : public Engine::Graphics::Allocator {
public:
    AllocatorBackend(const CoreContextBackend &coreContext);
    ~AllocatorBackend() override;

    void Init() override;
    void Destroy() override;

    BufferHandle CreateBuffer(const BufferCreateInfo &info, const void *data) override;
    void DestroyBuffer(BufferHandle handle) override;

    // Getters
    const Buffer &GetBuffer(BufferHandle handle) const { return m_buffers[handle.id]; }

private:
    BufferHandle CreateRawBuffer(
        VkBufferCreateInfo vkBufferCreateInfo,
        VmaAllocationCreateInfo vmaAllocationCreateInfo,
        std::size_t size);

    const CoreContextBackend &m_coreContext;

    CommandPool m_commandPool;

    VmaAllocator m_handle = nullptr;

    std::vector<Buffer> m_buffers;
    std::vector<std::size_t> m_freeBufferIndices;
};

}
