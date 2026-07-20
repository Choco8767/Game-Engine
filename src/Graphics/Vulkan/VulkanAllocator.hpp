#pragma once

#include <cstddef>
#include <vector>

#include "../Allocator.hpp"

#include "VMA.hpp"

#include "Procedural/Commands/VulkanCommandPool.hpp"

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

    BufferHandle CreateBuffer(const BufferCreateInfo &info, const void *data = nullptr) override;
    void DestroyBuffer(BufferHandle handle) override;
    void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) override;

    // Getters
    const Buffer &GetBuffer(BufferHandle handle) const;

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
